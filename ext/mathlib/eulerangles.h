#ifndef N_EULERANGLES_H
#define N_EULERANGLES_H
//------------------------------------------------------------------------------
/**
    @class nEulerAngles
    @ingroup Math

    A class representing a rotation using 3 euler angles.

    (C) 2004 RadonLabs GmbH
*/
#include <stdlib.h>
#include <math.h>
#include "mathlib/matrix.h"
#include "mathlib/euler.h"

//-------------------------------------------------------------------
/**
    @class nEulerAngles
    @ingroup NebulaMathDataTypes
*/
//-------------------------------------------------------------------
class nEulerAngles {
public:
    float x,y,z;

    //-- constructors -----------------------------------------------
    nEulerAngles()
        : x(0.0f),
          y(0.0f),
          z(0.0f)
    {};
    nEulerAngles(float _x, float _y, float _z)
        : x(_x),
          y(_y),
          z(_z)
    {};
    nEulerAngles(const nEulerAngles& e)
        : x(e.x),
          y(e.y),
          z(e.z)
    {};
    nEulerAngles(const matrix33& m)
    {
        Set(m);
    }

    //-- setting elements -------------------------------------------
    void Set(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    };
    void Set(const nEulerAngles& e)
    {
        x = e.x;
        y = e.y;
        z = e.z;
    };
    void Set(const matrix33& m)
    {
        int i,j,k,h,n,s,f;
        EulGetOrd(EulOrdXYZs,i,j,k,h,n,s,f);
        if (s == EulRepYes)
        {
            double sy = (float) sqrt(m.m[0][1]*m.m[0][1] + m.m[0][2]*m.m[0][2]);
            if (sy > 16*FLT_EPSILON)
            {
                this->x = (float) atan2(m.m[0][1], m.m[0][2]);
                this->y = (float) atan2(sy, m.m[0][0]);
                this->z = (float) atan2(m.m[1][0], -m.m[2][0]);
            }
            else
            {
                this->x = (float) atan2(-m.m[1][2], m.m[1][1]);
                this->y = (float) atan2(sy, m.m[0][0]);
                this->z = 0;
            }
        }
        else
        {
            double cy = sqrt(m.m[0][0]*m.m[0][0] + m.m[1][0]*m.m[1][0]);
            if (cy > 16*FLT_EPSILON)
            {
                this->x = (float) atan2(m.m[2][1], m.m[2][2]);
                this->y = (float) atan2(-m.m[2][0], cy);
                this->z = (float) atan2(m.m[1][0], m.m[0][0]);
            }
            else
            {
                this->x = (float) atan2(-m.m[1][2], m.m[1][1]);
                this->y = (float) atan2(-m.m[2][0], cy);
                this->z = 0;
            }
        }
        if (n==EulParOdd)
        {
            this->x = -this->x;
            this->y = -this->y;
            this->z = -this->z;
        }
        if (f==EulFrmR)
        {
            float t = this->x;
            this->x = this->z;
            this->z = t;
        }
    }

    matrix33 GetMatrix()
    {
        matrix33 mat;

        double ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
        int i,j,k,h,n,s,f;
        EulGetOrd(EulOrdXYZs,i,j,k,h,n,s,f);
        if (f==EulFrmR) {float t = x; x = z; z = t;}
        if (n==EulParOdd) {x = -x; y = -y; z = -z;}
        ti = x;   tj = y;   th = z;
        ci = cos(ti); cj = cos(tj); ch = cos(th);
        si = sin(ti); sj = sin(tj); sh = sin(th);
        cc = ci*ch; cs = ci*sh; sc = si*ch; ss = si*sh;
        if (s==EulRepYes) {
            mat.M11 = (float)(cj);     mat.M12 = (float)( sj*si);    mat.M13 = (float)( sj*ci);
            mat.M21 = (float)(sj*sh);  mat.M22 = (float)(-cj*ss+cc); mat.M23 = (float)(-cj*cs-sc);
            mat.M31 = (float)(-sj*ch); mat.M23 = (float)( cj*sc+cs); mat.M33 = (float)( cj*cc-ss);
        } else {
            mat.M11 = (float)(cj*ch); mat.M12 = (float)(sj*sc-cs); mat.M13 = (float)(sj*cc+ss);
            mat.M21 = (float)(cj*sh); mat.M22 = (float)(sj*ss+cc); mat.M23 = (float)(sj*cs-sc);
            mat.M31 = (float)(-sj);   mat.M32 = (float)(cj*si);    mat.M33 = (float)(cj*ci);
        }

        return mat;
    }

    //-- operators --------------------------------------------------
    bool operator== (const nEulerAngles& e)
    {
        return ((x == e.x) && (y == e.y) && (z == e.z)) ? true : false;
    }

    bool operator!= (const nEulerAngles& e)
    {
        return ((x != e.x) || (y != e.y) || (z != e.z)) ? true : false;
    }
};


#endif
