#ifndef _VECTOR4_H
#define _VECTOR4_H
//------------------------------------------------------------------------------
/**
    @class _vector4
    @ingroup Math

    A generic vector4 class.

    (C) 2002 RadonLabs GmbH
*/
#include "mathlib/nmath.h"
#include <float.h>

//------------------------------------------------------------------------------
class _vector4 
{
public:
    enum component
    {
        X = (1<<0),
        Y = (1<<1),
        Z = (1<<2),
        W = (1<<3),
    };

    /// constructor 1
    _vector4();
    /// constructor 2
    _vector4(const float _x, const float _y, const float _z, const float _w);
    /// constructor 3
    _vector4(const _vector4& vec);
    /// constructor from vector3 (w will be set to 1.0)
    _vector4(const _vector3& vec3);
    /// set elements 1
    void set(const float _x, const float _y, const float _z, const float _w);
    /// set elements 2 
    void set(const _vector4& v);
    /// set to vector3 (w will be set to 1.0)
    void set(const _vector3& v);
    /// return length
    float len() const;
    /// normalize
    void norm();
    /// inplace add
    void operator +=(const _vector4& v);
    /// inplace sub
    void operator -=(const _vector4& v);
    /// inplace scalar mul
    void operator *=(const float s);
    /// vector3 assignment operator (w set to 1.0f)
    _vector4& operator=(const _vector3& v);
    /// fuzzy compare
    bool isequal(const _vector4& v, float tol) const;
    /// fuzzy compare, return -1, 0, +1
    int compare(const _vector4& v, float tol) const;
    /// set own components to minimum
    void minimum(const _vector4& v);
    /// set own components to maximum
    void maximum(const _vector4& v);
    /// set component float value by mask
    void setcomp(float val, int mask);
    /// get component float value by mask
    float getcomp(int mask);
    /// get write mask for smallest component
    int mincompmask() const;
    /// inplace linear interpolation
    void lerp(const _vector4& v0, float lerpVal);
    /// saturate components between 0 and 1
    void saturate();
    /// dot product
    float dot(const _vector4& v0) const;

    float x, y, z, w;
};

//------------------------------------------------------------------------------
/**
*/
inline
_vector4::_vector4() :
    x(0.0f),
    y(0.0f),
    z(0.0f),
    w(0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector4::_vector4(const float _x, const float _y, const float _z, const float _w) :
    x(_x),
    y(_y),
    z(_z),
    w(_w)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector4::_vector4(const _vector4& v) :
    x(v.x),
    y(v.y),
    z(v.z),
    w(v.w)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector4::_vector4(const _vector3& v) :
    x(v.x),
    y(v.y),
    z(v.z),
    w(1.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4::set(const float _x, const float _y, const float _z, const float _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4::set(const _vector4& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4::set(const _vector3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = 1.0f;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
_vector4::len() const
{
    return (float) sqrt(x * x + y * y + z * z + w * w);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4::norm()
{
    float l = len();
    if (l > TINY) 
    {
        float oneDivL = 1.0f / l;
        x *= oneDivL;
        y *= oneDivL;
        z *= oneDivL;
        w *= oneDivL;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4::operator +=(const _vector4& v)
{
    x += v.x; 
    y += v.y; 
    z += v.z; 
    w += v.w;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4::operator -=(const _vector4& v)
{
    x -= v.x; 
    y -= v.y; 
    z -= v.z; 
    w -= v.w;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4::operator *=(const float s)
{
    x *= s; 
    y *= s; 
    z *= s; 
    w *= s;
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector4&
_vector4::operator=(const _vector3& v)
{
    this->set(v);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
_vector4::isequal(const _vector4& v, float tol) const
{
    if (fabs(v.x - x) > tol)      return false;
    else if (fabs(v.y - y) > tol) return false;
    else if (fabs(v.z - z) > tol) return false;
    else if (fabs(v.w - w) > tol) return false;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
_vector4::compare(const _vector4& v, float tol) const
{
    if (fabs(v.x - x) > tol)      return (v.x > x) ? +1 : -1; 
    else if (fabs(v.y - y) > tol) return (v.y > y) ? +1 : -1;
    else if (fabs(v.z - z) > tol) return (v.z > z) ? +1 : -1;
    else if (fabs(v.w - w) > tol) return (v.w > w) ? +1 : -1;
    else                          return 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4::minimum(const _vector4& v)
{
    if (v.x < x) x = v.x;
    if (v.y < y) y = v.y;
    if (v.z < z) z = v.z;
    if (v.w < w) w = v.w;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4::maximum(const _vector4& v)
{
    if (v.x > x) x = v.x;
    if (v.y > y) y = v.y;
    if (v.z > z) z = v.z;
    if (v.w > w) w = v.w;
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector4 operator +(const _vector4& v0, const _vector4& v1) 
{
    return _vector4(v0.x + v1.x, v0.y + v1.y, v0.z + v1.z, v0.w + v1.w);
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector4 operator -(const _vector4& v0, const _vector4& v1) 
{
    return _vector4(v0.x - v1.x, v0.y - v1.y, v0.z - v1.z, v0.w - v1.w);
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector4 operator *(const _vector4& v0, const float& s) 
{
    return _vector4(v0.x * s, v0.y * s, v0.z * s, v0.w * s);
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector4 operator -(const _vector4& v)
{
    return _vector4(-v.x, -v.y, -v.z, -v.w);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4::setcomp(float val, int mask)
{
    if (mask & X) x = val;
    if (mask & Y) y = val;
    if (mask & Z) z = val;
    if (mask & W) w = val;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
_vector4::getcomp(int mask)
{
    switch (mask)
    {
        case X:  return x;
        case Y:  return y;
        case Z:  return z;
        default: return w;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
int
_vector4::mincompmask() const
{
    float minVal = x;
    int minComp = X;
    if (y < minVal)
    {
        minComp = Y;
        minVal  = y;
    }
    if (z < minVal) 
    {
        minComp = Z;
        minVal  = z;
    }
    if (w < minVal) 
    {
        minComp = W;
        minVal  = w;
    }
    return minComp;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4::lerp(const _vector4& v0, float lerpVal)
{
    x = v0.x + ((x - v0.x) * lerpVal);
    y = v0.y + ((y - v0.y) * lerpVal);
    z = v0.z + ((z - v0.z) * lerpVal);
    w = v0.w + ((w - v0.w) * lerpVal);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4::saturate()
{
    x = n_saturate(x);
    y = n_saturate(y);
    z = n_saturate(z);
    w = n_saturate(w);
}


//------------------------------------------------------------------------------
/**
    Dot product for vector4
*/
inline
float _vector4::dot(const _vector4& v0) const
{
    return ( x * v0.x + y * v0.y + z * v0.z + w * v0.w );
}

//------------------------------------------------------------------------------
#endif

