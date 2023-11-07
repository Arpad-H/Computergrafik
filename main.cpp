#include <iostream>
#include "CGUtilities.h"
#include "SimpleRayTracer.h"
#include "CG_Helpers/test3.h"
#include "CG_Helpers/test2.h"
#include "CG_Helpers/test1.h"
#include "CG_Helpers/rgbimage.h"


int main(int argc, const char * argv[]) {
    // uncomment the following functions to start the corresponding test routines
//    Vector v1(0,1,1);
//    v1.test();


//	Test1::vector();
//    Test2::color();
//    Test3::rgbimage("C:\\Users\\Arpad\\Documents\\Studium\\Computergrafik\\Output_Images");

    Scene scene(20);
    RGBImage image(640,480);
    SimpleRayTracer tracer(2);
    tracer.traceScene(scene, image);
    image.saveToDisk("C:\\\\Users\\\\quint\\\\Documents\\\\Studium\\\\HSOS\\\\Computergrafik\\\\CGG\\\\Output_Images\\\\raytracing_image.bmp");
    return 0;
}
