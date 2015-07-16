#ifndef N_POLAR_H
#define N_POLAR_H
//------------------------------------------------------------------------------
/**
    @class polar2
    @ingroup Math

    A polar coordinate inline class, consisting of 2 angles theta (latitude)
    and rho (longitude). Also offers conversion between cartesian and 
    polar space.

    Allowed range for theta is 0..180 degree (in rad!) and for rho 0..360 degree
    (in rad).

    (C) 2004 RadonLabs GmbH
*/
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "mathlib/vector.h"

//------------------------------------------------------------------------------
class polar2
{
public:
    /// the default constructor
    polar2();
    /// constructor, theta and rho args
    polar2(float t, float r);
    /// constructor, normalized cartesian vector as arg
    polar2(const vector3& v);
    /// the copy constructor
    polar2(const polar2& src);
    /// the assignment operator
    polar2& operator=(const polar2& rhs);
    /// convert to normalized cartesian coords 
    vector3 get_cartesian() const;
    /// get theta and rho in a 2d vec
    vector2 get() const;
    /// set to polar object
    void set(const polar2& p);
    /// set to theta and rho
    void set(const float t, const float r);
    /// set to cartesian 
    void set(const vector3&);
    /// fuzzy equality check 
    bool isequal(const polar2& rhs, float tol);

    float theta;
    float rho;

private:
    /// the equal operator is not allowed, use isequal() with tolerance!
    bool operator==(const polar2& /*rhs*/) { return false; }
};

//------------------------------------------------------------------------------
/**
*/
inline
polar2::polar2() :
    theta(0.0f),
    rho(0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
polar2::polar2(float t, float r) :
    theta(t),
    rho(r)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
polar2::polar2(const vector3& v)
{
    this->set(v);
}

//------------------------------------------------------------------------------
/**
*/
inline
polar2::polar2(const polar2& src) :
    theta(src.theta),
    rho(src.rho)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
polar2&
polar2::operator=(const polar2& rhs)
{
    this->theta = rhs.theta;
    this->rho = rhs.rho;
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
polar2::set(const polar2& p)
{
    this->theta = p.theta;
    this->rho = p.rho;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
polar2::set(const float t, const float r)
{
    this->theta = t;
    this->rho = r;
}

//------------------------------------------------------------------------------
/**
    Convert cartesian to polar.
*/
inline
void
polar2::set(const vector3& vec)
{
    double dTheta = acos(vec.y);

    // build a normalized 2d vector of the xz component
    vector2 v2(vec.x, vec.z);
    v2.norm();

    // adjust dRho based on the quadrant we are in
    double dRho;
    if ((v2.x >= 0.0f) && (v2.y >= 0.0f))
    {
        // quadrant 1
        dRho = asin(v2.x);
    }
    else if ((v2.x < 0.0f) && (v2.y >= 0.0f))
    {
        // quadrant 2 
        dRho = asin(v2.y) + n_deg2rad(270.0f);
    }
    else if ((v2.x < 0.0f) && (v2.y < 0.0f))
    {
        // quadrant 3
        dRho = asin(-v2.x) + n_deg2rad(180.0f);
    }
    else
    {
        // quadrant 4
        dRho = asin(-v2.y) + n_deg2rad(90.0f);
    }

    this->theta = (float) dTheta;
    this->rho   = (float) dRho;
}
    
//------------------------------------------------------------------------------
/**
    Convert polar to cartesian.
*/
inline
vector3
polar2::get_cartesian() const
{
    vector3 v;
    double sin_theta = sin(this->theta);
    double cos_theta = cos(this->theta);
    double sin_rho   = sin(this->rho);
    double cos_rho   = cos(this->rho);
    float x = (float) (sin_theta * sin_rho);
    float y = (float) cos_theta;
    float z = (float) (sin_theta * cos_rho);
    v.set(x,y,z);
    return v;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
polar2::isequal(const polar2& rhs, float tol)
{
    float dt = n_abs(rhs.theta - this->theta);
    float dr = n_abs(rhs.rho - this->rho);
    if (dt > tol)      return false;
    else if (dr > tol) return false;
    return true;
}

//------------------------------------------------------------------------------
#endif
