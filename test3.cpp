//
//  test3.cpp
//  SimpleRayTracer
//
//  Created by Philipp Lensing on 19.09.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "test3.h"
#include <assert.h>
#include <math.h>
#include "rgbimage.h"
#include "color.h"

#define TEST( Msg, Cond) \
    { if(!(Cond)) { printf( "TEST FAILED: "); printf(Msg); }\
      assert( Cond); }

bool equals( const Color& c1, const Color& c2)
{
    return fabs(c1.R-c2.R)<1e-8 && fabs(c1.G-c2.G)<1e-8  && fabs(c1.B-c2.B)<1e-8;
}

void Test3::rgbimage( const char* Directory)
{
    printf( "Start RGBImage Test\n");
    // create 3 images
    RGBImage Img1( 800, 600);
    RGBImage Img2( 800, 600);
    RGBImage Img3( 800, 600);
    
    
    TEST( "Invalid Image Width\n", Img1.width()==800);
    TEST( "Invalid Image Height\n", Img1.height()==600);
    /*
    Img1.setPixelColor( 800, 100, Color());
    Color c = Img1.getPixelColor(800, 100);
    Img2.setPixelColor( 100, 600, Color());
    c = Img1.getPixelColor(100, 600);
    */
    Color c0(0,0,0);
    Color c1(1,0,0);
    Color c2(0,1,0);
    Color c3(0,0,1);

    for( unsigned int i=0; i<Img1.height(); i++ )
    {
        for( unsigned int j=0; j<Img1.width(); j++ )
        {
            float u = (float)j/(float)Img1.width();
            float v = (float)i/(float)Img1.height();
            Color cu0 = c0 *(1.0f-u) + c1*u;
            Color cu1 = c2 *(1.0f-u) + c3*u;
            Color co = cu0 *(1.0f-v) + cu1*v;
            Img1.setPixelColor(j, i, co);
            TEST( "Different colors for identical get/setPixelColor calls\n", equals(co, Img1.getPixelColor(j,i)));
        }
        
    }

    
    char Path[512];
    char CompletePath[512];
    size_t len = strlen( Directory );
    assert(len<=256);
    strcpy(Path, Directory);
    
    if(len>0 && Path[len-1] != '/' && Path[len-1] != '\\')
        strcat( Path, "/");
    
    strcpy( CompletePath, Path);
    strcat( CompletePath, "rgbtestimage1.bmp");
    TEST( "Unable to save Image!\n", Img1.saveToDisk(CompletePath));
    
    for( unsigned int i=0; i<Img1.height(); i++ )
    {
        for( unsigned int j=0; j<Img1.width(); j++ )
        {
            float cx = ((float)j-400.0f);
            float cy = ((float)i-300.0f);
            float r = sqrt(cx*cx + cy*cy);
            if(r<300)
            {
                r = 300-r;
                float rcx = cx * cos( r * 0.015f) - cy * sin(r* 0.015f);
                float rcy = cx * sin( r * 0.015f) + cy * cos(r* 0.015f);
                Color co = Img1.getPixelColor(400 + (int)rcx, 300 + (int)rcy);
                Img2.setPixelColor(j,i, co);
            }
            else
            {
                Color co = Img1.getPixelColor(j, i );
                Img2.setPixelColor(j,i, co);
            }
        }
    }
    
    strcpy( CompletePath, Path);
    strcat( CompletePath, "rgbtestimage2.bmp");
    TEST( "Unable to save Image!\n", Img2.saveToDisk(CompletePath));

    
    for( unsigned int i=0; i<Img2.height(); i++ )
    {
        for( unsigned int j=0; j<Img2.width(); j++ )
        {
            if( ((j/40)%2)==(i/40)%2 )
            {
                Color c = Color(1,1,1);
                c.R = c.R - Img2.getPixelColor(j,i).R;
                c.G = c.G - Img2.getPixelColor(j,i).G;
                c.B = c.B - Img2.getPixelColor(j,i).B;
                
                Img3.setPixelColor(j,i, c);
            }
            else
            {
                Img3.setPixelColor(j,i, Img2.getPixelColor(j,i));
            }

        }
    }
    
    strcpy( CompletePath, Path);
    strcat( CompletePath, "rgbtestimage3.bmp");
    TEST( "Unable to save Image!\n", Img3.saveToDisk(CompletePath));
    printf( "RGBImage Test succeeded if the saved bmp-images are equal to the reference images!\n");
}
