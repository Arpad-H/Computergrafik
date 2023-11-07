//
//  SimpleRayTracer.cpp
//  SimpleRayTracer
//
//  Created by Philipp Lensing on 03.09.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "SimpleRayTracer.h"
#include "CG_Helpers/rgbimage.h"
#include <stdio.h>
#include <math.h>
#include <cstdint>

#define EPSILON 1e-6

Camera::Camera(float zvalue, float planedist, float width, float height, unsigned int widthInPixel,
               unsigned int heightInPixel) {
    ZValue = zvalue;
    PlaneDist = planedist;
    Width = width;
    Height = height;
    WidthInPixel = widthInPixel;
    HeightInPixel = heightInPixel;
}

Vector Camera::generateRay(unsigned int x, unsigned int y) const {

    float x1 = -Width/2+x * Width / WidthInPixel;
    float y1 = -(-Height/2+y * Height / HeightInPixel);
    Vector v(x1, y1, PlaneDist);
    return v.normalize();

}

Vector Camera::Position() const {

    return Vector(0, 0, ZValue);
}

SimpleRayTracer::SimpleRayTracer(unsigned int MaxDepth) {
    m_MaxDepth = MaxDepth;
}


void SimpleRayTracer::traceScene(const Scene &SceneModel, RGBImage &Image) {
    Camera camera(-8, 1, 1, 0.75, 640, 480);
    for (int x = 0; x < Image.width(); x++) {
        for (int y = 0; y < Image.height(); y++) {
            Color c = trace(SceneModel, camera.Position(), camera.generateRay(x, y), 1);
            Image.setPixelColor(x, y, c);
        }
    }

}

Vector reflect(const Vector& I, const Vector& N) {

    // Calculate the reflection direction
    // d - normal * (d.dot(normal)) * 2
    Vector R = I -  N*(I.dot(N))*2;

    return R.normalize();
}
Color SimpleRayTracer::localIllumination(const Vector &Surface, const Vector &Eye, const Vector &N, const PointLight &Light,
                                   const Material &Mtrl) {
    // Calculate the direction from the surface point to the light source
    Vector L = (Light.Position - Surface).normalize();

    // Calculate the reflection direction (assuming a perfect mirror reflection)
    Vector R = reflect(-L,N);

    // Calculate the diffuse component of local illumination
    float NdotL = N.dot(L);
    Color DiffuseColor = Mtrl.getDiffuseCoeff(Surface) * Light.Intensity * std::max(0.0f, NdotL);

    // Calculate the specular component of local illumination (Blinn-Phong model)

    float SpecularExponent = Mtrl.getSpecularExp(Surface);
    float SpecularTerm = std::pow(std::max(0.0f, Eye.dot(R)), SpecularExponent);
    Color SpecularColor = Mtrl.getSpecularCoeff(Surface) * Light.Intensity * SpecularTerm;

    // Combine diffuse and specular components
    Color LocalIllumination = DiffuseColor + SpecularColor + Mtrl.getAmbientCoeff(Surface) * Light.Intensity;


    return LocalIllumination;
}

Color SimpleRayTracer::trace(const Scene &SceneModel, const Vector &o, const Vector &d, int depth) {
    if (depth >= m_MaxDepth+1 || depth == 0) return Color(0, 0, 0);
    float lastHitDistance = INT64_MAX;
    Triangle closestTriangle;

    for (int i = 0; i < SceneModel.getTriangleCount(); ++i) {
        float distance;
        Triangle t = SceneModel.getTriangle(i);
        if (o.triangleIntersection(d, t.A, t.B, t.C, distance)) {
            if (distance < lastHitDistance && distance > EPSILON) {
                lastHitDistance = distance;
                closestTriangle = t;

            }
        }
    }
    if (lastHitDistance == INT64_MAX) {

        return Color(0, 0, 0);
    }
    Vector surfacePoint = o + (d * lastHitDistance);
    Vector normal = closestTriangle.calcNormal(surfacePoint);

    //Lokale Beleuchtung
    Color localLighting = Color();
    for (int j = 0; j < SceneModel.getLightCount(); j++) {
        localLighting += (localIllumination(surfacePoint, d * -1, normal, SceneModel.getLight(j),
                                           *closestTriangle.pMtrl));
    }

    //Reflektionen
//    d - normal * (d.dot(normal)) * 2
    Vector reflectionDirection = reflect(d,normal);
    Color reflectionColor = trace(SceneModel, surfacePoint, reflectionDirection, depth + 1)*closestTriangle.pMtrl->getReflectivity(surfacePoint);


    //Kombinieren
    Color finalColor = localLighting +reflectionColor;

    return finalColor;
    //return closestTriangle.pMtrl->getDiffuseCoeff(o + (d * distance));


}



