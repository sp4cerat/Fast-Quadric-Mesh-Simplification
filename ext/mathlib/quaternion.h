#ifndef N_QUATERNION_H
#define N_QUATERNION_H
//-------------------------------------------------------------------
/**
    @class quaternion
    @ingroup Math

    quaternion class with some basic operators.

     - 08-Dec-00   floh    extended, ATTENTION: argument ordering in
                           constructor has changed!

    (C) 2004 RadonLabs GmbH
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mathlib/vector.h"

//-------------------------------------------------------------------
//  quaternion
//-------------------------------------------------------------------
class quaternion {
public:
    float x,y,z,w;

    //-- constructors -----------------------------------------------
    quaternion()
        : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
    {};
    quaternion(float _x, float _y, float _z, float _w)
        : x(_x), y(_y), z(_z), w(_w)
    {};
    quaternion(const quaternion& q)
        : x(q.x), y(q.y), z(q.z), w(q.w)
    {};

    //-- setting elements -------------------------------------------
    void set(float _x, float _y, float _z, float _w) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    };
    void set(const quaternion& q) {
        x = q.x;
        y = q.y;
        z = q.z;
        w = q.w;
    };

    //-- misc operations --------------------------------------------
    void ident(void) {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 1.0f;
    };

    void conjugate(void) {
        x = -x;
        y = -y;
        z = -z;
    };

    void scale(float s) {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
    };

    float norm(void) {
        return x*x + y*y + z*z + w*w;
    };

    float magnitude(void) {
        float n = norm();
        if (n > 0.0f) return n_sqrt(n);
        else          return 0.0f;
    };

    void invert(void) {
        float n = norm();
        if (n > 0.0f) scale(1.0f / norm());
        conjugate();
    };

    void normalize(void) {
        float l = magnitude();
        if (l > 0.0f) scale(1.0f / l);
        else          set(0.0f,0.0f,0.0f,1.0f);
    };

    //-- operators --------------------------------------------------
    bool operator==(const quaternion& q) {
        return ((x==q.x) && (y==q.y) && (z==q.z) && (w==q.w)) ? true : false;
    };

    bool operator!=(const quaternion& q) {
        return ((x!=q.x) || (y!=q.y) || (z!=q.z) || (w!=q.w)) ? true : false;
    };

    const quaternion& operator+=(const quaternion& q) {
        x += q.x;
        y += q.y;
        z += q.z;
        w += q.w;
        return *this;
    };

    const quaternion& operator-=(const quaternion& q) {
        x -= q.x;
        y -= q.y;
        z -= q.z;
        w -= q.w;
        return *this;
    };

    const quaternion& operator*=(const quaternion& q) {
        float qx = w*q.x + x*q.w + y*q.z - z*q.y;
        float qy = w*q.y + y*q.w + z*q.x - x*q.z;
        float qz = w*q.z + z*q.w + x*q.y - y*q.x;
        float qw = w*q.w - x*q.x - y*q.y - z*q.z;
        x = qx;
        y = qy;
        z = qz;
        w = qw;
        return *this;
    };

    /// rotate vector by quaternion
    vector3 rotate(const vector3& v) {
        quaternion q(v.x * w + v.z * y - v.y * z,
                     v.y * w + v.x * z - v.z * x,
                     v.z * w + v.y * x - v.x * y,
                     v.x * x + v.y * y + v.z * z);

        return vector3(w * q.x + x * q.w + y * q.z - z * q.y,
                       w * q.y + y * q.w + z * q.x - x * q.z,
                       w * q.z + z * q.w + x * q.y - y * q.x);
    };

    /**
        Create a rotation from one vector to an other. Works only with unit vectors.
        
        See http://www.martinb.com/maths/algebra/vectors/angleBetween/index.htm for 
        more information.
        
        @param from source unit vector 
        @param to destination unit vector
    */
    void set_from_axes( const vector3& from, const vector3& to )
    {
        vector3 c(from * to);
        set(c.x, c.y, c.z, from % to);
        w += 1.0f;      // reducing angle to halfangle
        if( w <= TINY ) // angle close to PI
        {
            if ((from.z * from.z) > (from.x * from.x))
                set(0, from.z, -from.y, w);
                //from*vector3(1,0,0) 
            else 
                set(from.y, -from.x, 0, w);
                //from*vector3(0,0,1) 
        }
        normalize(); 
    }

    /**
        Create a rotation from one vector to an other. Works with non unit vectors.
        
        See http://www.martinb.com/maths/algebra/vectors/angleBetween/index.htm for 
        more information.
        
        @param from source vector 
        @param to destination vector
    */
    void set_from_axes2( const vector3& from, const vector3& to )
    {
        vector3 c(from * to);
        set(c.x, c.y, c.z, from % to);
        normalize();    // if "from" or "to" not unit, normalize quat
        w += 1.0f;      // reducing angle to halfangle
        if( w <= TINY ) // angle close to PI
        {
            if ((from.z * from.z) > (from.x * from.x))
                set(0, from.z, -from.y, w);
                //from*vector3(1,0,0) 
            else 
                set(from.y, -from.x, 0, w);
                //from*vector3(0,0,1) 
        }
        normalize(); 
    }

    //-- convert from euler angles ----------------------------------
    void set_rotate_axis_angle(const vector3& v, float a) {
        float sin_a = n_sin(a * 0.5f);
        float cos_a = n_cos(a * 0.5f);
        x = v.x * sin_a;
        y = v.y * sin_a;
        z = v.z * sin_a;
        w = cos_a;
    };

    void set_rotate_x(float a) {
        float sin_a = n_sin(a * 0.5f);
        float cos_a = n_cos(a * 0.5f);
        x = sin_a;
        y = 0.0f;
        z = 0.0f;
        w = cos_a;
    };

    void set_rotate_y(float a) {
        float sin_a = n_sin(a * 0.5f);
        float cos_a = n_cos(a * 0.5f);
        x = 0.0f;
        y = sin_a;
        z = 0.0f;
        w = cos_a;
    };

    void set_rotate_z(float a) {
        float sin_a = n_sin(a * 0.5f);
        float cos_a = n_cos(a * 0.5f);
        x = 0.0f;
        y = 0.0f;
        z = sin_a;
        w = cos_a;
    };

    void set_rotate_xyz(float ax, float ay, float az) {
        quaternion qx, qy, qz;
        qx.set_rotate_x(ax);
        qy.set_rotate_y(ay);
        qz.set_rotate_z(az);
        *this = qx;
        *this *= qy;
        *this *= qz;
    };

	//--- fuzzy compare operators -----------------------------------
    bool isequal(const quaternion& v, float tol) const
    {
        if (fabs(v.x-x) > tol)      return false;
        else if (fabs(v.y-y) > tol) return false;
        else if (fabs(v.z-z) > tol) return false;
        else if (fabs(v.w-w) > tol) return false;
        return true;
    };

    //-- rotation interpolation, set this matrix to the -------------
    //-- interpolated result of q0->q1 with l as interpolator -------
    void slerp(const quaternion& q0, const quaternion& q1, float l) 
    {
        float fScale1;
        float fScale2;
        quaternion A = q0;
        quaternion B = q1;

        // compute dot product, aka cos(theta):
        float fCosTheta = A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;

        if (fCosTheta < 0.0f) 
        {
            // flip start quaternion
           A.x = -A.x; A.y = -A.y; A.z = -A.z; A.w = -A.w;
           fCosTheta = -fCosTheta;
        }

        if ((fCosTheta + 1.0f) > 0.05f) 
        {
            // If the quaternions are close, use linear interploation
            if ((1.0f - fCosTheta) < 0.05f) 
            {
                fScale1 = 1.0f - l;
                fScale2 = l;
            }
            else 
            { 
                // Otherwise, do spherical interpolation
                float fTheta    = n_acos(fCosTheta);
                float fSinTheta = n_sin(fTheta);
                fScale1 = n_sin( fTheta * (1.0f-l) ) / fSinTheta;
                fScale2 = n_sin( fTheta * l ) / fSinTheta;
            }
        }
        else 
        {
            B.x = -A.y;
            B.y =  A.x;
            B.z = -A.w;
            B.w =  A.z;
            fScale1 = n_sin( PI * (0.5f - l) );
            fScale2 = n_sin( PI * l );
        }

        x = fScale1 * A.x + fScale2 * B.x;
        y = fScale1 * A.y + fScale2 * B.y;
        z = fScale1 * A.z + fScale2 * B.z;
        w = fScale1 * A.w + fScale2 * B.w;
    };
};

//--- global operators ----------------------------------------------
static inline quaternion operator+(const quaternion& q0, const quaternion& q1) {
    return quaternion(q0.x+q1.x, q0.y+q1.y, q0.z+q1.z, q0.w+q1.w);
};

static inline quaternion operator-(const quaternion& q0, const quaternion& q1) {
    return quaternion(q0.x-q1.x, q0.y-q1.y, q0.z-q1.z, q0.w-q1.w);
};

static inline quaternion operator*(const quaternion& q0, const quaternion& q1) {
    return quaternion(q0.w*q1.x + q0.x*q1.w + q0.y*q1.z - q0.z*q1.y,
                      q0.w*q1.y + q0.y*q1.w + q0.z*q1.x - q0.x*q1.z,
                      q0.w*q1.z + q0.z*q1.w + q0.x*q1.y - q0.y*q1.x,
                      q0.w*q1.w - q0.x*q1.x - q0.y*q1.y - q0.z*q1.z);
};
//-------------------------------------------------------------------
#endif
