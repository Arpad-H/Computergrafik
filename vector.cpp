#include "vector.h"
#include <assert.h>
#include <math.h>

//using namespace Vector;

Vector::Vector(float x, float y, float z) {
    X = x;
    Y = y;
    Z = z;
}

Vector::Vector() {
    // nichts?
}

//               const Vector& = cannot modify Vector v
//                                 const = cant change class member variables || unless variable marked mutable
float Vector::dot(const Vector &v) const //pass by refrence?
{
    // TODO: add your code
    return v.X * X + v.Y * Y + v.Z * Z;
}

Vector Vector::cross(const Vector &v) const {
    Vector result = Vector((Y * v.Z) - (Z * v.Y),
                           (Z * v.X) - (X * v.Z),
                           (X * v.Y) - (Y * v.X));
    return result;
}


Vector Vector::operator+(const Vector &v) const {

    return Vector(X + v.X,
                  Y + v.Y,
                  Z + v.Z);
}

Vector Vector::operator-(const Vector &v) const {

    return Vector(X - v.X,
                  Y - v.Y,
                  Z - v.Z);
}

Vector Vector::operator*(float c) const {

    return Vector(X * c,
                  Y * c,
                  Z * c);
}

Vector Vector::operator-() const {
    // TODO: look at your code
    return Vector(-X,
                  -Y,
                  -Z); //is damit die inverse gemeint?
}

Vector &Vector::operator+=(const Vector &v) {
    // TODO: Read up on this crap, the refrencing that is
    this->X += v.X;
    this->Y += v.Y;
    this->Z += v.Z;
    return *this;
}


Vector &Vector::normalize() {
    float length = this->length();
    this->X /= length;
    this->Y /= length;
    this->Z /= length;
    return *this;
}

float Vector::length() const {

    return sqrt(lengthSquared());
}

float Vector::lengthSquared() const {

    return X * X + Y * Y + Z * Z;
}

Vector Vector::reflection(const Vector &normal) const {
    //TODO read up on this mess
    float dotProduct = dot(normal);
    Vector v = Vector(2 * dotProduct * normal.X,
                      2 * dotProduct * normal.Y,
                      2 * dotProduct * normal.Z);
    return *this - v;
}

bool Vector::triangleIntersection(const Vector &d, const Vector &a, const Vector &b, const Vector &c, float &s) const {
    // TODO: add your code
    return false; // dummy (remove)
}
