#ifndef _VECTOR3_H
#define _VECTOR3_H
//------------------------------------------------------------------------------
/**
    @class _vector3
    @ingroup Math

    Generic vector3 class. Uses 16 Byte of mem instead of 12 (!)

    (C) 2002 RadonLabs GmbH
*/
#include "mathlib/nmath.h"
#include <float.h>

//------------------------------------------------------------------------------
class _vector3
{
public:
    /// constructor 1
    _vector3();
    /// constructor 2
    _vector3(const float _x, const float _y, const float _z);
    /// constructor 3
    _vector3(const _vector3& vec);
    /// set elements 1
    void set(const float _x, const float _y, const float _z);
    /// set elements 2 
    void set(const _vector3& vec);
    /// return length
    float len() const;
    /// return length squared
    float lensquared() const;
    /// normalize
    void norm();
    /// inplace add
    void operator +=(const _vector3& v0);
    /// inplace sub
    void operator -=(const _vector3& v0);
    /// inplace scalar multiplication
    void operator *=(float s);
    /// true if any of the elements are greater
    bool operator >(const _vector3& rhs);
    /// true if any of the elements are smaller
    bool operator <(const _vector3& rhs);
    /// fuzzy compare, return true/false
    bool isequal(const _vector3& v, float tol) const;
    /// fuzzy compare, returns -1, 0, +1
    int compare(const _vector3& v, float tol) const;
    /// rotate around axis
    void rotate(const _vector3& axis, float angle);
    /// inplace linear interpolation
    void lerp(const _vector3& v0, float lerpVal);
    /// returns a vector orthogonal to self, not normalized
    _vector3 findortho() const;
    /// saturate components between 0 and 1
    void saturate();
    /// dot product
    float dot(const _vector3& v0) const;
    /// distance between 2 vector3's
    static float distance(const _vector3& v0, const _vector3& v1);

    float x, y, z;
};

//------------------------------------------------------------------------------
/**
*/
inline
_vector3::_vector3() :
    x(0.0f),
    y(0.0f),
    z(0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector3::_vector3(const float _x, const float _y, const float _z) :
    x(_x),
    y(_y),
    z(_z)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector3::_vector3(const _vector3& vec) :
    x(vec.x),
    y(vec.y),
    z(vec.z)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector3::set(const float _x, const float _y, const float _z)
{
    x = _x;
    y = _y;
    z = _z;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector3::set(const _vector3& vec)
{
    x = vec.x;
    y = vec.y;
    z = vec.z;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
_vector3::len() const
{
    return (float) n_sqrt(x * x + y * y + z * z);
}

//------------------------------------------------------------------------------
/**
*/
inline
float 
_vector3::lensquared() const
{
    return x * x + y * y + z * z;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector3::norm()
{
    float l = len();
    if (l > TINY) 
    {
        x /= l;
        y /= l;
        z /= l;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector3::operator +=(const _vector3& v0)
{
    x += v0.x;
    y += v0.y;
    z += v0.z;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector3::operator -=(const _vector3& v0)
{
    x -= v0.x;
    y -= v0.y;
    z -= v0.z;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector3::operator *=(float s)
{
    x *= s;
    y *= s;
    z *= s;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
_vector3::isequal(const _vector3& v, float tol) const
{
    if (fabs(v.x - x) > tol)      return false;
    else if (fabs(v.y - y) > tol) return false;
    else if (fabs(v.z - z) > tol) return false;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
_vector3::compare(const _vector3& v, float tol) const
{
    if (fabs(v.x - x) > tol)      return (v.x > x) ? +1 : -1; 
    else if (fabs(v.y - y) > tol) return (v.y > y) ? +1 : -1;
    else if (fabs(v.z - z) > tol) return (v.z > z) ? +1 : -1;
    else                          return 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector3::rotate(const _vector3& axis, float angle)
{
    // rotates this one around given vector. We do
    // rotation with matrices, but these aren't defined yet!
    float rotM[9];
    float sa, ca;

    sa = (float) sin(angle);
    ca = (float) cos(angle);

    // build a rotation matrix
    rotM[0] = ca + (1 - ca) * axis.x * axis.x;
    rotM[1] = (1 - ca) * axis.x * axis.y - sa * axis.z;
    rotM[2] = (1 - ca) * axis.z * axis.x + sa * axis.y;
    rotM[3] = (1 - ca) * axis.x * axis.y + sa * axis.z;
    rotM[4] = ca + (1 - ca) * axis.y * axis.y;
    rotM[5] = (1 - ca) * axis.y * axis.z - sa * axis.x;
    rotM[6] = (1 - ca) * axis.z * axis.x - sa * axis.y;
    rotM[7] = (1 - ca) * axis.y * axis.z + sa * axis.x;
    rotM[8] = ca + (1 - ca) * axis.z * axis.z;

    // "handmade" multiplication
    _vector3 help(rotM[0] * this->x + rotM[1] * this->y + rotM[2] * this->z,
                  rotM[3] * this->x + rotM[4] * this->y + rotM[5] * this->z,
                  rotM[6] * this->x + rotM[7] * this->y + rotM[8] * this->z);
    *this = help;
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector3 operator +(const _vector3& v0, const _vector3& v1) 
{
    return _vector3(v0.x + v1.x, v0.y + v1.y, v0.z + v1.z);
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector3 operator -(const _vector3& v0, const _vector3& v1) 
{
    return _vector3(v0.x - v1.x, v0.y - v1.y, v0.z - v1.z);
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector3 operator *(const _vector3& v0, const float s) 
{
    return _vector3(v0.x * s, v0.y * s, v0.z * s);
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector3 operator -(const _vector3& v) 
{
    return _vector3(-v.x, -v.y, -v.z);
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector3 operator /(const _vector3& v0, const float s)
{
    float one_over_s = 1.0f/s;
    return _vector3(v0.x*one_over_s, v0.y*one_over_s, v0.z*one_over_s);
}

//------------------------------------------------------------------------------
/**
    Dot product.
*/
static
inline
float operator %(const _vector3& v0, const _vector3& v1)
{
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

//------------------------------------------------------------------------------
/**
    Cross product.
*/
static 
inline 
_vector3 operator *(const _vector3& v0, const _vector3& v1) 
{
    return _vector3(v0.y * v1.z - v0.z * v1.y,
                    v0.z * v1.x - v0.x * v1.z,
                    v0.x * v1.y - v0.y * v1.x);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector3::lerp(const _vector3& v0, float lerpVal)
{
    x = v0.x + ((x - v0.x) * lerpVal);
    y = v0.y + ((y - v0.y) * lerpVal);
    z = v0.z + ((z - v0.z) * lerpVal);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector3::saturate()
{
    x = n_saturate(x);
    y = n_saturate(y);
    z = n_saturate(z);
}

//------------------------------------------------------------------------------
/**
    Find a vector that is orthogonal to self. Self should not be (0,0,0).
    Return value is not normalized.
*/
inline
_vector3
_vector3::findortho() const
{
    if (0.0 != x)
    {
        return _vector3((-y - z) / x, 1.0, 1.0);
    } else
    if (0.0 != y)
    {
        return _vector3(1.0, (-x - z) / y, 1.0);
    } else
    if (0.0 != z)
    {
        return _vector3(1.0, 1.0, (-x - y) / z);
    } else
    {
        return _vector3(0.0, 0.0, 0.0);
    }
}

//------------------------------------------------------------------------------
/**
    Dot product for vector3
*/
inline
float
_vector3::dot(const _vector3& v0) const
{
    return ( x * v0.x + y * v0.y + z * v0.z );
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
_vector3::operator >(const _vector3& rhs)
{
    if ((this->x > rhs.x) || (this->y > rhs.y) || (this->z > rhs.z))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
_vector3::operator <(const _vector3& rhs)
{
    if ((this->x < rhs.x) || (this->y < rhs.y) || (this->z < rhs.z))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
float
_vector3::distance(const _vector3& v0, const _vector3& v1)
{
    _vector3 v(v1 - v0);
    return (float) n_sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

//------------------------------------------------------------------------------
#endif
