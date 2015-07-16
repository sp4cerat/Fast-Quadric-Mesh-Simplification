#ifndef _MATRIX44_SSE_H
#define _MATRIX44_SSE_H
//------------------------------------------------------------------------------
/**
    @class _matrix44_sse
    @ingroup Math

    SSE based matrix44 class.

    (C) 2002 RadonLabs GmbH
*/
#include <xmmintrin.h>
#include "mathlib/_vector4_sse.h"
#include "mathlib/_vector3_sse.h"
#include "mathlib/quaternion.h"
#include "mathlib/euler.h"
#include "mathlib/matrixdefs.h"

static float _matrix44_sse_ident[16] = 
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

//------------------------------------------------------------------------------
class _matrix44_sse
{
public:
    /// constructor 1
    _matrix44_sse();
    /// constructor 2
    _matrix44_sse(const _vector4_sse& v1, const _vector4_sse& v2, const _vector4_sse& v3, const _vector4_sse& v4);
    /// constructor 3
    _matrix44_sse(const _matrix44_sse& m1);
    /// constructor 4
    _matrix44_sse(float _m11, float _m12, float _m13, float _m14,
                  float _m21, float _m22, float _m23, float _m24,
                  float _m31, float _m32, float _m33, float _m34,
                  float _m41, float _m42, float _m43, float _m44);
    /// construct from quaternion
    _matrix44_sse(const quaternion& q);
    /// construct from sse variables
    _matrix44_sse(const __m128& _m1, const __m128& _m2, const __m128& _m3, const __m128& _m4);
    /// convert to quaternion
    quaternion get_quaternion() const;
    /// set 1
    void set(const _vector4_sse& v1, const _vector4_sse& v2, const _vector4_sse& v3, const _vector4_sse& v4);
    /// set 2
    void set(const _matrix44_sse& m1);
    /// set 3
    void set(float _m11, float _m12, float _m13, float _m14,
             float _m21, float _m22, float _m23, float _m24,
             float _m31, float _m32, float _m33, float _m34,
             float _m41, float _m42, float _m43, float _m44);
    /// set from quaternion
    void set(const quaternion& q);
    /// set to identity
    void ident();
    /// transpose
    void transpose();
    /// determinant
    float det();
    /// full invert
    void invert(void);
    /// quick invert (if 3x3 rotation and translation)
    void invert_simple(void);
    /// quick multiplication, assumes that M14==M24==M34==0 and M44==1
    void mult_simple(const _matrix44_sse& m1);
    /// transform vector3, projecting back into w=1
    _vector3_sse transform_coord(const _vector3_sse& v) const;
    /// return x component
    _vector3_sse x_component() const;
    /// return y component
    _vector3_sse y_component() const;
    /// return z component
    _vector3_sse z_component() const;
    /// return translate component
    _vector3_sse pos_component() const;
    /// rotate around global x
    void rotate_x(const float a);
    /// rotate around global y
    void rotate_y(const float a);
    /// rotate around global z
    void rotate_z(const float a);
    /// rotate about any axis
    void rotate(const _vector3_sse& vec, float a);
    /// translate
    void translate(const _vector3_sse& t);
    /// set absolute translation
    void set_translation(const _vector3_sse& t);
    /// scale
    void scale(const _vector3_sse& s);
    /// unrestricted lookat
    void lookat(const _vector3_sse& to, const _vector3_sse& up);
    /// restricted lookat
    void billboard(const _vector3_sse& to, const _vector3_sse& up);
    /// inplace matrix mulitply
    void operator *= (const _matrix44_sse& m1);
    /// multiply source vector into target vector, eliminates tmp vector
    void mult(const _vector4_sse& src, _vector4_sse& dst) const;
    /// multiply source vector into target vector, eliminates tmp vector
    void mult(const _vector3_sse& src, _vector3_sse& dst) const;

    union
    {
        struct
        {
            __m128 m1;
            __m128 m2;
            __m128 m3;
            __m128 m4;
        };
        struct
        {
            float m[4][4];
        };
    };
};

//------------------------------------------------------------------------------
/**
*/
inline
_matrix44_sse::_matrix44_sse()
{
    memcpy(&(m[0][0]), _matrix44_sse_ident, sizeof(_matrix44_sse_ident));
}

//------------------------------------------------------------------------------
/**
*/
inline
_matrix44_sse::_matrix44_sse(const _vector4_sse& v1, const _vector4_sse& v2, const _vector4_sse& v3, const _vector4_sse& v4) :
    m1(v1.m128), m2(v2.m128), m3(v3.m128), m4(v4.m128)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
_matrix44_sse::_matrix44_sse(const _matrix44_sse& mx) :
    m1(mx.m1), m2(mx.m2), m3(mx.m3), m4(mx.m4)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
_matrix44_sse::_matrix44_sse(float _m11, float _m12, float _m13, float _m14,
                             float _m21, float _m22, float _m23, float _m24,
                             float _m31, float _m32, float _m33, float _m34,
                             float _m41, float _m42, float _m43, float _m44)
{
    m1 = _mm_set_ps(_m14, _m13, _m12, _m11);
    m2 = _mm_set_ps(_m24, _m23, _m22, _m21);
    m3 = _mm_set_ps(_m34, _m33, _m32, _m31);
    m4 = _mm_set_ps(_m44, _m43, _m42, _m41);
}

//------------------------------------------------------------------------------
/**
    FIXME: SSE OPTIMIZATION MISSING!
*/
inline
_matrix44_sse::_matrix44_sse(const quaternion& q) 
{
    float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
    x2 = q.x + q.x; y2 = q.y + q.y; z2 = q.z + q.z;
    xx = q.x * x2;   xy = q.x * y2;   xz = q.x * z2;
    yy = q.y * y2;   yz = q.y * z2;   zz = q.z * z2;
    wx = q.w * x2;   wy = q.w * y2;   wz = q.w * z2;

    m[0][0] = 1.0f - (yy + zz);
    m[1][0] = xy - wz;
    m[2][0] = xz + wy;

    m[0][1] = xy + wz;
    m[1][1] = 1.0f - (xx + zz);
    m[2][1] = yz - wx;

    m[0][2] = xz - wy;
    m[1][2] = yz + wx;
    m[2][2] = 1.0f - (xx + yy);

    m[3][0] = m[3][1] = m[3][2] = 0.0f;
    m[0][3] = m[1][3] = m[2][3] = 0.0f;
    m[3][3] = 1.0f;
}

//------------------------------------------------------------------------------
/**
*/
inline
_matrix44_sse::_matrix44_sse(const __m128& _m1, const __m128& _m2, const __m128& _m3, const __m128& _m4) :
    m1(_m1), m2(_m2), m3(_m3), m4(_m4)
{
    // empty
}

//------------------------------------------------------------------------------
/**
    convert orientation of 4x4 matrix into quaterion,
    4x4 matrix must not be scaled!

    FIXME: SSE OPTIMIZATION MISSING!
*/
inline
quaternion 
_matrix44_sse::get_quaternion() const
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
void 
_matrix44_sse::set(const _vector4_sse& v1, const _vector4_sse& v2, const _vector4_sse& v3, const _vector4_sse& v4) 
{
    m1 = v1.m128;
    m2 = v2.m128;
    m3 = v3.m128;
    m4 = v4.m128;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix44_sse::set(const _matrix44_sse& mx) 
{
    m1 = mx.m1;
    m2 = mx.m2;
    m3 = mx.m3;
    m4 = mx.m4;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_matrix44_sse::set(float _m11, float _m12, float _m13, float _m14,
                   float _m21, float _m22, float _m23, float _m24,
                   float _m31, float _m32, float _m33, float _m34,
                   float _m41, float _m42, float _m43, float _m44)
{
    m1 = _mm_set_ps(_m14, _m13, _m12, _m11);
    m2 = _mm_set_ps(_m24, _m23, _m22, _m21);
    m3 = _mm_set_ps(_m34, _m33, _m32, _m31);
    m4 = _mm_set_ps(_m44, _m43, _m42, _m41);
}

//------------------------------------------------------------------------------
/**
    FIXME: SSE OPTIMIZATION MISSING!
*/
inline
void 
_matrix44_sse::set(const quaternion& q) 
{
    float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
    x2 = q.x + q.x; y2 = q.y + q.y; z2 = q.z + q.z;
    xx = q.x * x2;   xy = q.x * y2;   xz = q.x * z2;
    yy = q.y * y2;   yz = q.y * z2;   zz = q.z * z2;
    wx = q.w * x2;   wy = q.w * y2;   wz = q.w * z2;

    m[0][0] = 1.0f - (yy + zz);
    m[1][0] = xy - wz;
    m[2][0] = xz + wy;

    m[0][1] = xy + wz;
    m[1][1] = 1.0f - (xx + zz);
    m[2][1] = yz - wx;

    m[0][2] = xz - wy;
    m[1][2] = yz + wx;
    m[2][2] = 1.0f - (xx + yy);

    m[3][0] = m[3][1] = m[3][2] = 0.0f;
    m[0][3] = m[1][3] = m[2][3] = 0.0f;
    m[3][3] = 1.0f;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix44_sse::ident() 
{
    memcpy(&(m[0][0]), _matrix44_sse_ident, sizeof(_matrix44_sse_ident));
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix44_sse::transpose() 
{
    _MM_TRANSPOSE4_PS(m1, m2, m3, m4);
}

//------------------------------------------------------------------------------
/**
    FIXME: OPTIMIZE FOR SSE!
*/
inline
float 
_matrix44_sse::det() 
{
    return
        (M11 * M22 - M12 * M21) * (M33 * M44 - M34 * M43)
       -(M11 * M23 - M13 * M21) * (M32 * M44 - M34 * M42)
       +(M11 * M24 - M14 * M21) * (M32 * M43 - M33 * M42)
       +(M12 * M23 - M13 * M22) * (M31 * M44 - M34 * M41)
       -(M12 * M24 - M14 * M22) * (M31 * M43 - M33 * M41)
       +(M13 * M24 - M14 * M23) * (M31 * M42 - M32 * M41);
}

//------------------------------------------------------------------------------
/**
    Code taken from Intel pdf "Streaming SIMD Extension - Inverse of 4x4 Matrix"
*/
inline
void
_matrix44_sse::invert() 
{
    float* src = &(m[0][0]);

    __m128 minor0, minor1, minor2, minor3;
    __m128 row0, row1, row2, row3;
    __m128 det, tmp1;

    tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src)), (__m64*)(src+ 4));
    row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(src+8)), (__m64*)(src+12));

    row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
    row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);

    tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src+ 2)), (__m64*)(src+ 6));
    row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(src+10)), (__m64*)(src+14));

    row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
    row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);

    tmp1 = _mm_mul_ps(row2, row3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

    minor0 = _mm_mul_ps(row1, tmp1);
    minor1 = _mm_mul_ps(row0, tmp1);

    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

    minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
    minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
    minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);

    tmp1 = _mm_mul_ps(row1, row2);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

    minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
    minor3 = _mm_mul_ps(row0, tmp1);

    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
    
    minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
    minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
    minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);

    tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
    row2 = _mm_shuffle_ps(row2, row2, 0x4E);

    minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
    minor2 = _mm_mul_ps(row0, tmp1);

    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

    minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
    minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
    minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);

    tmp1 = _mm_mul_ps(row0, row1);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

    minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
    minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);

    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

    minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
    minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));

    tmp1 = _mm_mul_ps(row0, row3);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

    minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
    minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);

    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

    minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
    minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));

    tmp1 = _mm_mul_ps(row0, row2);
    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

    minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
    minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));

    tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

    minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
    minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);

    det = _mm_mul_ps(row0, minor0);
    det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
    det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
    tmp1 = _mm_rcp_ss(det);

    det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
    det = _mm_shuffle_ps(det, det, 0x00);

    minor0 = _mm_mul_ps(det, minor0);
    _mm_storel_pi((__m64*)(src), minor0);
    _mm_storeh_pi((__m64*)(src+2), minor0);

    minor1 = _mm_mul_ps(det, minor1);
    _mm_storel_pi((__m64*)(src+4), minor1);
    _mm_storeh_pi((__m64*)(src+6), minor1);

    minor2 = _mm_mul_ps(det, minor2);
    _mm_storel_pi((__m64*)(src+ 8), minor2);
    _mm_storeh_pi((__m64*)(src+10), minor2);

    minor3 = _mm_mul_ps(det, minor3);
    _mm_storel_pi((__m64*)(src+12), minor3);
    _mm_storeh_pi((__m64*)(src+14), minor3);
}

//------------------------------------------------------------------------------
/**
    inverts a 4x4 matrix consisting of a 3x3 rotation matrix and
    a translation (eg. everything that has [0,0,0,1] as
    the rightmost column) MUCH cheaper then a real 4x4 inversion

    FIXME: SSE OPTIMIZATION!
*/
inline
void 
_matrix44_sse::invert_simple() 
{
    float s = det();
    if (s == 0.0f) return;
    s = 1.0f/s;
    this->set(
        s * ((M22 * M33) - (M23 * M32)),
        s * ((M32 * M13) - (M33 * M12)),
        s * ((M12 * M23) - (M13 * M22)),
        0.0f,
        s * ((M23 * M31) - (M21 * M33)),
        s * ((M33 * M11) - (M31 * M13)),
        s * ((M13 * M21) - (M11 * M23)),
        0.0f,
        s * ((M21 * M32) - (M22 * M31)),
        s * ((M31 * M12) - (M32 * M11)),
        s * ((M11 * M22) - (M12 * M21)),
        0.0f,
        s * (M21*(M33*M42 - M32*M43) + M22*(M31*M43 - M33*M41) + M23*(M32*M41 - M31*M42)),
        s * (M31*(M13*M42 - M12*M43) + M32*(M11*M43 - M13*M41) + M33*(M12*M41 - M11*M42)),
        s * (M41*(M13*M22 - M12*M23) + M42*(M11*M23 - M13*M21) + M43*(M12*M21 - M11*M22)),
        1.0f);
}

//------------------------------------------------------------------------------
/**
    optimized multiplication, assumes that M14==M24==M34==0 AND M44==1

    NOTE: On SSE, this is a normal matrix multiplication

    Takes 16 muls, 12 adds and 16 shuffles.
*/
inline
void
_matrix44_sse::mult_simple(const _matrix44_sse& mx) 
{
    m1 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(0,0,0,0)), mx.m1), _mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(1,1,1,1)), mx.m2)), _mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(2,2,2,2)), mx.m3)), _mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(3,3,3,3)), mx.m4));
    m2 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(0,0,0,0)), mx.m1), _mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(1,1,1,1)), mx.m2)), _mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(2,2,2,2)), mx.m3)), _mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(3,3,3,3)), mx.m4));
    m3 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(0,0,0,0)), mx.m1), _mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(1,1,1,1)), mx.m2)), _mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(2,2,2,2)), mx.m3)), _mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(3,3,3,3)), mx.m4));
    m4 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(0,0,0,0)), mx.m1), _mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(1,1,1,1)), mx.m2)), _mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(2,2,2,2)), mx.m3)), _mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(3,3,3,3)), mx.m4));
}

//------------------------------------------------------------------------------
/**
    Transforms a vector by the matrix, projecting the result back into w=1.
    
    FIXME: SSE OPTIMIZATION!
*/
inline
_vector3_sse
_matrix44_sse::transform_coord(const _vector3_sse& v) const
{
    float d = 1.0f / (M14*v.x + M24*v.y + M34*v.z + M44);
    return _vector3_sse(
        (M11*v.x + M21*v.y + M31*v.z + M41) * d,
        (M12*v.x + M22*v.y + M32*v.z + M42) * d,
        (M13*v.x + M23*v.y + M33*v.z + M43) * d);
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector3_sse 
_matrix44_sse::x_component() const
{
    _vector3_sse v(m1);
    return v;
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector3_sse 
_matrix44_sse::y_component() const
{
    _vector3_sse v(m2);
    return v;
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector3_sse 
_matrix44_sse::z_component() const 
{
    _vector3_sse v(m3);
    return v;
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector3_sse 
_matrix44_sse::pos_component() const 
{
    _vector3_sse v(M41, M42, M43);
    return v;
}

//------------------------------------------------------------------------------
/**
    FIXME: SSE OPTIMIERUNG!
*/
inline
void
_matrix44_sse::rotate_x(const float a) 
{
    float c = n_cos(a);
    float s = n_sin(a);
    int i;
    for (i=0; i<4; i++) {
        float mi1 = m[i][1];
        float mi2 = m[i][2];
        m[i][1] = mi1*c + mi2*-s;
        m[i][2] = mi1*s + mi2*c;
    }
}

//------------------------------------------------------------------------------
/**
    FIXME: SSE OPTIMIERUNG!
*/
inline
void 
_matrix44_sse::rotate_y(const float a) 
{
    float c = n_cos(a);
    float s = n_sin(a);
    int i;
    for (i=0; i<4; i++) {
        float mi0 = m[i][0];
        float mi2 = m[i][2];
        m[i][0] = mi0*c + mi2*s;
        m[i][2] = mi0*-s + mi2*c;
    }
}

//------------------------------------------------------------------------------
/**
    FIXME: SSE OPTIMIERUNG!
*/
inline
void 
_matrix44_sse::rotate_z(const float a) 
{
    float c = n_cos(a);
    float s = n_sin(a);
    int i;
    for (i=0; i<4; i++) {
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
_matrix44_sse::translate(const _vector3_sse& t) 
{
    m4 = _mm_add_ps(m4, t.m128);
}

//------------------------------------------------------------------------------
/**
FIXME: RAFAEL HAS NO CLUE ABOUT SSE!
*/
inline
void
_matrix44_sse::set_translation(const _vector3_sse& t) 
{
    m4 = t.m128;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
_matrix44_sse::scale(const _vector3_sse& s) 
{
    // _vector3_sse have the w element set to zero, we need it at 1...
    __m128 scale = _mm_add_ps(_mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f), s.m128);
    m1 = _mm_mul_ps(m1, scale);
    m2 = _mm_mul_ps(m2, scale);
    m3 = _mm_mul_ps(m3, scale);
    m4 = _mm_mul_ps(m4, scale);
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
_matrix44_sse::lookat(const _vector3_sse& to, const _vector3_sse& up) 
{
    _vector3_sse from(M41, M42, M43);
    _vector3_sse z(from - to);
    z.norm();
    _vector3_sse y(up);
    _vector3_sse x(y * z);  // x = y cross z
    y = z * x;              // y = z cross x
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
_matrix44_sse::billboard(const _vector3_sse& to, const _vector3_sse& up)
{
    _vector3_sse from(M41, M42, M43);
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
    This one uses no temp variables, this gives a 10% boost over the same
    code using temp variables even with optimizations turned on.
    Unfortunately it's not very readable though...
*/
inline
void
_matrix44_sse::operator *= (const _matrix44_sse& mx) 
{
    m1 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(0,0,0,0)), mx.m1), _mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(1,1,1,1)), mx.m2)), _mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(2,2,2,2)), mx.m3)), _mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(3,3,3,3)), mx.m4));
    m2 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(0,0,0,0)), mx.m1), _mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(1,1,1,1)), mx.m2)), _mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(2,2,2,2)), mx.m3)), _mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(3,3,3,3)), mx.m4));
    m3 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(0,0,0,0)), mx.m1), _mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(1,1,1,1)), mx.m2)), _mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(2,2,2,2)), mx.m3)), _mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(3,3,3,3)), mx.m4));
    m4 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(0,0,0,0)), mx.m1), _mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(1,1,1,1)), mx.m2)), _mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(2,2,2,2)), mx.m3)), _mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(3,3,3,3)), mx.m4));
}

//------------------------------------------------------------------------------
/**
    FIXME: SSE OPTIMIZATION!
*/
inline
void 
_matrix44_sse::rotate(const _vector3_sse& vec, float a)
{
    _vector3_sse v(vec);
    v.norm();
    float sa = (float) n_sin(a);
    float ca = (float) n_cos(a);

	_matrix44_sse rotM;
	rotM.M11 = ca + (1.0f - ca) * v.x * v.x;
	rotM.M12 = (1.0f - ca) * v.x * v.y - sa * v.z;
	rotM.M13 = (1.0f - ca) * v.z * v.x + sa * v.y;
	rotM.M21 = (1.0f - ca) * v.x * v.y + sa * v.z;
	rotM.M22 = ca + (1.0f - ca) * v.y * v.y;
	rotM.M23 = (1.0f - ca) * v.y * v.z - sa * v.x;
	rotM.M31 = (1.0f - ca) * v.z * v.x - sa * v.y;
	rotM.M32 = (1.0f - ca) * v.y * v.z + sa * v.x;
	rotM.M33 = ca + (1.0f - ca) * v.z * v.z;
	
	(*this) *= rotM;
}

//------------------------------------------------------------------------------
/**
    Multiply source directly into target vector (without creating a
    temporary vector4 object).
*/
inline
void
_matrix44_sse::mult(const _vector4_sse& src, _vector4_sse& dst) const
{
    dst.m128 = _mm_add_ps(
               _mm_add_ps(
               _mm_add_ps(
                    _mm_mul_ps(m1, _mm_shuffle_ps(src.m128, src.m128, _MM_SHUFFLE(0,0,0,0))), 
                    _mm_mul_ps(m2, _mm_shuffle_ps(src.m128, src.m128, _MM_SHUFFLE(1,1,1,1)))), 
                    _mm_mul_ps(m3, _mm_shuffle_ps(src.m128, src.m128, _MM_SHUFFLE(2,2,2,2)))), 
                    _mm_mul_ps(m4, _mm_shuffle_ps(src.m128, src.m128, _MM_SHUFFLE(3,3,3,3))));
}

//------------------------------------------------------------------------------
/**
    Multiply source directly into target vector (without creating a
    temporary vector4 object).
*/
inline
void
_matrix44_sse::mult(const _vector3_sse& src, _vector3_sse& dst) const
{
    dst.m128 = _mm_add_ps(
               _mm_add_ps(
               _mm_add_ps(
                    _mm_mul_ps(m1, _mm_shuffle_ps(src.m128, src.m128, _MM_SHUFFLE(0,0,0,0))), 
                    _mm_mul_ps(m2, _mm_shuffle_ps(src.m128, src.m128, _MM_SHUFFLE(1,1,1,1)))), 
                    _mm_mul_ps(m3, _mm_shuffle_ps(src.m128, src.m128, _MM_SHUFFLE(2,2,2,2)))), 
                    _mm_mul_ps(m4, _mm_shuffle_ps(src.m128, src.m128, _MM_SHUFFLE(3,3,3,3))));
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_matrix44_sse 
operator * (const _matrix44_sse& ma, const _matrix44_sse& mb) 
{
    return _matrix44_sse(
        _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(ma.m1, ma.m1, _MM_SHUFFLE(0,0,0,0)), mb.m1), _mm_mul_ps(_mm_shuffle_ps(ma.m1, ma.m1, _MM_SHUFFLE(1,1,1,1)), mb.m2)), _mm_mul_ps(_mm_shuffle_ps(ma.m1, ma.m1, _MM_SHUFFLE(2,2,2,2)), mb.m3)), _mm_mul_ps(_mm_shuffle_ps(ma.m1, ma.m1, _MM_SHUFFLE(3,3,3,3)), mb.m4)),
        _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(ma.m2, ma.m2, _MM_SHUFFLE(0,0,0,0)), mb.m1), _mm_mul_ps(_mm_shuffle_ps(ma.m2, ma.m2, _MM_SHUFFLE(1,1,1,1)), mb.m2)), _mm_mul_ps(_mm_shuffle_ps(ma.m2, ma.m2, _MM_SHUFFLE(2,2,2,2)), mb.m3)), _mm_mul_ps(_mm_shuffle_ps(ma.m2, ma.m2, _MM_SHUFFLE(3,3,3,3)), mb.m4)),
        _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(ma.m3, ma.m3, _MM_SHUFFLE(0,0,0,0)), mb.m1), _mm_mul_ps(_mm_shuffle_ps(ma.m3, ma.m3, _MM_SHUFFLE(1,1,1,1)), mb.m2)), _mm_mul_ps(_mm_shuffle_ps(ma.m3, ma.m3, _MM_SHUFFLE(2,2,2,2)), mb.m3)), _mm_mul_ps(_mm_shuffle_ps(ma.m3, ma.m3, _MM_SHUFFLE(3,3,3,3)), mb.m4)),
        _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(ma.m4, ma.m4, _MM_SHUFFLE(0,0,0,0)), mb.m1), _mm_mul_ps(_mm_shuffle_ps(ma.m4, ma.m4, _MM_SHUFFLE(1,1,1,1)), mb.m2)), _mm_mul_ps(_mm_shuffle_ps(ma.m4, ma.m4, _MM_SHUFFLE(2,2,2,2)), mb.m3)), _mm_mul_ps(_mm_shuffle_ps(ma.m4, ma.m4, _MM_SHUFFLE(3,3,3,3)), mb.m4))
    );
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector3_sse operator * (const _matrix44_sse& m, const _vector3_sse& v)
{
    return _vector3_sse(
        _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(m.m1, _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(0,0,0,0))), _mm_mul_ps(m.m2, _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(1,1,1,1)))), _mm_mul_ps(m.m3, _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(2,2,2,2)))), _mm_mul_ps(m.m4, _mm_set_ps(0.0f, 1.0f, 1.0f, 1.0f)))
    );
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector4_sse operator * (const _matrix44_sse& m, const _vector4_sse& v)
{
    return _vector4_sse(
        _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(m.m1, _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(0,0,0,0))), _mm_mul_ps(m.m2, _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(1,1,1,1)))), _mm_mul_ps(m.m3, _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(2,2,2,2)))), _mm_mul_ps(m.m4, _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(3,3,3,3))))
    );
}

//------------------------------------------------------------------------------
#endif
