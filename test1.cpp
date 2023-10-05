//
//  test1.cpp
//  SimpleRayTracer
//
//  Created by Philipp Lensing on 19.09.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "test1.h"
#include <assert.h>
#include <math.h>
#include "vector.h"


#define TEST( Msg, Cond) \
{ if(!(Cond)) { printf( "TEST FAILED: "); printf(Msg);printf( "\n"); }\
assert( Cond); }

#define EPSILON 1e-6

bool equals( const Vector& c1, const Vector& c2)
{
    return fabs(c1.X-c2.X)<EPSILON && fabs(c1.Y-c2.Y)<EPSILON  && fabs(c1.Z-c2.Z)<EPSILON;
}

void Test1::vector()
{

    printf( "Start Vector Test\n");
    Vector a, b(1,2,3);
    a.X = 1;
    a.Y = 2;
    a.Z = 3;
    
    TEST( "Vector initialization failed!", equals(a,b));

    TEST( "Vector lengthSquared failed!", (a.X*a.X + a.Y*a.Y + a.Z*a.Z)-EPSILON <= a.lengthSquared() && a.lengthSquared() <= (a.X*a.X + a.Y*a.Y + a.Z*a.Z)+EPSILON );

    TEST( "Vector length failed!", sqrt(a.X*a.X + a.Y*a.Y + a.Z*a.Z)-EPSILON <= a.length() && a.length() <= sqrt(a.X*a.X + a.Y*a.Y + a.Z*a.Z)+EPSILON );
    
    TEST( "Vector operator+ failed!", equals( a+b, Vector(2,4,6)) );
    TEST( "Vector operator- failed!", equals( a-b, Vector(0,0,0)) );
    
    a+=b;
    TEST( "Vector operator+= failed!", equals( a, Vector(2,4,6)) );
    
    a.normalize();
    b = b * (1.0f/b.length());
    TEST( "Vector normalize failed!", 1.0f-EPSILON <= a.length() && a.length() <= 1.0f+EPSILON );
    TEST( "Vector normalize, operator* or dot product failed!", a.dot(b) <= 1.0f+EPSILON && a.dot(b) >= 1.0f-EPSILON);
    
    
    Vector nx(1,0,0);
    Vector ny(0,1,0);
    Vector nz(0,0,1);

    TEST( "Vector dot product failed!", fabs(nx.dot(ny)) < EPSILON);
    TEST( "Vector dot product failed!", fabs(nx.dot(nz)) < EPSILON);
    TEST( "Vector dot product failed!", fabs(nz.dot(ny)) < EPSILON);
    TEST( "Vector dot product failed!", fabs(nz.dot(nz)) > 1.0f-EPSILON);
    
    Vector nnz = ny.cross(nx);
    
    TEST( "Vector cross product or operatore-() failed!", equals( nnz, -nz));
    
    nx = nx * 2.0f;
    TEST( "Vector scaling failed!", nx.length() < (2.0f + EPSILON) && nx.length() > (2.0f-EPSILON) );
    TEST( "Vector dot failed!", nx.dot(nx) < (2.0f*2.0f + EPSILON) && nx.dot(nx) > (2.0f*2.0f-EPSILON) );
    TEST( "Vector cross failed!", nx.cross(ny).length() < (2.0f + EPSILON) && nx.cross(ny).length() > (2.0f-EPSILON) );
    nx = nx * 0.5f;
    TEST( "Vector scaling failed!", nx.length() < (1.0f + EPSILON) && nx.length() > (1.0f-EPSILON) );
    
    
    Vector v(1,-1,1);
    v.normalize();
    Vector r = v.reflection(ny);

    TEST("Vector reflection failed!", equals(r, Vector(1,1,1).normalize()));
    
    for( int i=0; i<1000; i++)
    {
        Vector vr( (float)(rand()%10000)-5000.0f, (float)(rand()%10000)-5000.0f, float(rand()%10000)-5000.0f  );
        if(vr.lengthSquared() > 0)
        {
            vr.normalize();
            
            Vector rx = vr.reflection(nx);
            Vector ry = vr.reflection(ny);
            Vector rz = vr.reflection(nz);
            
            TEST( "Vector reflection, operator- or dot product failed", fabs(nx.dot(rx)-nx.dot(-vr)) <= EPSILON  );
            TEST( "Vector reflection, operator- or dot product failed", fabs(ny.dot(ry)-ny.dot(-vr)) <= EPSILON  );
            TEST( "Vector reflection, operator- or dot product failed", fabs(nz.dot(rz)-nz.dot(-vr)) <= EPSILON  );
        }
        
    }
    
    
    
    {
        // intersectiontest with xy-plane
        Vector a(0,0,0), b(1,0,0), c(0,1,0), d(1,1,0), n(0,0,1);
        
    
        for( int i=0; i<20000; i++)
        {
            float z= float(rand()%2000)-1000.0f;
            if( z==0) z+=1.0f;
            Vector o( (float)(rand()%2000)-1000.0f, (float)(rand()%2000)-1000.0f, z );
            o = o*0.01f;
            
            Vector dest( (float)(rand()%2000)-1000.0f, (float)(rand()%2000)-1000.0f, 0 );
            dest = dest*0.001f;
            
            Vector Dir = dest-o;
            Dir.normalize();
            
            
            bool h1 = false, h2=false;
            float s1=0, s2=0;
            
            h1 = o.triangleIntersection(Dir, a, b, c, s1);
            h2 = o.triangleIntersection(Dir, c, b, d, s2);
            
            if( dest.X >= 0 && dest.X <= 1 && dest.Y >= 0 && dest.Y <=1)
            {
                TEST( "Vector triangle Intersection failed (missed intersection)!", h1 || h2 );
                
                float s = h1 ? s1 : s2;
                
                Vector intersection = o + Dir * s;
             
                TEST( "Vector triangle Intersection failed (wrong s coordinate!",  intersection.dot(n ) <= EPSILON);
                
                h1 = o.triangleIntersection(-Dir, a, b, c, s1);
                h2 = o.triangleIntersection(-Dir, c, b, d, s2);
                TEST( "Vector triangle Intersection failed (missed intersection)!", !h1 && !h2 );
                
            }
            else
            {
                TEST( "Vector triangle Intersection failed (missed intersection)!", !h1 && !h2 );
            }
                
        }
        
    }
    {
        // intersectiontest with xz-plane
        Vector a(0,0,0), b(-1,0,0), c(0,0,-1), d(-1,0,-1), n(0,-1,0);
        
        
        for( int i=0; i<20000; i++)
        {
            float y= float(rand()%2000)-1000.0f;
            if( y==0) y+=1.0f;
            Vector o( (float)(rand()%2000)-1000.0f, y, (float)(rand()%2000)-1000.0f );
            o = o*0.01f;
            
            Vector dest( (float)(rand()%2000)-1000.0f, 0, (float)(rand()%2000)-1000.0f );
            dest = dest*0.001f;
            
            Vector Dir = dest-o;
            Dir.normalize();
            
            
            bool h1 = false, h2=false;
            float s1=0, s2=0;
            
            h1 = o.triangleIntersection(Dir, a, b, c, s1);
            h2 = o.triangleIntersection(Dir, c, b, d, s2);
            
            if( dest.X <= 0 && dest.X >= -1 && dest.Z <= 0 && dest.Z >=-1)
            {
                TEST( "Vector triangle Intersection failed (missed intersection)!", h1 || h2 );
                
                float s = h1 ? s1 : s2;
                
                Vector intersection = o + Dir * s;
                
                TEST( "Vector triangle Intersection failed (wrong s coordinate!",  intersection.dot(n ) <= EPSILON);
                
                h1 = o.triangleIntersection(-Dir, a, b, c, s1);
                h2 = o.triangleIntersection(-Dir, c, b, d, s2);
                TEST( "Vector triangle Intersection failed (missed intersection)!", !h1 && !h2 );
                
            }
            else
            {
                TEST( "Vector triangle Intersection failed (missed intersection)!", !h1 && !h2 );
            }
            
        }
        
    }
    
    {
        // intersectiontest with xz-plane
        Vector a(0,1,1), b(0,2,1), c(0,1,2), d(0,2,2), n(1,0,0);
        
        
        for( int i=0; i<20000; i++)
        {
            float x= float(rand()%2000)-1000.0f;
            if( x==0) x+=1.0f;
            Vector o( x, (float)(rand()%4000)-2000.0f, (float)(rand()%4000)-2000.0f );
            o = o*0.01f;
            
            Vector dest( 0, (float)(rand()%2000), (float)(rand()%2000) );
            dest = dest*0.001f;
            
            Vector Dir = dest-o;
            Dir.normalize();
            
            
            bool h1 = false, h2=false;
            float s1=0, s2=0;
            
            h1 = o.triangleIntersection(Dir, a, b, c, s1);
            h2 = o.triangleIntersection(Dir, c, b, d, s2);
            
            if( dest.Y >= 1.0f && dest.Y <= 2 && dest.Z >= 1.0f && dest.Z <=2)
            {
                TEST( "Vector triangle Intersection failed (missed intersection)!", h1 || h2 );
                
                float s = h1 ? s1 : s2;
                
                Vector intersection = o + Dir * s;
                
                TEST( "Vector triangle Intersection failed (wrong s coordinate!",  intersection.dot(n ) <= EPSILON);
                
                h1 = o.triangleIntersection(-Dir, a, b, c, s1);
                h2 = o.triangleIntersection(-Dir, c, b, d, s2);
                TEST( "Vector triangle Intersection failed (missed intersection)!", !h1 && !h2 );
                
            }
            else
            {
                TEST( "Vector triangle Intersection failed (missed intersection)!", !h1 && !h2 );
            }
            
        }
        
    }
    
    printf("Vector test succeeded!\n");
}