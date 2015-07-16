#ifndef N_PKNORM_H
#define N_PKNORM_H
//-------------------------------------------------------------------
/**
    @class pknorm3
    @ingroup Math

    A normal packed into 16 bits

    (C) 2004 RadonLabs GmbH
*/
#include "mathlib/vector.h"

//-------------------------------------------------------------------
class pknorm3 {
    ushort n;

public:
    //--- pack each component into 5 bits ---------------------------
    void pack(vector3& v) {
        const float r = 31.999f;
        unsigned int ix = (unsigned int) (r * (v.x+1.0f)*0.5f);
        unsigned int iy = (unsigned int)(r * (v.y+1.0f)*0.5f);
        unsigned int iz = (unsigned int) (r * (v.z+1.0f)*0.5f);
        n = ((ix&31)<<10) | ((iy&31)<<5) | (iz&31);
    };

    //--- unpack into vector3 ---------------------------------------
    vector3 unpack(void) {
        const float r = 31.999f;
        float ix = float((n>>10) & 31);
        float iy = float((n>>5) & 31);
        float iz = float(n & 31);
        vector3 v((((ix/r)*2.0f)-1.0f),
                  (((iy/r)*2.0f)-1.0f),
                  (((iz/r)*2.0f)-1.0f));
        return v;
    };

    //-- constructors -----------------------------------------------
    pknorm3() : n(0) {};
    pknorm3(vector3& v) {
        pack(v);
    };

    //-- get/set packed representation ------------------------------
    ushort getpacked(void) {
        return n;
    };
    void setpacked(ushort _n) {
        n = _n;
    };
};
//-------------------------------------------------------------------
#endif        
