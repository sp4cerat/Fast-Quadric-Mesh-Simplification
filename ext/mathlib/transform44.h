#ifndef N_TRANSFORM44_H
#define N_TRANSFORM44_H
//------------------------------------------------------------------------------
/**
    @class transform44
    @ingroup Math

    A 4x4 matrix which is described by translation, rotation and scale.
    
    (C) 2004 RadonLabs GmbH
*/
#include "mathlib/matrix.h"
#include "mathlib/vector.h"

//------------------------------------------------------------------------------
class transform44
{
public:
    /// constructor
    transform44();
    /// set translation
    void settranslation(const vector3& v);
    /// get translation
    const vector3& gettranslation() const;
    /// set euler rotation
    void seteulerrotation(const vector3& v);
    /// get euler rotation
    const vector3& geteulerrotation() const;
    /// set quaternion rotation
    void setquatrotation(const quaternion& q);
    /// get quaternion rotation
    const quaternion& getquatrotation() const;
    /// set scale
    void setscale(const vector3& v);
    /// get scale
    const vector3& getscale() const;
    /// set optional rotate pivot
    void setrotatepivot(const vector3& p, bool balance);
    /// get optional rotate pivot
    const vector3& getrotatepivot() const;
    /// set optional scale pivot
    void setscalepivot(const vector3& p);
    /// get optional scale pivot
    const vector3& getscalepivot() const;
    /// set matrix 4x4
    void setmatrix(matrix44 matrix); //not per reference - so make shure the data is copyed
    /// get resulting 4x4 matrix
    const matrix44& getmatrix();
    /// return true if euler rotation is used (otherwise quaternion rotation is used)
    bool iseulerrotation() const;
    /// return true if the transformation matrix is dirty
    bool isdirty() const;
    /// return true if rotate pivot has been set
    bool hasrotatepivot() const;
    /// return true if scale pivot has been set
    bool hasscalepivot() const;

private:
    enum
    {
        Dirty          = (1<<0),
        UseEuler       = (1<<1),
        HasRotatePivot = (1<<2),
		HasRotatePivotTranslation = (1<<3),
        HasScalePivot  = (1<<4),
    };
    vector3 translation;
    vector3 euler;
    quaternion quat;
    vector3 scale;
    vector3 rotatePivot;
	vector3 rotatePivotTranslation;
    vector3 scalePivot;
    matrix44 matrix;
    uchar flags;
};

//------------------------------------------------------------------------------
/**
*/
inline
transform44::transform44() :
    scale(1.0f, 1.0f, 1.0f),
    flags(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
transform44::settranslation(const vector3& v)
{
    this->translation = v;
    this->flags |= Dirty;
}

//------------------------------------------------------------------------------
/**
*/
inline
const vector3&
transform44::gettranslation() const
{
    return this->translation;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
transform44::seteulerrotation(const vector3& v)
{
    this->euler = v;
    this->flags |= (Dirty | UseEuler);
}

//------------------------------------------------------------------------------
/**
*/
inline
const vector3&
transform44::geteulerrotation() const
{
    return this->euler;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
transform44::setquatrotation(const quaternion& q)
{
    this->quat = q;
    this->flags |= Dirty;
    this->flags &= ~UseEuler;
}

//------------------------------------------------------------------------------
/**
*/
inline
const quaternion&
transform44::getquatrotation() const
{
    return this->quat;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
transform44::setscale(const vector3& v)
{
    this->scale = v;
    this->flags |= Dirty;
}

//------------------------------------------------------------------------------
/**
*/
inline
const vector3&
transform44::getscale() const
{
    return this->scale;
}

//------------------------------------------------------------------------------
/**
	 Same as Maya MFnTransfrom:
	 If balance if true, then the overall transformation matrix will not change
	 and a compensating transformation will be added to the rotate translate
	 pivot to compensate for the pivot modification.
*/
inline
void
transform44::setrotatepivot(const vector3& p, bool balance)
{
    if (balance)
	{
		this->rotatePivotTranslation = p * -1.f;
		this->flags |= Dirty | HasRotatePivotTranslation;
	}
	this->rotatePivot = p;
    this->flags |= Dirty | HasRotatePivot;
}

//------------------------------------------------------------------------------
/**
*/
inline
const vector3&
transform44::getrotatepivot() const
{
    return this->rotatePivot;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
transform44::setscalepivot(const vector3& p)
{
    this->scalePivot = p;
    this->flags |= Dirty | HasScalePivot;
}

//------------------------------------------------------------------------------
/**
*/
inline
const vector3&
transform44::getscalepivot() const
{
    return this->scalePivot;
}

//------------------------------------------------------------------------------
/**
*/
inline
const matrix44&
transform44::getmatrix()
{
    if (this->flags & Dirty)
    {
        if (this->flags & UseEuler)
        {
            this->matrix.ident();
            if (this->flags & HasScalePivot)
            {
                this->matrix.translate(-this->scalePivot);
            }
            this->matrix.scale(this->scale);
            if (this->flags & HasScalePivot)
            {
                this->matrix.translate(this->scalePivot);
            }
            if (this->flags & HasRotatePivot)
            {
                this->matrix.translate(-this->rotatePivot);
            }
            this->matrix.rotate_x(this->euler.x);
            this->matrix.rotate_y(this->euler.y);
            this->matrix.rotate_z(this->euler.z);
            if (this->flags & HasRotatePivot)
            {
                this->matrix.translate(this->rotatePivot);
            }
			if (this->flags & HasRotatePivotTranslation)
			{
				this->matrix.translate(this->rotatePivotTranslation);
			}
            this->matrix.translate(this->translation);
        }
        else
        {
            this->matrix.ident();
            if (this->flags & HasScalePivot)
            {
                this->matrix.translate(-this->scalePivot);
            }
            this->matrix.scale(this->scale);
            if (this->flags & HasScalePivot)
            {
                this->matrix.translate(this->scalePivot);
            }
            if (this->flags & HasRotatePivot)
            {
                this->matrix.translate(-this->rotatePivot);
            }
            this->matrix.mult_simple(matrix44(this->quat));
            if (this->flags & HasRotatePivot)
            {
                this->matrix.translate(this->rotatePivot);
            }
			if (this->flags & HasRotatePivotTranslation)
			{
				this->matrix.translate(this->rotatePivotTranslation);
			}
            this->matrix.translate(this->translation);
        }
        this->flags &= ~Dirty;
    }
    return this->matrix;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
transform44::iseulerrotation() const
{
    return (0 != (this->flags & UseEuler));
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
transform44::isdirty() const
{
    return (0 != (this->flags & Dirty));
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
transform44::hasrotatepivot() const
{
    return (0 != (this->flags & HasRotatePivot));
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
transform44::hasscalepivot() const
{
    return (0 != (this->flags & HasScalePivot));
}

//------------------------------------------------------------------------------
/**
*/
inline
void
transform44::setmatrix(matrix44 m)
{
    this->matrix = m;
    this->flags &= ~Dirty;
}
//------------------------------------------------------------------------------
#endif

    
    