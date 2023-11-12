//
//  CGUtilities.cpp
//  SimpleRayTracer
//
//  Created by Philipp Lensing on 03.09.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "CGUtilities.h"
#include "assert.h"
#include "CG_Helpers/color.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <vector>
#include <map>
#include <cstdint>

class NoiseGrid
{
public:
    NoiseGrid( unsigned int size=16)
    {
        Grid = new float[size*size*size];
        assert("Unable to allocate enough memory for noise grid" && Grid);
        Size=size;
        
        for(unsigned int i=0; i<size; i++)
            for(unsigned int j=0; j<size; j++)
                for(unsigned int k=0; k<size; k++)
                    Cell(k,j,i) = (float)rand()/(float)RAND_MAX;

    }
    
    float value( float u, float v, float w )
    {
        if( u < 0 ) u = u + ceil( fabs(u));
        if( v < 0 ) v = v + ceil( fabs(v));
        if( w < 0 ) w = w + ceil( fabs(w));
        
        u*=Size;
        v*=Size;
        w*=Size;

        float _u = u - floor(u);
        float _v = v - floor(v);
        float _w = w - floor(w);
        
        
        unsigned int i = (unsigned int) u % Size;
        unsigned int j = (unsigned int) v % Size;
        unsigned int k = (unsigned int) w % Size;
        
        
        float value = 0;
        
        
        for(int a = 0; a < 2; a++)
            for(int b = 0; b < 2; b++)
                for(int c = 0; c < 2; c++)
                {
                    value += pow( _u, a) * pow( _v, b) * pow(_w, c) * pow( 1-_u, 1-a) * pow( 1-_v, 1-b) * pow( 1-_w, 1-c) * Cell( (i+a)%Size, (j+b)%Size, (k+c)%Size);
                }
        
        return value;
    }
    
private:
    float& Cell( unsigned int x, unsigned int y, unsigned int z) { assert(x<Size && y<Size && z<Size); return Grid[ z*Size*Size + y*Size + x ]; }
    float* Grid;
    unsigned int Size;
};

NoiseGrid Noisegrid;

class ProcMaterial : public Material
{
public:
    ProcMaterial()
    {
        m_DiffuseCoeff = Color(1.0f,0.5f,0);
        m_AmbientCoeff = Color(0.1f,0.1f,0.1f);
        m_SpecularCoeff = Color(0.6f,0.6f,0.6f);
        m_SpecularExp = 32;
        m_Reflectivity = 0.10f;
        n2 = 1.33;
        n1 = 1.0;
    }
//    ProcMaterial( const Color& Diffuse, const Color& Specular, const Color& Ambient, float SpecularExp, float OpticalDensityN,float RefractionIndex)
//        : Material( Diffuse, Specular, Ambient, SpecularExp, OpticalDensityN, RefractionIndex
//    {
//
//    }


    Color getDiffuseCoeff(const Vector& Pos) const
    {
        Vector P = Pos*2.0f + Vector( 2,0,0);
        float r = sqrt( P.X*P.X + P.Z * P.Z);

        r -= sin( (float)M_PI * r /3.0f /*/ 6*/);
        r += 0.1f * Noisegrid.value(P.X/6.0f, P.Z/6.0f, P.Y/6.0f);

        float d = fmod(r, 0.5f) / 0.5f;

        return m_DiffuseCoeff * sqrt(d) + m_DiffuseCoeff*0.8f * (1-sqrt(d));// Noisegrid.value(Pos.X , Pos.Y, Pos.Z);

    }

    static ProcMaterial DefaultMaterial;

};

ProcMaterial ProcMaterial::DefaultMaterial;

Material Material::DefaultMaterial;
Material Material::RedMtrl( Color(0.8f, 0.2f, 0.2f), Color(0.3f,0.3f,0.3f), Color(0.0,0.0f,0.0f), 16, 0.3f);
Material Material::GreenMtrl( Color(0.2f, 0.8f, 0.2f), Color(0.3f,0.3f,0.3f), Color(0.0,0.0f,0.0f), 16, 0.3);
Material Material::BlueMtrl( Color(0.2f, 0.2f, 0.8f), Color(0.3f,0.3f,0.3f), Color(0.0,0.0f,0.0f), 16, 0.3f);
Material Material::YellowMtrl( Color(0.8f, 0.8f, 0.2f), Color(0.3f,0.3f,0.3f), Color(0.0,0.0f,0.0f), 16, 0.3f);
Material Material::CyanMtrl( Color(0.2f, 0.8f, 0.8f), Color(0.3f,0.3f,0.3f), Color(0.0f,0.0f,0.0f), 16, 0.3f);
//Material Material::GlasMtrl( Color(0.0f, 0.0f, 0.0f), Color(0.0f,0.0f,0.0f), Color(0.0f,0.0f,0.0f), 16, 0.3f,1.33);


Material::Material()
{
    m_DiffuseCoeff = Color(0.5f,0.2f,0.2f);
    m_AmbientCoeff = Color(0.2f,0.2f,0.2f);
    m_SpecularCoeff = Color(0.4f,0.4f,0.4f);
    m_SpecularExp = 16;
    n1 = 1.0f;
    n2 = 1.0f;

    
}

Material::Material(const Color& Diffuse, const Color& Specular, const Color& Ambient, float SpecularExp, float Reflectivity)
{
    m_DiffuseCoeff = Diffuse;
    m_SpecularCoeff = Specular;
    m_AmbientCoeff = Ambient;
    m_SpecularExp = SpecularExp;
    m_Reflectivity = Reflectivity;
    n2 = 1.0f;
    n1 = 1.0f;
}

float Material::getReflectivity(const Vector& Pos) const
{
    return m_Reflectivity;
}

Color Material::getDiffuseCoeff(const Vector& Pos) const
{
    return m_DiffuseCoeff;
}
Color Material::getSpecularCoeff(const Vector& Pos) const
{
    return m_SpecularCoeff;
}

Color Material::getAmbientCoeff(const Vector& Pos) const
{
    return m_AmbientCoeff;
}

float Material::getSpecularExp(const Vector& Pos) const
{
    return m_SpecularExp;
}














//TODO: Implement this
float Material::schlick(const Vector &Dir, const Vector& Normal, const float &N1, const float &N2) const {
    //Schlick Aproximation folie 31
    float R0 = ((N1 - N2) / (N1 + N2)) * ((N1 - N2) / (N1 + N2));
    float R = R0 + (1 - R0) * pow((1 + Dir.dot(Normal)), 5);
    return R;

}
float Material::getTransmissionCoeff(const Vector &Pos, const Vector& Normal) const {
    return 1-schlick(Pos, Normal, n1, n2);
}

//Vector Material::refract(const Vector& Ray_in, const Vector& Normal , float N1, float N2) const
//{
    //Folie 26
    //T = n1/n2 (Ray_in - Normal(Ray_in · Normal)) - Normal sqrt( (1- n1^2(1-(Ray_in · Normal)^2) / n2^2))
//    float r = N1/N2;
//    float c = Ray_in.dot(Normal);
//    float discriminant = 1 - r*r*(1-c*c);
//    if (discriminant >= 0){
//        return (-(Ray_in * r) + Normal * (r * c - sqrt(discriminant)));
//    }
//    //Total internal reflection
//return Vector(INT64_MAX,INT64_MAX,INT64_MAX);
Vector Material::refract(const Vector& Ray_in, const Vector& Normal , float N1, float N2) const
    {
        const double n = N1 / N2;
        const double cosI = -Normal.dot(Ray_in);
        const double sinT2 = n * n * (1.0 - cosI * cosI);
//        if(sinT2 > 1.0) return reflect(Ray_in, Normal);
        if(sinT2 > 1.0) return Vector(INT64_MAX,INT64_MAX,INT64_MAX);
        const double cosT = sqrt(1.0 - sinT2);
        return Ray_in*n + Normal*(n * cosI - cosT);
    }
//}



Vector Material::reflect(const Vector &I, const Vector &N) const {

//    std::cout << "currently in  Reflect" << std::endl;

    float dotProduct = I.dot(N);
//    std::cout << "dotProduct: " << dotProduct << std::endl;
    float times2 = dotProduct * 2;
//    std::cout << "times2: " << times2 << std::endl;
    Vector timesN = N * times2;
//    std::cout << "timesN: " << timesN.X << ", " << timesN.Y << ", " << timesN.Z << std::endl;
    Vector R = I - timesN;
//    std::cout << "R: " << R.X << ", " << R.Y << ", " << R.Z << std::endl;

//    std::cout << "leaving  Reflect" << std::endl;
    if (R.length() == 0){
        return R;
    }
    return R.normalize();
}

void Material::test() const{
    std::cout << "Start Reflect Test" << std::endl;
    Vector debugDirection;
    Vector d = Vector(0.0, 0.0, 0.0);
    Vector n = Vector(0.0, 1.0, 0.0);
    debugDirection = reflect(d, n);
    std::cout << "expected 0.0, 0.0, 0.0 | calculated" << debugDirection.X << ", " << debugDirection.Y << ", " << debugDirection.Z << std::endl;


    d = Vector(0.707, -0.707, 0.0);
    n = Vector(0.0, 1.0, 0.0);
    debugDirection = reflect(d, n);
    std::cout << "expected 0.707, 0.707, 0.0 | calculated "<< debugDirection.X << ", " << debugDirection.Y << ", " << debugDirection.Z << std::endl;

    std::cout << "Start Schlick Test" << std::endl;
    // test schlick
    d = Vector(0.707, 0.707, 0.0);
    n = Vector(0.0, 1.0, 0.0);
    double n1 = 1.0;
    double n2 = 1.5;
    double rPhi = schlick(d, n, n1, n2);
    std::cout << "expected 13.9579 | calculated "<<rPhi<< std::endl;

    d = Vector(0.707, 0.707, 0.0);
    n = Vector(0.0, 1.0, 0.0);
    n1 = 1.5;
    n2 = 1.0;
    rPhi = schlick(d, n, n1, n2);
    std::cout << " expected 13.9579 | calculated "<<rPhi<< std::endl;

    d = Vector(0.995, -0.1, 0.0);
    n = Vector(0.0, 1.0, 0.0);
    n1 = 1.0;
    n2 = 1.5;
    rPhi = schlick(d, n, n1, n2);
    std::cout << " expected 0.608435 | calculated "<<rPhi<< std::endl;

    d = Vector(0.995, -0.1, 0.0);
    n = Vector(0.0, 1.0, 0.0);
    n1 = 1.5;
    n2 = 1.0;
    rPhi = schlick(d, n, n1, n2);
    std::cout << " expected 0.608435 | calculated "<<rPhi<< std::endl;

    std::cout << "Start Refract Test" << std::endl;
    // test refract
    d = Vector(0.707, 0.707, 0.0);
    n = Vector(0.0, 1.0, 0.0);
    n1 = 1.0;
    n2 = 1.5;
    debugDirection = refract(d, n, n1, n2);
    std::cout << " expected 0.471, -0.882, 0.0 | calculated "<< debugDirection.X << ", " << debugDirection.Y << ", " << debugDirection.Z << std::endl;

    d = Vector(0.707, 0.707, 0.0);
    n = Vector(0.0, 1.0, 0.0);
    n1 = 1.5;
    n2 = 1.0;
    debugDirection = refract(d, n, n1, n2);
    if (debugDirection.X == INT64_MAX) std::cout << "MAAAAX" << std::endl;
    std::cout << " expected null | calculated "<< debugDirection.X << ", " << debugDirection.Y << ", " << debugDirection.Z << std::endl;

    d = Vector(0.995, -0.1, 0.0);
    n = Vector(0.0, 1.0, 0.0);
    n1 = 1.0;
    n2 = 1.5;
    debugDirection = refract(d, n, n1, n2);
    std::cout << " expected 0.663, -0.748, 0.0 | calculated "<< debugDirection.X << ", " << debugDirection.Y << ", " << debugDirection.Z << std::endl;

    d = Vector(0.995, -0.1, 0.0);
    n = Vector(0.0, 1.0, 0.0);
    n1 = 1.5;
    n2 = 1.0;
    debugDirection = refract(d, n, n1, n2);
    if (debugDirection.X == INT64_MAX) std::cout << "MAAAAX" << std::endl;
    std::cout << " expected null | calculated "<< debugDirection.X << ", " << debugDirection.Y << ", " << debugDirection.Z << std::endl;

    d = Vector(0.1, -0.995, 0.0);
    n = Vector(0.0, 1.0, 0.0);
    n1 = 1.0;
    n2 = 1.5;
    debugDirection = refract(d, n, n1, n2);
    std::cout << " expected 0.066, -0.998, 0.0 | calculated "<< debugDirection.X << ", " << debugDirection.Y << ", " << debugDirection.Z << std::endl;

    d = Vector(0.1, -0.995, 0.0);
    n = Vector(0.0, 1.0, 0.0);
    n1 = 1.5;
    n2 = 1.0;
    debugDirection = refract(d, n, n1, n2);
    std::cout << " expected 0.149, -0.989, 0.0 | calculated "<< debugDirection.X << ", " << debugDirection.Y << ", " << debugDirection.Z << std::endl;
}


float Material::getN1()  const{
    return n1;
}
float Material::getN2()  const{
    return n1;
}
void Material::setN1( float &N1)  {
    this->n1 = N1;
}
void Material::setN2( float &N2)  {
    this->n2 = N2;
}




Triangle::Triangle()
{
    pMtrl = &Material::DefaultMaterial;
}

Triangle::Triangle(const Vector& a, const Vector& b, const Vector& c, const Material* mtrl)
{
    A=a; B=b; C=c; pMtrl = mtrl;
    NA = NB = NC = (B-A).cross(C-A).normalize();
}


Vector Triangle::calcNormal( const Vector& PointOnTriangle) const
{
    Vector BA = B-A;
    Vector CA = C-A;
    Vector CB = C-B;
    float AreaTriangle = fabs(BA.cross(CA).length() * 0.5f);
    float AreaA = fabs(CB.cross(PointOnTriangle-B).length() * 0.5f);
    float AreaB = fabs(CA.cross(PointOnTriangle-A).length() * 0.5f);
    float AreaC = fabs(BA.cross(PointOnTriangle-A).length() * 0.5f);

    Vector Normal = (NA * AreaA + NB * AreaB + NC * AreaC) * (1.0f/AreaTriangle);
    Normal.normalize();
    return Normal;
}


PointLight::PointLight()
{
    Position = Vector( -0.0,1.75f,-2.0f);
    Intensity = Color(1,1,1);
}

PointLight::PointLight( const Vector& pos, const Color& intensity)
{
    Position = pos;
    Intensity = intensity;
}



Scene::Scene( unsigned int SceneComplexity )
{
    
    std::vector<Triangle> InnerModelTriangles;

    createInnerModel(InnerModelTriangles, 1.2f, SceneComplexity>=3 ? SceneComplexity : 10, SceneComplexity>=3 ? SceneComplexity : 10);

    unsigned int NumModelTriangles = (SceneComplexity>=3)?(unsigned int)InnerModelTriangles.size():0;
    m_TriangleCount = 10 + NumModelTriangles;
    m_Triangles = new Triangle[m_TriangleCount];
    
    int i = 0;
    
    m_Triangles[i++] = Triangle( Vector(-2,-2,2), Vector(-2,2,2), Vector(2,-2,2), &Material::RedMtrl);
    m_Triangles[i++] = Triangle( Vector(-2,2,2), Vector(2,2,2), Vector(2,-2,2), &Material::RedMtrl);

    m_Triangles[i++] = Triangle( Vector(-2,-2,-2), Vector(-2,-2,2), Vector(2,-2,-2), &Material::YellowMtrl);
    m_Triangles[i++] = Triangle( Vector(2,-2,-2), Vector(-2,-2,2), Vector(2,-2,2),  &Material::YellowMtrl);

    m_Triangles[i++] = Triangle( Vector(-2,-2,-2), Vector(-2,2,-2), Vector(-2,-2,2), &Material::GreenMtrl);
    m_Triangles[i++] = Triangle( Vector(-2,-2,2), Vector(-2,2,-2), Vector(-2,2,2),  &Material::GreenMtrl);

    m_Triangles[i++] = Triangle( Vector(2,-2,2), Vector(2,2,-2), Vector(2,-2,-2), &Material::BlueMtrl);
    m_Triangles[i++] = Triangle( Vector(2,2,2), Vector(2,2,-2), Vector(2,-2,2),  &Material::BlueMtrl);

    m_Triangles[i++] = Triangle( Vector(2,2,-2), Vector(-2,2,2), Vector(-2,2,-2), &Material::CyanMtrl);
    m_Triangles[i++] = Triangle( Vector(2,2,2), Vector(-2,2,2), Vector(2,2,-2),  &Material::CyanMtrl);

    if(SceneComplexity>=3)
        for( std::vector<Triangle>::const_iterator it=InnerModelTriangles.begin(); it != InnerModelTriangles.end(); ++it)
            m_Triangles[i++] = (*it);
    
    
    m_Lights = new PointLight[2]
    {
        PointLight(Vector( -1.0,1.75f,-1.8f),Color(0.5f,0.5f,0.5f)),
        PointLight(Vector( 1.0,1.75f,-1.8f),Color(0.5f,0.5f,0.5f))
    };
    
    m_LightCount = 2;
}

void Scene::createInnerModel( std::vector<Triangle>& Triangles, float Radius, int Slices, int Stacks) const
{
    if(Slices*Stacks <= 0 )
        return;
    
    int width = Slices;
    int height = Stacks;
    
    float theta, phi;
    int i, j, t, ntri, nvec;
    
    nvec = (height-2)* width+2;
    ntri = (height-2)*(width-1)*2;
    
    int VertexCount = nvec;
    int IndexCount = ntri*3;
    Vector* pVertices = new Vector[VertexCount];
    assert(pVertices);
    int* pIndices = new int[IndexCount];
    assert(pIndices);
    
    // create vertices
    for( t=0, j=1; j<height-1; j++ )
        for( i=0; i<width; i++ )
        {
            theta = (float)j/(height-1) * (float)M_PI;
            phi   = (float)i/(width-1 ) * (float)M_PI*2.0f;
            pVertices[t  ].X =  sin(theta) * cos(phi) * Radius;
            pVertices[t  ].Y =  cos(theta) * Radius;
            pVertices[t++].Z = -sin(theta) * sin(phi) * Radius;
        }
    
    pVertices[t].X=0; pVertices[t].Y= Radius; pVertices[t].Z=0;
    t++;
    pVertices[t].X=0; pVertices[t].Y=-Radius; pVertices[t].Z=0;
    t++;
    assert(t==VertexCount);
    
    // create indices
    for( t=0, j=0; j<height-3; j++ )
        for(  i=0; i<width-1; i++ )
        {
            pIndices[t++] = (j  )*width + i  ;
            pIndices[t++] = (j+1)*width + i+1;
            pIndices[t++] = (j  )*width + i+1;
            pIndices[t++] = (j  )*width + i  ;
            pIndices[t++] = (j+1)*width + i  ;
            pIndices[t++] = (j+1)*width + i+1;
        }
    
    for( i=0; i<width-1; i++ )
    {
        pIndices[t++] = (height-2)*width;
        pIndices[t++] = i;
        pIndices[t++] = i+1;
        pIndices[t++] = (height-2)*width+1;
        pIndices[t++] = (height-3)*width + i+1;
        pIndices[t++] = (height-3)*width + i;
    }
    
    assert(t==IndexCount);
    
    for( int i=0; i<IndexCount/3; i++)
    {
        Triangle t;
        t.NA = t.A = pVertices[pIndices[i*3+2]];
        t.NB = t.B = pVertices[pIndices[i*3+1]];
        t.NC = t.C = pVertices[pIndices[i*3]];
        t.NA.normalize();
        t.NB.normalize();
        t.NC.normalize();
        t.pMtrl = &ProcMaterial::DefaultMaterial;
        Triangles.push_back(t);
    }
    
    delete [] pVertices;
    delete [] pIndices;
}


Scene::~Scene()
{
    if( m_Triangles)
    {
        delete [] m_Triangles;
        m_Triangles = NULL;
    }
}

const Triangle& Scene::getTriangle(unsigned int Index) const
{
    assert(Index<m_TriangleCount);
    return m_Triangles[Index];
}

unsigned int Scene::getTriangleCount() const
{
    return m_TriangleCount;
}

const PointLight& Scene::getLight(unsigned int Index) const
{
    assert(Index<m_LightCount);
    return m_Lights[Index];
}

unsigned int Scene::getLightCount() const
{
    return m_LightCount;
}

