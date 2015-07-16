#ifndef _VECTOR2_H
#define _VECTOR2_H
//------------------------------------------------------------------------------
/**
    @class _vector2
    @ingroup Math

    Generic vector2 class.

    (C) 2002 RadonLabs GmbH
*/
#include "mathlib/nmath.h"
#include <float.h>

//------------------------------------------------------------------------------
class _vector2 {
public:
    /// constructor 1
    _vector2();
    /// constructor 2
    _vector2(const float _x, const float _y);
    /// constructor 3
    _vector2(const _vector2& vec);
    /// constructor 4
    _vector2(const float* p);
    /// set elements 1
    void set(const float _x, const float _y);
    /// set elements 2 
    void set(const _vector2& vec);
    /// set elements 3
    void set(const float* p);
    /// return length
    float len() const;
    /// normalize
    void norm();
    /// in place add
    void operator+=(const _vector2& v0);
    /// in place sub
    void operator-=(const _vector2& v0);
    /// in place scalar mul
    void operator*=(const float s);
    /// in place scalar div
    void operator/=(const float s);
    /// fuzzy compare operator
    bool isequal(const _vector2& v, const float tol) const;
    /// fuzzy compare, returns -1, 0, +1
    int compare(const _vector2& v, float tol) const;
    /// rotate around P(0,0)
    void rotate(float angle);

    float x, y;
};

//------------------------------------------------------------------------------
/**
*/
inline
_vector2::_vector2() :
    x(0.0f),
    y(0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector2::_vector2(const float _x, const float _y) :
    x(_x),
    y(_y)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector2::_vector2(const _vector2& vec) :
    x(vec.x),
    y(vec.y)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector2::_vector2(const float* p) :
    x(p[0]),
    y(p[1])
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector2::set(const float _x, const float _y)
{
    x = _x;
    y = _y;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector2::set(const _vector2& v)
{
    x = v.x;
    y = v.y;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector2::set(const float* p)
{
    x = p[0];
    y = p[1];
}

//------------------------------------------------------------------------------
/**
*/
inline
float
_vector2::len() const
{
    return (float) sqrt(x * x + y * y);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector2::norm()
{
    float l = len();
    if (l > TINY)
    {
        x /= l;
        y /= l;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector2::operator +=(const _vector2& v0) 
{
    x += v0.x;
    y += v0.y;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector2::operator -=(const _vector2& v0) 
{
    x -= v0.x;
    y -= v0.y;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector2::operator *=(const float s) 
{
    x *= s;
    y *= s;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector2::operator /=(const float s) 
{
    x /= s;
    y /= s;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool 
_vector2::isequal(const _vector2& v, const float tol) const
{
    if (fabs(v.x - x) > tol)      return false;
    else if (fabs(v.y - y) > tol) return false;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
_vector2::compare(const _vector2& v, float tol) const
{
    if (fabs(v.x - x) > tol)      return (v.x > x) ? +1 : -1; 
    else if (fabs(v.y - y) > tol) return (v.y > y) ? +1 : -1;
    else                          return 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector2::rotate(float angle)
{
    // rotates this one around P(0,0).
    float sa, ca;

    sa = (float) sin(angle);
    ca = (float) cos(angle);

    // "handmade" multiplication
    _vector2 help(ca * this->x - sa * this->y,
                  sa * this->x + ca * this->y);  

    *this = help;
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector2 operator +(const _vector2& v0, const _vector2& v1) 
{
    return _vector2(v0.x + v1.x, v0.y + v1.y); 
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector2 operator -(const _vector2& v0, const _vector2& v1) 
{
    return _vector2(v0.x - v1.x, v0.y - v1.y);
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector2 operator *(const _vector2& v0, const float s) 
{
    return _vector2(v0.x * s, v0.y * s);
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector2 operator -(const _vector2& v) 
{
    return _vector2(-v.x, -v.y);
}

//------------------------------------------------------------------------------
#endif

