#ifndef N_LINE_H
#define N_LINE_H
//-------------------------------------------------------------------
/**
    @class line2
    @ingroup Math  

    A 2- and 3-dimensional line objects.

    (C) 2004 RadonLabs GmbH
*/
#include "mathlib/vector.h"

//-------------------------------------------------------------------
class line2 
{
public:
    /// constructor
    line2();
    /// constructor #2
    line2(const vector2& v0, const vector2& v1);
    /// copy constructor
    line2(const line2& rhs);
    /// return start point
    const vector2& start() const;
    /// return end point
    vector2 end() const;
    /// return vector
    const vector2& vec() const;
    /// return length
    float len() const;
    /// get point on line given t
    vector2 ipol(const float t) const;

    vector2 b;
    vector2 m;
};

//-------------------------------------------------------------------
/**
*/
inline
line2::line2()
{
    // empty
}

//-------------------------------------------------------------------
/**
*/
inline
line2::line2(const vector2& v0, const vector2& v1) :
    b(v0),
    m(v1 - v0)
{
    // empty
}

//-------------------------------------------------------------------
/**
*/
inline
line2::line2(const line2& rhs) :
    b(rhs.b),
    m(rhs.m)
{
    // empty
}

//-------------------------------------------------------------------
/**
*/
inline
const vector2&
line2::start() const
{
    return this->b;
}

//-------------------------------------------------------------------
/**
*/
inline
vector2
line2::end() const
{
    return this->b + this->m;
}

//-------------------------------------------------------------------
/**
*/
inline
const vector2&
line2::vec() const
{
    return this->m;
}

//-------------------------------------------------------------------
/**
*/
inline
float
line2::len() const
{
    return m.len();
}

//-------------------------------------------------------------------
/**
*/
inline
vector2
line2::ipol(const float t) const
{
    return vector2(b + m * t);
}

//------------------------------------------------------------------------------
/**
    @class line3
    @ingroup Math
*/
class line3 
{
public:
    /// constructor
    line3();
    /// constructor #2
    line3(const vector3& v0, const vector3& v1);
    /// copy constructor
    line3(const line3& l);
    /// set start and end point
    void set(const vector3& v0, const vector3& v1);
    /// get start point
    const vector3& start() const;
    /// get end point
    vector3 end() const;
    /// get vector
    const vector3& vec() const;
    /// get length
    float len() const;
    /// get squared length
    float lensquared() const;
    /// minimal distance of point to line
    float distance(const vector3& p) const;
    /// get point on line at t
    vector3 ipol(float t) const;

    vector3 b;
    vector3 m;
};

//------------------------------------------------------------------------------
/**
*/
inline
line3::line3()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
line3::line3(const vector3& v0, const vector3& v1) :
    b(v0),
    m(v1 - v0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
line3::line3(const line3& rhs) :
    b(rhs.b),
    m(rhs.m)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
line3::set(const vector3& v0, const vector3& v1)
{
    this->b = v0;
    this->m = v1 - v0;
}

//------------------------------------------------------------------------------
/**
*/
inline
const vector3&
line3::start() const
{
    return this->b;
}

//------------------------------------------------------------------------------
/**
*/
inline
vector3
line3::end() const
{
    return this->b + this->m;
}

//------------------------------------------------------------------------------
/**
*/
inline
const vector3&
line3::vec() const
{
    return this->m;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
line3::len() const
{
    return this->m.len();
}

//------------------------------------------------------------------------------
/**
*/
inline
float
line3::lensquared() const
{
    return this->m.lensquared();
}

//------------------------------------------------------------------------------
/**
*/
inline
float
line3::distance(const vector3& p) const
{
    vector3 diff(p - this->b);
    float l = (this->m % this->m);
    if (l > 0.0f) 
    {
        float t = (this->m % diff) / l;
        diff = diff - this->m * t;
        return diff.len();
    } else {
        // line is really a point...
        vector3 v(p - this->b);
        return v.len();
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
vector3 
line3::ipol(const float t) const
{
    return vector3(b + m*t);
}

//------------------------------------------------------------------------------
#endif
