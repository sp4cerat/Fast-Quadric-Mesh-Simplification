#ifndef N_PERLIN_H
#define N_PERLIN_H
//------------------------------------------------------------------------------
/**
    @class noise
    @ingroup Math
    
    Perlin noise class.
    
    See http://mrl.nyu.edu/~perlin/noise/ for details.

    (C) 2004 RadonLabs GmbH
*/
#include "kernel/ntypes.h"
#include "math.h"

//------------------------------------------------------------------------------
class noise
{
public:
    /// generate noise value
    static float gen(float x, float y, float z);

private:
    /// compute fade curve
    static float fade(float t);
    /// lerp between a and b
    static float lerp(float t, float a, float b);
    /// convert into gradient direction
    static float grad(int hash, float x, float y, float z);

    static int perm[512];
};

//------------------------------------------------------------------------------
/**
*/
inline
float
noise::fade(float t)
{
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

//------------------------------------------------------------------------------
/**
*/
inline
float
noise::lerp(float t, float a, float b)
{
    return a + t * (b - a);
}

//------------------------------------------------------------------------------
/**
*/
inline
float
noise::grad(int hash, float x, float y, float z)
{
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : ((h == 12) || (h==14)) ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

//------------------------------------------------------------------------------
/**
*/
inline
float
noise::gen(float x, float y, float z)
{
    float floorX = floorf(x);
    float floorY = floorf(y);
    float floorZ = floorf(z);

    // find unit cube that contains point
    int X = int(floorX) & 255;
    int Y = int(floorY) & 255;
    int Z = int(floorZ) & 255;

    // find relative x,y,z of point in cube
    x -= floorX;
    y -= floorY;
    z -= floorZ;

    // compute fade curves for x, y, z
    float u = fade(x);
    float v = fade(y);
    float w = fade(z);

    // hash coords of 8 cube corners
    int A  = perm[X] + Y;
    int AA = perm[A] + Z;
    int AB = perm[A+1] + Z;
    int B  = perm[X+1] + Y;
    int BA = perm[B] + Z;
    int BB = perm[B+1] + Z;

    // add blended results from 8 corners of cube
    return lerp(w, lerp(v, lerp(u, grad(perm[AA  ], x  , y  , z   ),
                                   grad(perm[BA  ], x-1, y  , z   )),
                           lerp(u, grad(perm[AB  ], x  , y-1, z   ),
                                   grad(perm[BB  ], x-1, y-1, z   ))),
                   lerp(v, lerp(u, grad(perm[AA+1], x  , y  , z-1 ),
                                   grad(perm[BA+1], x-1, y  , z-1 )),
                           lerp(u, grad(perm[AB+1], x  , y-1, z-1 ),
                                   grad(perm[BB+1], x-1, y-1, z-1 ))));
}

//------------------------------------------------------------------------------
#endif