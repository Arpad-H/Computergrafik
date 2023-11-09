//
//  CGUtilities.h
//  SimpleRayTracer
//
//  Created by Philipp Lensing on 03.09.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifndef SimpleRayTracer_CGUtilities_h
#define SimpleRayTracer_CGUtilities_h
#include <vector>
#include "CG_Helpers/color.h"
#include "CG_Helpers/vector.h"


class Material
{
public:
    Material();
    Material( const Color& Diffuse, const Color& Specular, const Color& Ambient, float SpecularExp,  float Reflectivity);
    virtual Color getDiffuseCoeff(const Vector& Pos) const;
    virtual Color getSpecularCoeff(const Vector& Pos) const;
    virtual Color getAmbientCoeff(const Vector& Pos) const;
    virtual float getSpecularExp(const Vector& Pos) const;
    virtual float getReflectivity(const Vector& Pos) const;


    virtual float getN1()const ;
    virtual float getN2() const;
    virtual void setN2(float &N2);
    virtual void setN1(float &N1);


    virtual float schlick(const Vector &Pos, const Vector& Normal, const float &N1, const float &N2) const ;
    virtual float getTransmissionCoeff(const Vector& Pos, const Vector& Normal) const;
    virtual Vector refract (const Vector& Ray_in, const Vector& Normal, float n1, float n2)const;
    Vector reflect(const Vector &I, const Vector &N) const;
    static Material DefaultMaterial;
    static Material RedMtrl;
    static Material GreenMtrl;
    static Material BlueMtrl;
    static Material YellowMtrl;
    static Material CyanMtrl;
    static Material GlasMtrl;

     float n2;
     float n1;

    virtual void test() const;

protected:
    Color m_DiffuseCoeff;
    Color m_SpecularCoeff;
    Color m_AmbientCoeff;
    float m_SpecularExp;

    float m_Reflectivity;


};


class Triangle
{
public:
    Triangle();
    Triangle(const Vector& a, const Vector& b, const Vector& c, const Material* mtrl);
    Vector A,B,C; // vertex-positions
    const Material* pMtrl; // pointer to triangle material
    Vector calcNormal( const Vector& PointOnTriangle) const;
protected:
    friend class Scene;
    Vector NA, NB, NC; // vertex-normals

};

class PointLight
{
public:
    PointLight();
    PointLight( const Vector& pos, const Color& intensity);
    Vector Position;
    Color Intensity;
};

class Scene
{
public:
    Scene( unsigned int SceneComplexity ); // Set SceneComplexity to zero for a scene with only 10 triangles, for a more complex scene set Scenecompleyity to 20.
    virtual ~Scene();
    const Triangle& getTriangle(unsigned int Index) const;
    unsigned int getTriangleCount() const;
    const PointLight& getLight(unsigned int Index) const;
    unsigned int getLightCount() const;
    
protected:
    Triangle* m_Triangles;
    unsigned int m_TriangleCount;
    PointLight* m_Lights;
    unsigned int m_LightCount;
    void createInnerModel( std::vector<Triangle>& Triangles, float Radius, int Slices, int Stacks) const;

};




#endif
