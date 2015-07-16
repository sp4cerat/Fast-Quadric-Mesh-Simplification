/**** EulerAngles.h - Support for 24 angle schemes ****/
/* Ken Shoemake, 1993 */
#ifndef _H_EulerAngles
#define _H_EulerAngles

/*** Definitions ***/
typedef struct {float x, y, z, w;} QuatX; /* Quaternion */
//enum QuatPart {X, Y, Z, W};
typedef float HMatrix[4][4]; /* Right-handed, for column vectors */
typedef QuatX EulerAngles;    /* (x,y,z)=ang 1,2,3, w=order code  */

/*** Order type constants, constructors, extractors ***/
    /* There are 24 possible conventions, designated by:    */
    /*    o EulAxI = axis used initially                    */
    /*    o EulPar = parity of axis permutation             */
    /*    o EulRep = repetition of initial axis as last     */
    /*    o EulFrm = frame from which axes are taken        */
    /* Axes I,J,K will be a permutation of X,Y,Z.           */
    /* Axis H will be either I or K, depending on EulRep.   */
    /* Frame S takes axes from initial static frame.        */
    /* If ord = (AxI=X, Par=Even, Rep=No, Frm=S), then      */
    /* {a,b,c,ord} means Rz(c)Ry(b)Rx(a), where Rz(c)v      */
    /* rotates v around Z by c radians.                     */
#define EulFrmS      0
#define EulFrmR      1
#define EulFrm(ord)  ((unsigned)(ord)&1)
#define EulRepNo     0
#define EulRepYes    1
#define EulRep(ord)  (((unsigned)(ord)>>1)&1)
#define EulParEven   0
#define EulParOdd    1
#define EulPar(ord)  (((unsigned)(ord)>>2)&1)
#define EulSafe      "\000\001\002\000"
#define EulNext      "\001\002\000\001"
#define EulAxI(ord)  ((int)(EulSafe[(((unsigned)(ord)>>3)&3)]))
#define EulAxJ(ord)  ((int)(EulNext[EulAxI(ord)+(EulPar(ord)==EulParOdd)]))
#define EulAxK(ord)  ((int)(EulNext[EulAxI(ord)+(EulPar(ord)!=EulParOdd)]))
#define EulAxH(ord)  ((EulRep(ord)==EulRepNo)?EulAxK(ord):EulAxI(ord))
    /* EulGetOrd unpacks all useful information about order simultaneously. */
#define EulGetOrd(ord,i,j,k,h,n,s,f) {unsigned o=ord;f=o&1;o>>=1;s=o&1;o>>=1;\
    n=o&1;o>>=1;i=EulSafe[o&3];j=EulNext[i+n];k=EulNext[i+1-n];h=s?k:i;}
    /* EulOrd creates an order value between 0 and 23 from 4-tuple choices. */
#define EulOrd(i,p,r,f)    (((((((i)<<1)+(p))<<1)+(r))<<1)+(f))
    /* Static axes */
enum xx_yy_zz_ww
{
    XX = (1<<0),
    YY = (1<<1),
    ZZ = (1<<2),
    WW = (1<<3),
};

#define EulOrdXYZs    EulOrd(XX,EulParEven,EulRepNo,EulFrmS)
#define EulOrdXYXs    EulOrd(XX,EulParEven,EulRepYes,EulFrmS)
#define EulOrdXZYs    EulOrd(XX,EulParOdd,EulRepNo,EulFrmS)
#define EulOrdXZXs    EulOrd(XX,EulParOdd,EulRepYes,EulFrmS)
#define EulOrdYZXs    EulOrd(YY,EulParEven,EulRepNo,EulFrmS)
#define EulOrdYZYs    EulOrd(YY,EulParEven,EulRepYes,EulFrmS)
#define EulOrdYXZs    EulOrd(YY,EulParOdd,EulRepNo,EulFrmS)
#define EulOrdYXYs    EulOrd(YY,EulParOdd,EulRepYes,EulFrmS)
#define EulOrdZXYs    EulOrd(ZZ,EulParEven,EulRepNo,EulFrmS)
#define EulOrdZXZs    EulOrd(ZZ,EulParEven,EulRepYes,EulFrmS)
#define EulOrdZYXs    EulOrd(ZZ,EulParOdd,EulRepNo,EulFrmS)
#define EulOrdZYZs    EulOrd(ZZ,EulParOdd,EulRepYes,EulFrmS)
    /* Rotating axes */
#define EulOrdZYXr    EulOrd(XX,EulParEven,EulRepNo,EulFrmR)
#define EulOrdXYXr    EulOrd(XX,EulParEven,EulRepYes,EulFrmR)
#define EulOrdYZXr    EulOrd(XX,EulParOdd,EulRepNo,EulFrmR)
#define EulOrdXZXr    EulOrd(XX,EulParOdd,EulRepYes,EulFrmR)
#define EulOrdXZYr    EulOrd(YY,EulParEven,EulRepNo,EulFrmR)
#define EulOrdYZYr    EulOrd(YY,EulParEven,EulRepYes,EulFrmR)
#define EulOrdZXYr    EulOrd(YY,EulParOdd,EulRepNo,EulFrmR)
#define EulOrdYXYr    EulOrd(YY,EulParOdd,EulRepYes,EulFrmR)
#define EulOrdYXZr    EulOrd(ZZ,EulParEven,EulRepNo,EulFrmR)
#define EulOrdZXZr    EulOrd(ZZ,EulParEven,EulRepYes,EulFrmR)
#define EulOrdXYZr    EulOrd(ZZ,EulParOdd,EulRepNo,EulFrmR)
#define EulOrdZYZr    EulOrd(ZZ,EulParOdd,EulRepYes,EulFrmR)

EulerAngles Eul_(float ai, float aj, float ah, int order);
QuatX Eul_ToQuat(EulerAngles ea);
void Eul_ToHMatrix(EulerAngles ea, HMatrix M);
EulerAngles Eul_FromHMatrix(HMatrix M, int order);
EulerAngles Eul_FromQuat(QuatX q, int order);
#endif
