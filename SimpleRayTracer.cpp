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

#define EPSILON 0.0001f

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

    float x1 = -Width / 2 + x * Width / WidthInPixel;
    float y1 = -(-Height / 2 + y * Height / HeightInPixel);
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
//            std::cout << "y: " << y << std::endl;
        }
//        std::cout << "x: " << x << std::endl;
    }
//int x= 320;
//int y= 450;
//    Color c = trace(SceneModel, camera.Position(), camera.generateRay(x,y),1);
//    Image.setPixelColor(x, y, c);
}

Vector reflect(const Vector &I, const Vector &N) {
    // Calculate the reflection direction
    // d - normal * (d.dot(normal)) * 2
    Vector R = I - N * (2 * I.dot(N));
    if (R.length() == 0) {
        return R;
    }
    return R.normalize();
}

Color
SimpleRayTracer::localIllumination(const Vector &Surface, const Vector &Eye, const Vector &N, const PointLight &Light,
                                   const Material &Mtrl) {

    // Calculate the direction from the surface point to the light source
    Vector L = (Light.Position - Surface).normalize();
    Vector R = reflect(-L, N);

    // Calculate the diffuse component of local illumination
    //Folie 17 D=K*I*max(0,N*L)
    float NdotL = N.dot(L);
    Color DiffuseColor = Mtrl.getDiffuseCoeff(Surface) * Light.Intensity * std::max(0.0f, NdotL);

    // Calculate the specular component of local illumination (Phong model)
    //Folie 18 S=K*I*max(0,R*E)^n
    float SpecularExponent = Mtrl.getSpecularExp(Surface);
    float SpecularTerm = std::pow(std::max(0.0f, Eye.dot(R)), SpecularExponent);
    Color SpecularColor = Mtrl.getSpecularCoeff(Surface) * Light.Intensity * SpecularTerm;

    // Combine diffuse and specular components
//    Color LocalIllumination = DiffuseColor + SpecularColor + Mtrl.getAmbientCoeff(Surface) * Light.Intensity;
    Color LocalIllumination = DiffuseColor + SpecularColor;
    return LocalIllumination;
}

Color SimpleRayTracer::trace(const Scene &SceneModel, const Vector &o, const Vector &d, int depth) {

    if (depth >= m_MaxDepth + 1 || depth == 0) return Color(0, 0, 0);

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

//    closestTriangle.pMtrl->test();


///////////////////////////////////////////////////

//////////////////////////////////////////////////



    if (closestTriangle.pMtrl->n1 != 1 || closestTriangle.pMtrl->n2 != 1) {

        float n1 = closestTriangle.pMtrl->getN1();
        float n2 = closestTriangle.pMtrl->getN2();

        //Snells Law
        if (normal.dot(d) > 0) {
            normal = -normal;
            float temp = n1;
            n1 = n2;
            n2 = temp;
        }
        Vector refractedDirection = closestTriangle.pMtrl->refract(d, normal, n1, n2);
        Vector reflectionDirection = reflect(d, normal);
        Color refractedColor;
        Color reflectedColor;
//        if ( refractedDirection.X != INT64_MAX ){
            refractedColor = trace(SceneModel, surfacePoint, refractedDirection.normalize(), depth + 1);
//        }else {
            reflectedColor = trace(SceneModel, surfacePoint, reflectionDirection, depth + 1) *
                             closestTriangle.pMtrl->getReflectivity(surfacePoint);
//        }
        float schlick = closestTriangle.pMtrl->schlick(surfacePoint, normal, n1, n2);
        return reflectedColor * schlick + refractedColor * (1 - schlick);
    }


    Color reflectionColor = Color(0, 0, 0);
    Color localLighting = Color(0, 0, 0);



//Check for eyecontact with lightsource
//Phong reflection model on surfacepoint
    for (int i = 0; i < SceneModel.getLightCount(); ++i) {
        PointLight light = SceneModel.getLight(i);

        bool inShadow = false;
        Vector dirToLight = (light.Position - surfacePoint).normalize();
        float distanceToLight = (light.Position - surfacePoint).length();
        for (int j = 0;
             j < SceneModel.getTriangleCount(); ++j) {
            Triangle t = SceneModel.getTriangle(j);
            float distance;
            if (surfacePoint.triangleIntersection(dirToLight, t.A, t.B, t.C, distance)) {
                if (distance > EPSILON && distance < distanceToLight) {
                    inShadow = true;
                    break;
                }
            }
        }
        if (!inShadow) {
//Local Illumination
            localLighting += (localIllumination(surfacePoint, -d, normal, light, *closestTriangle.pMtrl));
        }

        reflectionColor = trace(SceneModel, surfacePoint, reflect(d, normal), depth + 1) *
                          closestTriangle.pMtrl->getReflectivity(surfacePoint);
    }

    Color finalColor = localLighting + reflectionColor + closestTriangle.pMtrl->getAmbientCoeff(surfacePoint);
    return
            finalColor;













/////////////////////////////////////////////////

//    Refraktion
//     wenn eins ungleich 1 dann findet refraktion statt
//    if (closestTriangle.pMtrl->n1 != 1 || closestTriangle.pMtrl->n2 != 1) {
//        float n1 = closestTriangle.pMtrl->getN1();
//        float n2 = closestTriangle.pMtrl->getN2();
//
//        //Snells Law
//        if (normal.dot(d) > 0) {
//            normal =-normal ;
//            float temp = n1;
//            n1 = n2;
//            n2 = temp;
//        }
//        Vector refractedDirection = closestTriangle.pMtrl->refract(d, normal, n1, n2);
//
////        check if it got refracted
//        if (refractedDirection.X != INT64_MAX && refractedDirection.Y != INT64_MAX &&
//            refractedDirection.Z != INT64_MAX) {
//
////            generate random number between 0 and 1
//            float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//            if(random > closestTriangle.pMtrl->schlick(surfacePoint, normal, n1, n2)){
////                std::cout << "refracted" << std::endl;
//                newDir = refractedDirection;
//            }else{
//                newDir = reflect(d, normal);
//            }
//        }
//            if not its reflected
//        else {
//            newDir = reflect(d, normal);
//        }

//    }

//Combine



}



