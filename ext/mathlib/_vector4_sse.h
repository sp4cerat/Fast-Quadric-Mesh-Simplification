#ifndef _VECTOR4_SSE_H
#define _VECTOR4_SSE_H
//------------------------------------------------------------------------------
/**
    @class _vector4_sse
    @ingroup Math

    A vector4 class using SSE.

    (C) 2002 RadonLabs GmbH
*/
#include <xmmintrin.h>

//------------------------------------------------------------------------------
class _vector4_sse
{
public:
    /// constructor 1
    _vector4_sse();
    /// constructor 2
    _vector4_sse(const float _x, const float _y, const float _z, const float _w);
    /// constructor 3
    _vector4_sse(const _vector4_sse& vec);
    /// set elements 1
    void set(const float _x, const float _y, const float _z, const float _w);
    /// set elements 2 
    void set(const _vector4_sse& v);
    /// return length
    float len() const;
    /// normalize
    void norm();
    /// inplace add
    void operator +=(const _vector4_sse& v);
    /// inplace sub
    void operator -=(const _vector4_sse& v);
    /// inplace scalar mul
    void operator *=(const float s);
    /// fuzzy compare
    bool isequal(const _vector4_sse& v, float tol) const;
    /// fuzzy compare, return -1, 0, +1
    int compare(const _vector4_sse& v, float tol) const;
    /// set own components to minimum
    void minimum(const _vector4_sse& v);
    /// set own components to maximum
    void maximum(const _vector4_sse& v);
    /// inplace linear interpolation
    void lerp(const _vector4_sse& v0, float lerpVal);

    union
    {
        __m128 m128;
        struct
        {
            float x, y, z, w;
        };
    };

    /// private constructor, takes _m128
    _vector4_sse(const __m128& m);
    /// add components
	float addhorizontal(const __m128 &a);
};

//------------------------------------------------------------------------------
/**
*/
inline
_vector4_sse::_vector4_sse()
{
    m128 = _mm_setzero_ps();
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector4_sse::_vector4_sse(const float _x, const float _y, const float _z, const float _w)
{
    m128 = _mm_set_ps(_w, _z, _y, _x);
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector4_sse::_vector4_sse(const _vector4_sse& v)
{
    m128 = v.m128;
}

//------------------------------------------------------------------------------
/**
*/
inline
_vector4_sse::_vector4_sse(const __m128& m) :
    m128(m)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4_sse::set(const float _x, const float _y, const float _z, const float _w)
{
    m128 = _mm_set_ps(_w, _z, _y, _x);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4_sse::set(const _vector4_sse& v)
{
    m128 = v.m128;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
_vector4_sse::len() const
{
    const int X = 0;
    const int Y = 1;
    const int Z = 2;
    const int W = 3;

    __m128 a = _mm_mul_ps(m128, m128);

    // horizontal add
    __m128 b = _mm_add_ss(a, _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(X,X,X,X)), _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(Y,Y,Y,Y)), _mm_shuffle_ps(a, a, _MM_SHUFFLE(Z,Z,Z,Z)))));
    __m128 l = _mm_sqrt_ss(b);

    return l.m128_f32[X];
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4_sse::norm()
{
    const int X = 0;
    const int Y = 1;
    const int Z = 2;
    const int W = 3;

    // get len
    __m128 a = _mm_mul_ps(m128, m128);
    __m128 b = _mm_add_ss(a, _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(X,X,X,X)), _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(Y,Y,Y,Y)), _mm_shuffle_ps(a, a, _MM_SHUFFLE(Z,Z,Z,Z)))));
    
    // get reciprocal of square root of squared length
    __m128 f = _mm_rsqrt_ss(b);
    __m128 oneDivLen = _mm_shuffle_ps(f, f, _MM_SHUFFLE(X, X, X, X));
    
    m128 = _mm_mul_ps(m128, oneDivLen);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4_sse::operator +=(const _vector4_sse& v)
{
    m128 = _mm_add_ps(m128, v.m128);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4_sse::operator -=(const _vector4_sse& v)
{
    m128 = _mm_sub_ps(m128, v.m128);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4_sse::operator *=(const float s)
{
    __m128 packed = _mm_set1_ps(s);
    m128 = _mm_mul_ps(m128, packed);
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
_vector4_sse::isequal(const _vector4_sse& v, float tol) const
{
    if (fabs(v.x - x) > tol)      return false;
    else if (fabs(v.y - y) > tol) return false;
    else if (fabs(v.z - z) > tol) return false;
    else if (fabs(v.w - w) > tol) return false;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
_vector4_sse::compare(const _vector4_sse& v, float tol) const
{
    if (fabs(v.x - x) > tol)      return (v.x > x) ? +1 : -1; 
    else if (fabs(v.y - y) > tol) return (v.y > y) ? +1 : -1;
    else if (fabs(v.z - z) > tol) return (v.z > z) ? +1 : -1;
    else if (fabs(v.w - w) > tol) return (v.w > w) ? +1 : -1;
    else                          return 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4_sse::minimum(const _vector4_sse& v)
{
    if (v.x < x) x = v.x;
    if (v.y < y) y = v.y;
    if (v.z < z) z = v.z;
    if (v.w < w) w = v.w;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4_sse::maximum(const _vector4_sse& v)
{
    if (v.x > x) x = v.x;
    if (v.y > y) y = v.y;
    if (v.z > z) z = v.z;
    if (v.w > w) w = v.w;
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector4_sse operator +(const _vector4_sse& v0, const _vector4_sse& v1) 
{
    return _vector4_sse(_mm_add_ps(v0.m128, v1.m128));
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector4_sse operator -(const _vector4_sse& v0, const _vector4_sse& v1) 
{
    return _vector4_sse(_mm_sub_ps(v0.m128, v1.m128));
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector4_sse operator *(const _vector4_sse& v0, const float& s) 
{
    __m128 packed = _mm_set1_ps(s);
    return _vector4_sse(_mm_mul_ps(v0.m128, packed));
}

//------------------------------------------------------------------------------
/**
*/
static 
inline 
_vector4_sse operator -(const _vector4_sse& v)
{
    __m128 zero = _mm_setzero_ps();
    return _vector4_sse(_mm_sub_ps(zero, v.m128));
}

//------------------------------------------------------------------------------
/**
*/
inline
void
_vector4_sse::lerp(const _vector4_sse& v0, float lerpVal)
{
    x = v0.x + ((x - v0.x) * lerpVal);
    y = v0.y + ((y - v0.y) * lerpVal);
    z = v0.z + ((z - v0.z) * lerpVal);
    w = v0.w + ((w - v0.w) * lerpVal);
}

//------------------------------------------------------------------------------
#endif
