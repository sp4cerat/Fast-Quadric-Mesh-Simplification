#ifndef N_MATRIX_H
#define N_MATRIX_H
//-------------------------------------------------------------------
//  CLASSES
//  matrix33    -- 3x3 matrix
//  matrix34    -- 3x4 matrix
//  matrix44    -- 4x4 matrix
//-------------------------------------------------------------------
#include "mathlib/ntypes.h"

#ifndef __USE_SSE__
#include "mathlib/_matrix33.h"
#include "mathlib/_matrix44.h"
typedef _matrix33 matrix33;
typedef _matrix44 matrix44;
#else
#include "mathlib/_matrix33_sse.h"
#include "mathlib/_matrix44_sse.h"
typedef _matrix33_sse matrix33;
typedef _matrix44_sse matrix44;
#endif

//-------------------------------------------------------------------
#endif
