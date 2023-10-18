//
//  SimpleRayTracer.cpp
//  SimpleRayTracer
//
//  Created by Philipp Lensing on 03.09.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "SimpleRayTracer.h"
#include "../../../../../Downloads/CGPrakt2/CGPrakt2/src/rgbimage.h"
#include <stdio.h>
#include <math.h>


Camera::Camera(float zvalue, float planedist, float width, float height, unsigned int widthInPixel, unsigned int heightInPixel) {
    ZValue = zvalue;
    PlaneDist = planedist;
    Width = width;
    Height = height;
    WidthInPixel = widthInPixel;
    HeightInPixel = heightInPixel;
}

Vector Camera::generateRay(unsigned int x, unsigned int y) const {
//    for (int x = 0; x < WidthInPixel; x++) {
//        for (int y = 0; y < HeightInPixel; y++) {
//            float x1 = (x - WidthInPixel / 2) * Width / WidthInPixel;
//            float y1 = (y - HeightInPixel / 2) * Height / HeightInPixel;
//            Vector v(x1, y1, ZValue);
//            return v;
//        }
//    }
    return Vector(); // dummy (remove)
}

Vector Camera::Position() const {
    // TODO: Add your code
    return Vector(); // dummy (remove)
}

SimpleRayTracer::SimpleRayTracer(unsigned int MaxDepth) {
    // TODO: Add your code
}


void SimpleRayTracer::traceScene(const Scene &SceneModel, RGBImage &Image) {
    // TODO: Add your code
}

Color
SimpleRayTracer::localIllumination(const Vector &Surface, const Vector &Eye, const Vector &N, const PointLight &Light,
                                   const Material &Mtrl) {
    // TODO: Add your code
    return Color(); // dummy (remove)
}

Color SimpleRayTracer::trace(const Scene &SceneModel, const Vector &o, const Vector &d, int depth) {
    // TODO: Add your code
    return Color(); // dummy (remove)
}

