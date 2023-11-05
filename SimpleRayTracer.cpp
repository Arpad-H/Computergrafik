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
//    x =640;
//    y=480;
    float x1 = -Width/2+x * Width / WidthInPixel;
    float y1 = -Height/2+y * Height / HeightInPixel;
   // std::cout << x1 << " " << y1 << "\n";
    Vector v(x1, y1, PlaneDist);
    return v.normalize();
//    float x1 = (x - WidthInPixel / 2) * Width / WidthInPixel;
//    float y1 = -((y - HeightInPixel / 2) * Height / HeightInPixel);
//    Vector v(x1, y1, PlaneDist);
//    return v.normalize();
//        }
//    }
}

Vector Camera::Position() const {

    return Vector(Width / 2,
                  Height / 2,
                  ZValue);
}

SimpleRayTracer::SimpleRayTracer(unsigned int MaxDepth) {
    m_MaxDepth = MaxDepth;
}


void SimpleRayTracer::traceScene(const Scene &SceneModel, RGBImage &Image) {
    Camera camera(-8, 1,1,0.75, 640,480);
    for (int x = 0; x <Image.width() ; x++) {
        for (int y = 0; y < Image.height(); y++) {
           Color c = trace(SceneModel, camera.Position() ,camera.generateRay(x,y), m_MaxDepth);
           Image.setPixelColor(x,y,c);
        }
    }

}

Color
SimpleRayTracer::localIllumination(const Vector &Surface, const Vector &Eye, const Vector &N, const PointLight &Light,
                                   const Material &Mtrl) {
    // TODO: Temp
    return Mtrl.getDiffuseCoeff(Surface);
}

Color SimpleRayTracer::trace(const Scene &SceneModel, const Vector &o, const Vector &d, int depth) {
    if(depth == 0) return Color(0,0,0);
    float lastHitDistance = INT64_MAX;
    Triangle closestTriangle;
    for (int i = 0; i < SceneModel.getTriangleCount(); ++i) {
        Triangle t = SceneModel.getTriangle(i);
        float distance;
       if( o.triangleIntersection(d,t.A,t.B,t.C,distance)) {
           if (distance < lastHitDistance) {
               lastHitDistance = distance;
               closestTriangle = t;
           }
       }else{
          // std::cout << "no intersection\n" <<SceneModel.getTriangleCount();
       }
    }
//std::cout <<"(" <<closestTriangle.pMtrl->getDiffuseCoeff(o+d).R <<"," << closestTriangle.pMtrl->getDiffuseCoeff(o+d).G << ","<< closestTriangle.pMtrl->getDiffuseCoeff(o+d).B << ")\n";
    return closestTriangle.pMtrl->getDiffuseCoeff(o+d);
}

