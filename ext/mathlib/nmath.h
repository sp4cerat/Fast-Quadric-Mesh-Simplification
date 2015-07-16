#ifndef N_MATH_H
#define N_MATH_H
//------------------------------------------------------------------------------
/**
    General math functions and macros.

    (C) 2003 RadonLabs GmbH
*/
#include <math.h>
#include <stdlib.h>

#ifdef _MSC_VER
#define isnan _isnan
#define isinf _isinf
#endif

#ifndef PI
#define PI (3.1415926535897932384626433832795028841971693993751f)
#endif

#define N_PI PI

#ifndef TINY
#define TINY (0.0000001f)
#endif

#define n_max(a,b)      (((a) > (b)) ? (a) : (b))
#define n_min(a,b)      (((a) < (b)) ? (a) : (b))
#define n_abs(a)        (((a)<0.0f) ? (-(a)) : (a))
#define n_sgn(a)        (((a)<0.0f) ? (-1) : (1))
#define n_deg2rad(d)    (((d)*PI)/180.0f)
#define n_rad2deg(r)    (((r)*180.0f)/PI)
#define n_sin(x) (float(sin(x)))
#define n_cos(x) (float(cos(x)))
#define n_tan(x) (float(tan(x)))
#define n_atan(x) (float(atan(x)))

//------------------------------------------------------------------------------
/**
    log2() function.
*/
const float LN_2 = 0.693147180559945f;
inline float n_log2(float f) 
{ 
    return logf(f) / LN_2; 
}

//------------------------------------------------------------------------------
/**
    Integer clamping.
*/
inline int n_iclamp(int val, int minVal, int maxVal)
{
    if (val < minVal)      return minVal;
    else if (val > maxVal) return maxVal;
    else return val;
}

//------------------------------------------------------------------------------
/**
    acos with value clamping.
*/
inline float n_acos(float x)
{
    if(x >  1.0f) x =  1.0f;
    if(x < -1.0f) x = -1.0f;
    return (float)acos(x);
}

//------------------------------------------------------------------------------
/**
    asin with value clamping.
*/
inline float n_asin(float x)
{
    if(x >  1.0f) x =  1.0f;
    if(x < -1.0f) x = -1.0f;
    return (float)asin(x);
}

//------------------------------------------------------------------------------
/**
    Safe sqrt.
*/
inline float n_sqrt(float x)
{
    if (x < 0.0f) x = (float) 0.0f;
    return (float) sqrt(x);
}

//------------------------------------------------------------------------------
/**
    A fuzzy floating point equality check
*/
inline bool n_fequal(float f0, float f1, float tol) {
    float f = f0-f1;
    if ((f>(-tol)) && (f<tol)) return true;
    else                       return false;
}

//------------------------------------------------------------------------------
/**
    A fuzzy floating point less-then check.
*/
inline bool n_fless(float f0, float f1, float tol) {
    if ((f0-f1)<tol) return true;
    else             return false;
}

//------------------------------------------------------------------------------
/**
    A fuzzy floating point greater-then check.
*/
inline bool n_fgreater(float f0, float f1, float tol) {
    if ((f0-f1)>tol) return true;
    else             return false;
}

//------------------------------------------------------------------------------
/**
    fast float to int conversion (always truncates)
    see http://www.stereopsis.com/FPU.html for a discussion.
    NOTE: this works only on x86 endian machines.
*/
inline long n_ftol(float val)
{
    double v = double(val) + (68719476736.0*1.5);
    return ((long*)&v)[0] >> 16;
}

//------------------------------------------------------------------------------
/**
    Smooth a new value towards an old value using a change value.
*/
inline float n_smooth(float newVal, float curVal, float maxChange)
{
    float diff = newVal - curVal;
    if (fabs(diff) > maxChange)
    {
        if (diff > 0.0f)
        {
            curVal += maxChange;
            if (curVal > newVal)
            {
                curVal = newVal;
            }
        }
        else if (diff < 0.0f)
        {
            curVal -= maxChange;
            if (curVal < newVal)
            {
                curVal = newVal;
            }
        }
    }
    else
    {
        curVal = newVal;
    }
    return curVal;
}

//------------------------------------------------------------------------------
/**
    Clamp a value against lower und upper boundary.
*/
inline float n_clamp(float val, float lower, float upper)
{
    if (val < lower)      return lower;
    else if (val > upper) return upper;
    else                  return val;
}

//------------------------------------------------------------------------------
/**
    Saturate a value (clamps between 0.0f and 1.0f)
*/
inline float n_saturate(float val)
{
    if (val < 0.0f)      return 0.0f;
    else if (val > 1.0f) return 1.0f;
    else return val;
}

//------------------------------------------------------------------------------
/**
    Return a pseudo random number between 0 and 1.
*/
inline float n_rand()
{
    return float(rand()) / float(RAND_MAX);
}

//------------------------------------------------------------------------------
/**
    Chop float to int.
*/
inline int n_fchop(float f)
{
    // FIXME!
    return int(f);
}

//------------------------------------------------------------------------------
/**
    Round float to integer.
*/
inline int n_frnd(float f)
{
    return n_fchop(floorf(f + 0.5f));
}

//------------------------------------------------------------------------------
/**
    Linearly interpolate between 2 values: ret = x + l * (y - x)
*/
inline float n_lerp(float x, float y, float l)
{
    return x + l * (y - x);
}

//------------------------------------------------------------------------------
#endif

