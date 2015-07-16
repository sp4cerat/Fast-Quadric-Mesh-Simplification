#ifndef _MATRIX33_H
#define _MATRIX33_H
//------------------------------------------------------------------------------
/**
    @class _matrix33
    @ingroup Math

    A generic matrix33 class.

    (C) 2002 RadonLabs GmbH
*/
#include "mathlib/_vector3.h"
#include "mathlib/quaternion.h"
#include "mathlib/euler.h"
#include "matrixdefs.h"
#include <memory.h>

static float _matrix33_ident[9] = 
{
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};

//------------------------------------------------------------------------------
class _matrix33 
{
public:
    /// constructor 1
    _matrix33();
    /// constructor 2
    _matrix33(const _vector3& v0, const _vector3& v1, const _vector3& v2);
    /// constructor 3
    _matrix33(const _matrix33& mx);
    /// constructor 4
    _matrix33(float _m11, float _m12, float _m13, float _m21, float _m22, float _m23, float _m31, float _m32, float _m33);
    /// constructor 5
    _matrix33(const quaternion& q);
    /// get as quaternion
    quaternion get_quaternion() const;
    /// get as euler representation
    _vector3 to_euler() const;
    /// set as euler
    void from_euler(const _vector3& ea);
    /// unrestricted lookat
    void lookat(const _vector3& from, const _vector3& to, const _vector3& up);
    /// restricted lookat (billboard)
    void billboard(const _vector3& from, const _vector3& to, const _vector3& up);
    /// set 1
    void set(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33);
    /// set 2
    void set(const _vector3& v0, const _vector3& v1, const _vector3& v2);
    /// set 3
    void set(const _matrix33& m1);
    /// set to identity
    void ident();
    /// set to transpose
    void transpose();
    /// is orthonormal?
    bool orthonorm(float limit);
    /// scale
    void scale(const _vector3& s);
    /// rotate about global x
    void rotate_x(const float a);
    /// rotates matrix about global y
    void rotate_y(const float a);
    /// rotate about global z
    void rotate_z(const float a);
    /// rotate about local x (not very fast)
    void rotate_local_x(const float a);
    /// rotate about local y (not very fast)
    void rotate_local_y(const float a);
    /// rotate about local z (not very fast)
    void rotate_local_z(const float a);
    /// rotate about any axis
    void rotate(const _vector3& vec, float a);
    /// get x component
    _vector3 x_component(void) const;
    /// get y component
    _vector3 y_component(void) const;
    /// get z component
    _vector3 z_component(void) const;
    // inplace matrix multiply
    void operator *= (const _matrix33& m1);
    /// multiply source vector into target vector
    void mult(const _vector3& src, _vector3& dst) const;
    /// translate, this treats the matrix as a 2x2 rotation + translate matrix
    void translate(const _vector2& t);

    float m[3][3];
};

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_matrix33
operator * (const _matrix33& m0, const _matrix33& m1) 
{
    _matrix33 m2(
        m0.m[0][0]*m1.m[0][0] + m0.m[0][1]*m1.m[1][0] + m0.m[0][2]*m1.m[2][0],
        m0.m[0][0]*m1.m[0][1] + m0.m[0][1]*m1.m[1][1] + m0.m[0][2]*m1.m[2][1],
        m0.m[0][0]*m1.m[0][2] + m0.m[0][1]*m1.m[1][2] + m0.m[0][2]*m1.m[2][2],

        m0.m[1][0]*m1.m[0][0] + m0.m[1][1]*m1.m[1][0] + m0.m[1][2]*m1.m[2][0],
        m0.m[1][0]*m1.m[0][1] + m0.m[1][1]*m1.m[1][1] + m0.m[1][2]*m1.m[2][1],
        m0.m[1][0]*m1.m[0][2] + m0.m[1][1]*m1.m[1][2] + m0.m[1][2]*m1.m[2][2],

        m0.m[2][0]*m1.m[0][0] + m0.m[2][1]*m1.m[1][0] + m0.m[2][2]*m1.m[2][0],
        m0.m[2][0]*m1.m[0][1] + m0.m[2][1]*m1.m[1][1] + m0.m[2][2]*m1.m[2][1],
        m0.m[2][0]*m1.m[0][2] + m0.m[2][1]*m1.m[1][2] + m0.m[2][2]*m1.m[2][2]
    );
    return m2;
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector3 operator * (const _matrix33& m, const _vector3& v)
{
    return _vector3(
        m.M11*v.x + m.M21*v.y + m.M31*v.z,
        m.M12*v.x + m.M22*v.y + m.M32*v.z,
        m.M13*v.x + m.M23*v.y + m.M33*v.z);
};

//------------------------------------------------------------------------------
/**
*/
inline
_matrix33::_matrix33() 
{
    memcpy(&(m[0][0]), _matrix33_ident, sizeof(_matrix33_ident));
}

//------------------------------------------------------------------------------
/**
*/
inline
_matrix33::_matrix33(const _vector3& v0, const _vector3& v1, const _vector3& v2) 
{
    M11=v0.x; M12=v0.y; M13=v0.z;
    M21=v1.x; M22=v1.y; M23=v1.z;
    M31=v2.x; M32=v2.y; M33=v2.z;
}

//------------------------------------------------------------------------------
/**
*/
inline
_matrix33::_matrix33(const _matrix33& m1) 
{
    memcpy(m, &(m1.m[0][0]), 9*sizeof(float));
}

//------------------------------------------------------------------------------
/**
*/
inline
_matrix33::_matrix33(float _m11, float _m12, float _m13,
                     float _m21, float _m22, float _m23,
                     float _m31, float _m32, float _m33)
{
    M11=_m11; M12=_m12; M13=_m13;
    M21=_m21; M22=_m22; M23=_m23;
    M31=_m31; M32=_m32; M33=_m33;
}

//------------------------------------------------------------------------------
/**
*/
inline
_matrix33::_matrix33(const quaternion& q) 
{
    float xx = q.x*q.x; float yy = q.y*q.y; float zz = q.z*q.z;
    float xy = q.x*q.y; float xz = q.x*q.z; float yz = q.y*q.z;
    float wx = q.w*q.x; float wy = q.w*q.y; float wz = q.w*q.z;

    m[0][0] = 1.0f - 2.0f * (yy + zz);
    m[1][0] =        2.0f * (xy - wz);
    m[2][0] =        2.0f * (xz + wy);

    m[0][1] =        2.0f * (xy + wz);
    m[1][1] = 1.0f - 2.0f * (xx + zz);
    m[2][1] =        2.0f * (yz - wx);

    m[0][2] =        2.0f * (xz - wy);
    m[1][2] =        2.0f * (yz + wx);
    m[2][2] = 1.0f - 2.0f * (xx + yy);
}

//------------------------------------------------------------------------------
/**
*/
inline
quaternion 
_matrix33::get_quaternion() const
{
    float qa[4];
    float tr = m[0][0] + m[1][1] + m[2][2];
    if (tr > 0.0f) 
    {
        float s = n_sqrt (tr + 1.0f);
        qa[3] = s * 0.5f;
        s = 0.5f / s;
        qa[0] = (m[1][2] - m[2][1]) * s;
        qa[1] = (m[2][0] - m[0][2]) * s;
        qa[2] = (m[0][1] - m[1][0]) * s;
    } 
    else 
    {
        int i, j, k, nxt[3] = {1,2,0};
        i = 0;
        if (m[1][1] > m[0][0]) i=1;
        if (m[2][2] > m[i][i]) i=2;
        j = nxt[i];
        k = nxt[j];
        float s = n_sqrt((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);
        qa[i] = s * 0.5f;
        s = 0.5f / s;
        qa[3] = (m[j][k] - m[k][j])* s;
        qa[j] = (m[i][j] + m[j][i]) * s;
        qa[k] = (m[i][k] + m[k][i]) * s;
    }
    quaternion q(qa[0],qa[1],qa[2],qa[3]);
    return q;
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector3
_matrix33::to_euler() const
{    
    _vector3 ea;
    
    // work on matrix with flipped row/columns
    _matrix33 tmp(*this);
    tmp.transpose();

    int i,j,k,h,n,s,f;
    EulGetOrd(EulOrdXYZs,i,j,k,h,n,s,f);
    if (s==EulRepYes) 
    {
        double sy = (float) sqrt(tmp.M12 * tmp.M12 + tmp.M13 * tmp.M13);
        if (sy > 16*FLT_EPSILON) 
        {
            ea.x = (float) atan2(tmp.M12, tmp.M13);
            ea.y = (float) atan2((float)sy, tmp.M11);
            ea.z = (float) atan2(tmp.M21, -tmp.M31);
        } else {
            ea.x = (float) atan2(-tmp.M23, tmp.M22);
            ea.y = (float) atan2((float)sy, tmp.M11);
            ea.z = 0;
        }
    } 
    else 
    {
        double cy = sqrt(tmp.M11 * tmp.M11 + tmp.M21 * tmp.M21);
        if (cy > 16*FLT_EPSILON) 
        {
            ea.x = (float) atan2(tmp.M32, tmp.M33);
            ea.y = (float) atan2(-tmp.M31, (float)cy);
            ea.z = (float) atan2(tmp.M21, tmp.M11);
        } 
        else 
        {
            ea.x = (float) atan2(-tmp.M23, tmp.M22);
            ea.y = (float) atan2(-tmp.M31, (float)cy);
            ea.z = 0;
        }
    }
    if (n==EulParOdd) {ea.x = -ea.x; ea.y = - ea.y; ea.z = -ea.z;}
    if (f==EulFrmR) {float t = ea.x; ea.x = ea.z; ea.z = t;}

    return ea;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33::from_euler(const _vector3& ea) 
{
    _vector3 tea = ea;
    double ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
    int i,j,k,h,n,s,f;
    EulGetOrd(EulOrdXYZs,i,j,k,h,n,s,f);
    if (f==EulFrmR) {float t = ea.x; tea.x = ea.z; tea.z = t;}
    if (n==EulParOdd) {tea.x = -ea.x; tea.y = -ea.y; tea.z = -ea.z;}
    ti = tea.x;   tj = tea.y;   th = tea.z;
    ci = cos(ti); cj = cos(tj); ch = cos(th);
    si = sin(ti); sj = sin(tj); sh = sin(th);
    cc = ci*ch; cs = ci*sh; sc = si*ch; ss = si*sh;
    if (s==EulRepYes) 
    {
        M11 = (float)(cj);     M12 = (float)(sj*si);     M13 = (float)(sj*ci);
        M21 = (float)(sj*sh);  M22 = (float)(-cj*ss+cc); M23 = (float)(-cj*cs-sc);
        M31 = (float)(-sj*ch); M23 = (float)( cj*sc+cs); M33 = (float)( cj*cc-ss);
    } 
    else 
    {
        M11 = (float)(cj*ch); M12 = (float)(sj*sc-cs); M13 = (float)(sj*cc+ss);
        M21 = (float)(cj*sh); M22 = (float)(sj*ss+cc); M23 = (float)(sj*cs-sc);
        M31 = (float)(-sj);   M32 = (float)(cj*si);    M33 = (float)(cj*ci);
    }

    // flip row/column
    this->transpose();
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33::lookat(const _vector3& from, const _vector3& to, const _vector3& up) 
{
    _vector3 z(from - to);
    z.norm();
    _vector3 x(up * z);   // x = y cross z
    x.norm();
    _vector3 y = z * x;   // y = z cross x

    M11=x.x;  M12=x.y;  M13=x.z;
    M21=y.x;  M22=y.y;  M23=y.z;
    M31=z.x;  M32=z.y;  M33=z.z;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33::billboard(const _vector3& from, const _vector3& to, const _vector3& up)
{
    _vector3 z(from - to);
    z.norm();
    _vector3 y(up);
    y.norm();
    _vector3 x(y * z);
    z = x * y;

    M11=x.x;  M12=x.y;  M13=x.z;
    M21=y.x;  M22=y.y;  M23=y.z;
    M31=z.x;  M32=z.y;  M33=z.z;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33::set(float m11, float m12, float m13,
               float m21, float m22, float m23,
               float m31, float m32, float m33) 
{
    M11=m11; M12=m12; M13=m13;
    M21=m21; M22=m22; M23=m23;
    M31=m31; M32=m32; M33=m33;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33::set(const _vector3& v0, const _vector3& v1, const _vector3& v2) 
{
    M11=v0.x; M12=v0.y; M13=v0.z;
    M21=v1.x; M22=v1.y; M23=v1.z;
    M31=v2.x; M32=v2.y; M33=v2.z;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33::set(const _matrix33& m1) 
{
    memcpy(m, &(m1.m), 9*sizeof(float));
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33::ident() 
{
    memcpy(&(m[0][0]), _matrix33_ident, sizeof(_matrix33_ident));
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33::transpose() 
{
    #define n_swap(x,y) { float t=x; x=y; y=t; }
    n_swap(m[0][1],m[1][0]);
    n_swap(m[0][2],m[2][0]);
    n_swap(m[1][2],m[2][1]);
}

//------------------------------------------------------------------------------
/**
*/
inline
bool 
_matrix33::orthonorm(float limit) 
{
    if (((M11*M21+M12*M22+M13*M23)<limit) &&
        ((M11*M31+M12*M32+M13*M33)<limit) &&
        ((M31*M21+M32*M22+M33*M23)<limit) &&
        ((M11*M11+M12*M12+M13*M13)>(1.0-limit)) &&
        ((M11*M11+M12*M12+M13*M13)<(1.0+limit)) &&
        ((M21*M21+M22*M22+M23*M23)>(1.0-limit)) &&
        ((M21*M21+M22*M22+M23*M23)<(1.0+limit)) &&
        ((M31*M31+M32*M32+M33*M33)>(1.0-limit)) &&
        ((M31*M31+M32*M32+M33*M33)<(1.0+limit)))
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_matrix33::scale(const _vector3& s)
{
    int i;
    for (i=0; i<3; i++) {
        m[i][0] *= s.x;
        m[i][1] *= s.y;
        m[i][2] *= s.z;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33::rotate_x(const float a)
{
    float c = n_cos(a);
    float s = n_sin(a);
    int i;
    for (i=0; i<3; i++)
    {
        float mi1 = m[i][1];
        float mi2 = m[i][2];
        m[i][1] = mi1*c + mi2*-s;
        m[i][2] = mi1*s + mi2*c;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33::rotate_y(const float a)
{
    float c = n_cos(a);
    float s = n_sin(a);
    int i;
    for (i=0; i<3; i++)
    {
        float mi0 = m[i][0];
        float mi2 = m[i][2];
        m[i][0] = mi0*c + mi2*s;
        m[i][2] = mi0*-s + mi2*c;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33::rotate_z(const float a)
{
    float c = n_cos(a);
    float s = n_sin(a);
    int i;
    for (i=0; i<3; i++)
    {
        float mi0 = m[i][0];
        float mi1 = m[i][1];
        m[i][0] = mi0*c + mi1*-s;
        m[i][1] = mi0*s + mi1*c;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33::rotate_local_x(const float a)
{
    _matrix33 rotM;  // initialized as identity matrix
	rotM.M22 = (float) cos(a); rotM.M23 = -(float) sin(a);
	rotM.M32 = (float) sin(a); rotM.M33 =  (float) cos(a);

	(*this) = rotM * (*this); 
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33::rotate_local_y(const float a)
{
    _matrix33 rotM;  // initialized as identity matrix
	rotM.M11 = (float) cos(a);  rotM.M13 = (float) sin(a);
    rotM.M31 = -(float) sin(a); rotM.M33 = (float) cos(a);

	(*this) = rotM * (*this); 
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33::rotate_local_z(const float a)
{
    _matrix33 rotM;  // initialized as identity matrix
    rotM.M11 = (float) cos(a); rotM.M12 = -(float) sin(a);
	rotM.M21 = (float) sin(a); rotM.M22 =  (float) cos(a);

	(*this) = rotM * (*this); 
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33::rotate(const _vector3& vec, float a)
{
    _vector3 v(vec);
    v.norm();
    float sa = (float) n_sin(a);
    float ca = (float) n_cos(a);

	_matrix33 rotM;
	rotM.M11 = ca + (1.0f - ca) * v.x * v.x;
	rotM.M12 = (1.0f - ca) * v.x * v.y - sa * v.z;
	rotM.M13 = (1.0f - ca) * v.z * v.x + sa * v.y;
	rotM.M21 = (1.0f - ca) * v.x * v.y + sa * v.z;
	rotM.M22 = ca + (1.0f - ca) * v.y * v.y;
	rotM.M23 = (1.0f - ca) * v.y * v.z - sa * v.x;
	rotM.M31 = (1.0f - ca) * v.z * v.x - sa * v.y;
	rotM.M32 = (1.0f - ca) * v.y * v.z + sa * v.x;
	rotM.M33 = ca + (1.0f - ca) * v.z * v.z;
	
	(*this) = (*this) * rotM;
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector3 
_matrix33::x_component() const
{
    _vector3 v(M11,M12,M13);
    return v;
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector3 
_matrix33::y_component(void) const
{
    _vector3 v(M21,M22,M23);
    return v;
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector3 
_matrix33::z_component(void) const 
{
    _vector3 v(M31,M32,M33);
    return v;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
_matrix33::operator *= (const _matrix33& m1) 
{
    int i;
    for (i=0; i<3; i++) {
        float mi0 = m[i][0];
        float mi1 = m[i][1];
        float mi2 = m[i][2];
        m[i][0] = mi0*m1.m[0][0] + mi1*m1.m[1][0] + mi2*m1.m[2][0];
        m[i][1] = mi0*m1.m[0][1] + mi1*m1.m[1][1] + mi2*m1.m[2][1];
        m[i][2] = mi0*m1.m[0][2] + mi1*m1.m[1][2] + mi2*m1.m[2][2];
    };
}

//------------------------------------------------------------------------------
/**
    multiply source vector with matrix and store in destination vector
    this eliminates the construction of a temp _vector3 object
*/
inline
void 
_matrix33::mult(const _vector3& src, _vector3& dst) const
{
    dst.x = M11*src.x + M21*src.y + M31*src.z;
    dst.y = M12*src.x + M22*src.y + M32*src.z;
    dst.z = M13*src.x + M23*src.y + M33*src.z;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_matrix33::translate(const _vector2& t)
{
    M31 += t.x;
    M32 += t.y;
}

//------------------------------------------------------------------------------
#endif
