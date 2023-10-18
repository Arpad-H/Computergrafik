//
//  test2.cpp
//  SimpleRayTracer
//
//  Created by Philipp Lensing on 19.09.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "test2.h"
#include <assert.h>
#include <math.h>
#include "color.h"


#define TEST( Msg, Cond) \
{ if(!(Cond)) { printf( "TEST FAILED: "); printf(Msg);printf( "\n"); }\
assert( Cond); }

#define EPSILON 1e-6

bool equalsc( const Color& c1, const Color& c2)
{
    return fabs(c1.R-c2.R)<EPSILON && fabs(c1.G-c2.G)<EPSILON  && fabs(c1.B-c2.B)<EPSILON;
}

void Test2::color()
{
    printf( "Start Color Test\n");
    
    Color a, b( 1,0.5f, 0.25f);
    
    a.R = 1.0f;
    a.G = 0.5f;
    a.B = 0.25f;
    
    TEST( "Color initialization failed", equalsc(a,b));
    
    Color atimesb = a * b;
    
    TEST( "Color component multiplication failed", equalsc( atimesb, Color( a.R*b.R, a.G*b.G, a.B*b.B)));
    
    Color as = a * 2.0f;
    TEST( "Color scaling failed", equalsc( as, Color( a.R*2.0f, a.G*2.0f, a.B*2.0f)));
    
    Color aplusb = a + b;
    TEST( "Color addition or scaling failed", equalsc( aplusb, Color( a.R*2.0f, a.G*2.0f, a.B*2.0f)));
    
    a += b;
    
    TEST( "Color operator+= failed", equalsc( aplusb, a));
    
    
    printf( "Color Test Succeeded\n");
  
    
    
}

