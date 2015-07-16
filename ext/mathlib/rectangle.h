#ifndef N_RECTANGLE_H
#define N_RECTANGLE_H
//------------------------------------------------------------------------------
/**
    @class rectangle
    @ingroup Math

    A 2d rectangle class.
    
    (C) 2003 RadonLabs GmbH
*/
#include "mathlib/vector.h"

//------------------------------------------------------------------------------
class rectangle
{
public:
    /// default constructor
    rectangle();
    /// constructor 1
    rectangle(const vector2& topLeft, const vector2& bottomRight);
    /// set content
    void set(const vector2& topLeft, const vector2& bottomRight);
    /// return true if point is inside
    bool inside(const vector2& p) const;
    /// return midpoint
    vector2 midpoint() const;
    /// return width
    float width() const;
    /// return height
    float height() const;
    /// return size
    vector2 size() const;

    vector2 v0;
    vector2 v1;
};

//------------------------------------------------------------------------------
/**
*/
inline
rectangle::rectangle()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
rectangle::rectangle(const vector2& topLeft, const vector2& bottomRight) :
    v0(topLeft),
    v1(bottomRight)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
rectangle::set(const vector2& topLeft, const vector2& bottomRight)
{
    this->v0 = topLeft;
    this->v1 = bottomRight;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
rectangle::inside(const vector2& p) const
{
    return ((this->v0.x <= p.x) && (p.x <= this->v1.x) &&
            (this->v0.y <= p.y) && (p.y <= this->v1.y));
}

//------------------------------------------------------------------------------
/**
*/
inline
vector2
rectangle::midpoint() const
{
    return (this->v0 + this->v1) * 0.5f;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
rectangle::width() const
{
    return this->v1.x - this->v0.x;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
rectangle::height() const
{
    return this->v1.y - this->v0.y;
}

//------------------------------------------------------------------------------
/**
*/
inline
vector2
rectangle::size() const
{
    return this->v1 - this->v0;
}

//------------------------------------------------------------------------------
#endif



