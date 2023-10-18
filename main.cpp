#include <iostream>
#include "CGUtilities.h"
#include "../../../../../Downloads/CGPrakt2/CGPrakt2/src/rgbimage.h"
#include "SimpleRayTracer.h"
#include "CG_Helpers/test3.h"
#include "CG_Helpers/test2.h"
#include "CG_Helpers/test1.h"


int main(int argc, const char * argv[]) {
    // uncomment the following functions to start the corresponding test routines
//    Vector v1(0,1,1);
//    v1.test();


	Test1::vector();
    Test2::color();
    Test3::rgbimage("C:\\Users\\quint\\Documents\\Studium\\HSOS\\Computergrafik\\CGG\\Output_Images");

//    Scene scene(0);
//    RGBImage image(640,480);
//    SimpleRayTracer tracer(2);
//    tracer.traceScene(scene, image);
//    image.saveToDisk("raytracing_image.bmp");
//    return 0;
}
