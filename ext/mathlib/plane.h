#ifndef N_PLANE_H
#define N_PLANE_H
//------------------------------------------------------------------------------
/**
    @class plane
    @ingroup Math

    A plane in 3d space.

    (C) 2004 RadonLabs GmbH
*/
#include "mathlib/vector.h"
#include "mathlib/line.h"

//------------------------------------------------------------------------------
class plane 
{
public:
    /// default constructor
    plane();
    /// constructor 1
    plane(float A, float B, float C, float D);
    /// constructor 2
    plane(const plane& p);
    /// constructor 3
    plane(const vector3& v0, const vector3& v1, const vector3& v2);
    /// set contents
    void set(float A, float B, float C, float D);
    /// construct plane from 3 vectors
    void set(const vector3& v0, const vector3& v1, const vector3& v2);
    /// compute distance of point to plane
    float distance(const vector3& v) const;
    /// get plane normal
    vector3 normal() const;
    /// get intersecting t of line with one sided plane
    bool intersect(const line3& l, float& t) const;
    /// get plane/plane intersection
    bool intersect(const plane& p, line3& l) const;

    float a , b, c, d;
};

//------------------------------------------------------------------------------
/**
*/
inline
plane::plane() :
    a(0.0f),
    b(0.0f),
    c(0.0f),
    d(1.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
plane::plane(float A, float B, float C, float D) :
    a(A),
    b(B),
    c(C),
    d(D)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
plane::plane(const plane& rhs) :
    a(rhs.a),
    b(rhs.b),
    c(rhs.c),
    d(rhs.d)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
plane::set(float A, float B, float C, float D)
{
    this->a = A;
    this->b = B;
    this->c = C;
    this->d = D;
}

//------------------------------------------------------------------------------
/**
    Constructs a plane from 3 position vectors.
*/
inline
void
plane::set(const vector3& v0, const vector3& v1, const vector3& v2)
{
    vector3 cross((v2 - v0) * (v1 - v0));
    cross.norm();
    this->a = cross.x;
    this->b = cross.y;
    this->c = cross.z;
    this->d = -(a * v0.x + b * v0.y + c * v0.z);
}

//------------------------------------------------------------------------------
/**
*/
inline
plane::plane(const vector3& v0, const vector3& v1, const vector3& v2)
{
    this->set(v0, v1, v2);
}

//------------------------------------------------------------------------------
/**
    Computes the distance of a point to the plane. Return 0.0 if the 
    point is on the plane.
*/
inline
float
plane::distance(const vector3& v) const
{
    return this->a * v.x + this->b * v.y + this->c * v.z + this->d;
}

//------------------------------------------------------------------------------
/**
    Returns the plane normal.
*/
inline
vector3 
plane::normal() const
{
    return vector3(this->a, this->b, this->c);
}

//------------------------------------------------------------------------------
/**
    Get intersecting t of line with one-sided plane. Returns false
    if the line is parallel to the plane.
*/
inline
bool
plane::intersect(const line3& l, float& t) const
{
    float f0 = this->a * l.b.x + this->b * l.b.y + this->c * l.b.z + this->d;
    float f1 = this->a * -l.m.x + this->b * -l.m.y + this->c * -l.m.z;
    if ((f1 < -0.0001f) || (f1 > 0.0001f))
    {
        t = f0 / f1;
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Get plane/plane intersection. Return false if planes are parallel.
*/
inline
bool
plane::intersect(const plane& p, line3& l) const
{
    vector3 n0 = this->normal();
    vector3 n1 = p.normal();
    float n00 = n0 % n0;
    float n01 = n0 % n1;
    float n11 = n1 % n1;
    float det = n00 * n11 - n01 * n01;
    const float tol = 1e-06f;
    if (fabs(det) < tol) 
    {
        return false;
    }
    else 
    {
        float inv_det = 1.0f/det;
        float c0 = (n11 * this->d - n01 * p.d)    * inv_det;
        float c1 = (n00 * p.d - n01 * this->d)* inv_det;
        l.m = n0 * n1;
        l.b = n0 * c0 + n1 * c1;
        return true;
    }
}

//------------------------------------------------------------------------------
#endif
