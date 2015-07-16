#ifndef _MATRIX33_SSE_H
#define _MATRIX33_SSE_H
//------------------------------------------------------------------------------
/**
    @class _matrix33_sse
    @ingroup Math

    An SSE based matrix33 class.

    (C) 2002 RadonLabs GmbH
*/
#include "mathlib/_vector3_sse.h"
#include "mathlib/quaternion.h"
#include "mathlib/euler.h"
#include "matrixdefs.h"

#include <string.h>

static float _matrix33_sse_ident[12] = 
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
};

//------------------------------------------------------------------------------
class _matrix33_sse 
{
public:
    /// constructor 1
    _matrix33_sse();
    /// constructor 2
    _matrix33_sse(const _vector3_sse& v1, const _vector3_sse& v2, const _vector3_sse& v3);
    /// constructor 3
    _matrix33_sse(const _matrix33_sse& mx);
    /// constructor 4
    _matrix33_sse(float _m11, float _m12, float _m13, float _m21, float _m22, float _m23, float _m31, float _m32, float _m33);
    /// constructor 5
    _matrix33_sse(const quaternion& q);
    /// constructor 6
    _matrix33_sse(const __m128& _m1, const __m128& _m2, const __m128& _m3);
    /// get as quaternion
    quaternion get_quaternion() const;
    /// get as euler representation
    _vector3_sse to_euler() const;
    /// set as euler
    void from_euler(const _vector3_sse& ea);
    /// unrestricted lookat
    void lookat(const _vector3_sse& from, const _vector3_sse& to, const _vector3_sse& up);
    /// restricted lookat (billboard)
    void billboard(const _vector3_sse& from, const _vector3_sse& to, const _vector3_sse& up);
    /// set 1
    void set(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33);
    /// set 2
    void set(const _vector3_sse& v1, const _vector3_sse& v2, const _vector3_sse& v3);
    /// set 3
    void set(const _matrix33_sse& mx);
    /// set to identity
    void ident();
    /// set to transpose
    void transpose();
    /// is orthonormal?
    bool orthonorm(float limit);
    /// scale
    void scale(const _vector3_sse& s);
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
    void rotate(const _vector3_sse& vec, float a);
    /// get x component
    _vector3_sse x_component(void) const;
    /// get y component
    _vector3_sse y_component(void) const;
    /// get z component
    _vector3_sse z_component(void) const;
    // inplace matrix multiply
    void operator *= (const _matrix33_sse& m1);
    /// multiply source vector into target vector
    void mult(const _vector3_sse& src, _vector3_sse& dst) const;

    union
    {
        struct
        {
            __m128 m1;
            __m128 m2;
            __m128 m3;
        };
        struct
        {
            float m[3][4];
        };
    };
};

//------------------------------------------------------------------------------
/**
    FIXME: OPTIMIZE -> KILL TEMPORARYS, SEE _MATRIX44_SSE
*/
static 
inline 
_matrix33_sse operator * (const _matrix33_sse& ma, const _matrix33_sse& mb) 
{
    return _matrix33_sse(
        _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(ma.m1, ma.m1, _MM_SHUFFLE(0,0,0,0)), mb.m1), 
            _mm_mul_ps(_mm_shuffle_ps(ma.m1, ma.m1, _MM_SHUFFLE(1,1,1,1)), mb.m2)), 
            _mm_mul_ps(_mm_shuffle_ps(ma.m1, ma.m1, _MM_SHUFFLE(2,2,2,2)), mb.m3)),

        _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(ma.m2, ma.m2, _MM_SHUFFLE(0,0,0,0)), mb.m1), 
            _mm_mul_ps(_mm_shuffle_ps(ma.m2, ma.m2, _MM_SHUFFLE(1,1,1,1)), mb.m2)), 
            _mm_mul_ps(_mm_shuffle_ps(ma.m2, ma.m2, _MM_SHUFFLE(2,2,2,2)), mb.m3)),

        _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(ma.m3, ma.m3, _MM_SHUFFLE(0,0,0,0)), mb.m1), 
            _mm_mul_ps(_mm_shuffle_ps(ma.m3, ma.m3, _MM_SHUFFLE(1,1,1,1)), mb.m2)), 
            _mm_mul_ps(_mm_shuffle_ps(ma.m3, ma.m3, _MM_SHUFFLE(2,2,2,2)), mb.m3)));
}

//------------------------------------------------------------------------------
/**
    FIXME: KILL TEMPORARY
*/
static 
inline 
_vector3_sse operator * (const _matrix33_sse& mx, const _vector3_sse& v)
{
    return _vector3_sse(
        _mm_add_ps(
        _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(0,0,0,0)), mx.m1), 
            _mm_mul_ps(_mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(1,1,1,1)), mx.m2)), 
            _mm_mul_ps(_mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(2,2,2,2)), mx.m3)));
}

//------------------------------------------------------------------------------
/**
*/
inline
_matrix33_sse::_matrix33_sse() 
{
    memcpy(&(m[0][0]), _matrix33_sse_ident, sizeof(_matrix33_sse_ident));
}

//------------------------------------------------------------------------------
/**
*/
inline
_matrix33_sse::_matrix33_sse(const _vector3_sse& v1, const _vector3_sse& v2, const _vector3_sse& v3) :
    m1(v1.m128),
    m2(v2.m128),
    m3(v3.m128)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
_matrix33_sse::_matrix33_sse(const _matrix33_sse& mx) :
    m1(mx.m1),
    m2(mx.m2),
    m3(mx.m3)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
_matrix33_sse::_matrix33_sse(float _m11, float _m12, float _m13,
                             float _m21, float _m22, float _m23,
                             float _m31, float _m32, float _m33)
{
    m1 = _mm_set_ps(0.0f, _m13, _m12, _m11);
    m2 = _mm_set_ps(0.0f, _m23, _m22, _m21);
    m3 = _mm_set_ps(0.0f, _m33, _m32, _m31);
}

//------------------------------------------------------------------------------
/**
*/
inline
_matrix33_sse::_matrix33_sse(const __m128& _m1, const __m128& _m2, const __m128& _m3) :
    m1(_m1),
    m2(_m2),
    m3(_m3)
{
    // empty
}

//------------------------------------------------------------------------------
/**
    FIXME: SSE OPTIMIZATION!
*/
inline
_matrix33_sse::_matrix33_sse(const quaternion& q) 
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
    FIXME: SSE OPTIMIZATION!
*/
inline
quaternion 
_matrix33_sse::get_quaternion() const
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
    FIXME: SSE OPTIMIZATION!
*/
inline
_vector3_sse
_matrix33_sse::to_euler() const
{    
    _vector3_sse ea;
    
    // work on matrix with flipped row/columns
    _matrix33_sse tmp(*this);
    tmp.transpose();

    int i,j,k,h,n,s,f;
    EulGetOrd(EulOrdXYZs,i,j,k,h,n,s,f);
    if (s==EulRepYes) 
    {
        double sy = (float) sqrt(tmp.M12 * tmp.M12 + tmp.M13 * tmp.M13);
        if (sy > 16*FLT_EPSILON) 
        {
            ea.x = (float) atan2(tmp.M12, tmp.M13);
            ea.y = (float) atan2(sy, tmp.M11);
            ea.z = (float) atan2(tmp.M21, -tmp.M31);
        } else {
            ea.x = (float) atan2(-tmp.M23, tmp.M22);
            ea.y = (float) atan2(sy, tmp.M11);
            ea.z = 0;
        }
    } 
    else 
    {
        double cy = sqrt(tmp.M11 * tmp.M11 + tmp.M21 * tmp.M21);
        if (cy > 16*FLT_EPSILON) 
        {
            ea.x = (float) atan2(tmp.M32, tmp.M33);
            ea.y = (float) atan2(-tmp.M31, cy);
            ea.z = (float) atan2(tmp.M21, tmp.M11);
        } 
        else 
        {
            ea.x = (float) atan2(-tmp.M23, tmp.M22);
            ea.y = (float) atan2(-tmp.M31, cy);
            ea.z = 0;
        }
    }
    if (n==EulParOdd) {ea.x = -ea.x; ea.y = - ea.y; ea.z = -ea.z;}
    if (f==EulFrmR) {float t = ea.x; ea.x = ea.z; ea.z = t;}

    return ea;
}

//------------------------------------------------------------------------------
/**
    FIXME: SSE OPTIMIZATION!
*/
inline
void 
_matrix33_sse::from_euler(const _vector3_sse& ea) 
{
    _vector3_sse tea = ea;
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
_matrix33_sse::lookat(const _vector3_sse& from, const _vector3_sse& to, const _vector3_sse& up) 
{
    _vector3_sse z(from - to);
    z.norm();
    _vector3_sse y(up);
    _vector3_sse x(y * z);   // x = y cross z
    y = z * x;          // y = z cross x
    x.norm();
    y.norm();

    m1 = x.m128;
    m2 = y.m128;
    m3 = z.m128;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33_sse::billboard(const _vector3_sse& from, const _vector3_sse& to, const _vector3_sse& up)
{
    _vector3_sse z(from - to);
    z.norm();
    _vector3_sse y(up);
    _vector3_sse x(y * z);
    z = x * y;
    x.norm();
    y.norm();
    z.norm();

    m1 = x.m128;
    m2 = y.m128;
    m3 = z.m128;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33_sse::set(float m11, float m12, float m13,
                   float m21, float m22, float m23,
                   float m31, float m32, float m33) 
{
    m1 = _mm_set_ps(0.0f, m13, m12, m11);
    m2 = _mm_set_ps(0.0f, m23, m22, m21);
    m3 = _mm_set_ps(0.0f, m33, m32, m31);
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33_sse::set(const _vector3_sse& v1, const _vector3_sse& v2, const _vector3_sse& v3) 
{
    m1 = v1.m128;
    m2 = v2.m128;
    m3 = v3.m128;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33_sse::set(const _matrix33_sse& mx) 
{
    m1 = mx.m1;
    m2 = mx.m2;
    m3 = mx.m3;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix33_sse::ident() 
{
    memcpy(&(m[0][0]), _matrix33_sse_ident, sizeof(_matrix33_sse_ident));
}

//------------------------------------------------------------------------------
/**
    FIXME: SSE OPTIMIZATION
*/
inline
void 
_matrix33_sse::transpose() 
{
    #define n_swap(x,y) { float t=x; x=y; y=t; }
    n_swap(m[0][1],m[1][0]);
    n_swap(m[0][2],m[2][0]);
    n_swap(m[1][2],m[2][1]);
}

//------------------------------------------------------------------------------
/**
    FIXME: SSE OPTIMIZATION
*/
inline
bool 
_matrix33_sse::orthonorm(float limit) 
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
_matrix33_sse::scale(const _vector3_sse& s)
{
    m1 = _mm_mul_ps(m1, s.m128);
    m2 = _mm_mul_ps(m2, s.m128);
    m3 = _mm_mul_ps(m3, s.m128);
}

//------------------------------------------------------------------------------
/**
    FIXME: SSE OPTIMIZATION
*/
inline
void 
_matrix33_sse::rotate_x(const float a)
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
    FIXME: SSE OPTIMIZATION
*/
inline
void 
_matrix33_sse::rotate_y(const float a)
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
    FIXME: SSE OPTIMIZATION
*/
inline
void 
_matrix33_sse::rotate_z(const float a)
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
    FIXME: SSE OPTIMIZATION
*/
inline
void 
_matrix33_sse::rotate_local_x(const float a)
{
    _matrix33_sse rotM;  // initialized as identity matrix
	rotM.M22 = (float) cos(a); rotM.M23 = -(float) sin(a);
	rotM.M32 = (float) sin(a); rotM.M33 =  (float) cos(a);

	(*this) = rotM * (*this); 
}

//------------------------------------------------------------------------------
/**
    FIXME: SSE OPTIMIZATION
*/
inline
void 
_matrix33_sse::rotate_local_y(const float a)
{
    _matrix33_sse rotM;  // initialized as identity matrix
	rotM.M11 = (float) cos(a);  rotM.M13 = (float) sin(a);
    rotM.M31 = -(float) sin(a); rotM.M33 = (float) cos(a);

	(*this) = rotM * (*this); 
}

//------------------------------------------------------------------------------
/**
    FIXME: SSE OPTIMIZATION
*/
inline
void 
_matrix33_sse::rotate_local_z(const float a)
{
    _matrix33_sse rotM;  // initialized as identity matrix
    rotM.M11 = (float) cos(a); rotM.M12 = -(float) sin(a);
	rotM.M21 = (float) sin(a); rotM.M22 =  (float) cos(a);

	(*this) = rotM * (*this); 
}

//------------------------------------------------------------------------------
/**
    FIXME: SSE OPTIMIZATION
*/
inline
void 
_matrix33_sse::rotate(const _vector3_sse& vec, float a)
{
    _vector3_sse v(vec);
    v.norm();
    float sa = (float) n_sin(a);
    float ca = (float) n_cos(a);

	_matrix33_sse rotM;
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
_vector3_sse 
_matrix33_sse::x_component() const
{
    return _vector3_sse(m1);
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector3_sse 
_matrix33_sse::y_component(void) const
{
    return _vector3_sse(m2);
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector3_sse 
_matrix33_sse::z_component(void) const 
{
    return _vector3_sse(m3);
};

//------------------------------------------------------------------------------
/**
*/
inline
void
_matrix33_sse::operator *= (const _matrix33_sse& mx) 
{
    m1 = _mm_add_ps(
         _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(0,0,0,0)), mx.m1), 
            _mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(1,1,1,1)), mx.m2)), 
            _mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(2,2,2,2)), mx.m3));

    m2 = _mm_add_ps(
         _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(0,0,0,0)), mx.m1), 
            _mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(1,1,1,1)), mx.m2)), 
            _mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(2,2,2,2)), mx.m3));

    m3 = _mm_add_ps(
         _mm_add_ps(
            _mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(0,0,0,0)), mx.m1), 
            _mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(1,1,1,1)), mx.m2)), 
            _mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(2,2,2,2)), mx.m3));
}

//------------------------------------------------------------------------------
/**
    multiply source vector with matrix and store in destination vector
    this eliminates the construction of a temp _vector3_sse object
*/
inline
void 
_matrix33_sse::mult(const _vector3_sse& src, _vector3_sse& dst) const
{
    dst.m128 = _mm_add_ps(
               _mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(src.m128, src.m128, _MM_SHUFFLE(0,0,0,0)), m1), 
                          _mm_mul_ps(_mm_shuffle_ps(src.m128, src.m128, _MM_SHUFFLE(1,1,1,1)), m2)), 
                          _mm_mul_ps(_mm_shuffle_ps(src.m128, src.m128, _MM_SHUFFLE(2,2,2,2)), m3));
}

//------------------------------------------------------------------------------
#endif
