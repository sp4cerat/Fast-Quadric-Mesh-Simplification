#ifndef N_BBOX_H
#define N_BBOX_H
//------------------------------------------------------------------------------
/**
    @class bbox3
    @ingroup Math

    A bounding box class.

    (C) 2004 RadonLabs GmbH
*/
#include "mathlib/vector.h"
#include "mathlib/matrix.h"
#include "mathlib/line.h"

//------------------------------------------------------------------------------
//  bbox3
//------------------------------------------------------------------------------
class bbox3 
{
public:
    /// clip codes
    enum 
    {
        ClipLeft   = (1<<0),
        ClipRight  = (1<<1),
        ClipBottom = (1<<2),
        ClipTop    = (1<<3),
        ClipNear   = (1<<4),
        ClipFar    = (1<<5),
    };
    /// clip status
    enum ClipStatus 
    {
        Outside,
        Inside,
        Clipped,
    };

    /// constructor 1
    bbox3();
    /// constructor 3
    bbox3(const vector3& center, const vector3& extents);
    /// construct bounding box from matrix44
    bbox3(const matrix44& m);
    /// get center point
    vector3 center() const;
    /// get extents of box
    vector3 extents() const;
    /// get size of box
    vector3 size() const;
    /// get diagonal size of box
    float diagonal_size() const;
    /// set from matrix44
    void set(const matrix44& m);
    /// set from center point and extents
    void set(const vector3& center, const vector3& extents);
    /// begin extending the box
    void begin_extend();
    /// extend the box
    void extend(const vector3& v);
    /// this resets the bounding box size to zero if no extend() method was called after begin_extend()
    void end_extend();
    /// extend the box
    void extend(float x, float y, float z);
    /// extend the box
    void extend(const bbox3& box);
    /// transform axis aligned bounding box
    void transform(const matrix44& m);
    /// check for intersection with axis aligned bounding box
    bool intersects(const bbox3& box) const;
    /// check if this box completely contains the parameter box
    bool contains(const bbox3& box) const;
    /// return true if this box contains the position
    bool contains(const vector3& pos) const;
    /// check for intersection with other bounding box
    ClipStatus clipstatus(const bbox3& other) const;
    /// check for intersection with projection volume
    ClipStatus clipstatus(const matrix44& viewProjection) const;
    /// create a matrix which transforms a unit cube to this bounding box
    matrix44 to_matrix44() const;

    vector3 vmin;
    vector3 vmax;
};

//------------------------------------------------------------------------------
/**
*/
inline
bbox3::bbox3()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
bbox3::bbox3(const vector3& center, const vector3& extents)
{
    vmin = center - extents;
    vmax = center + extents;
}

//------------------------------------------------------------------------------
/**
    Construct a bounding box around a 4x4 matrix. The translational part
    defines the center point, and the x,y,z vectors of the matrix
    define the extents.
*/
inline
void
bbox3::set(const matrix44& m)
{
    // get extents
    float xExtent = n_max(n_max(n_abs(m.M11), n_abs(m.M21)), n_abs(m.M31));
    float yExtent = n_max(n_max(n_abs(m.M12), n_abs(m.M22)), n_abs(m.M32));
    float zExtent = n_max(n_max(n_abs(m.M13), n_abs(m.M23)), n_abs(m.M33));
    vector3 extent(xExtent, yExtent, zExtent);

    vector3 center = m.pos_component();
    this->vmin = center - extent;
    this->vmax = center + extent;
}

//------------------------------------------------------------------------------
/**
*/
inline
bbox3::bbox3(const matrix44& m)
{
    this->set(m);
}

//------------------------------------------------------------------------------
/**
*/
inline
vector3
bbox3::center() const
{
    return vector3((vmin + vmax) * 0.5f);
}

//------------------------------------------------------------------------------
/**
*/
inline
vector3
bbox3::extents() const
{
    return vector3((vmax - vmin) * 0.5f);
}

//------------------------------------------------------------------------------
/**
*/
inline
vector3
bbox3::size() const
{
    return vector3(vmax - vmin);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
bbox3::set(const vector3& center, const vector3& extents)
{
    vmin = center - extents;
    vmax = center + extents;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
bbox3::begin_extend()
{
    vmin.set(+1000000.0f,+1000000.0f,+1000000.0f);
    vmax.set(-1000000.0f,-1000000.0f,-1000000.0f);
}

//------------------------------------------------------------------------------
/**
    This just checks whether the extend() method has actually been called after
    begin_extend() and just sets vmin and vmax to the null vector if it hasn't.
*/
inline
void
bbox3::end_extend()
{
    if (vmin.isequal(vector3(+1000000.0f,+1000000.0f,+1000000.0f), TINY) &&
        vmax.isequal(vector3(-1000000.0f,-1000000.0f,-1000000.0f), TINY))
    {
        vmin.set(0.0f, 0.0f, 0.0f);
        vmax.set(0.0f, 0.0f, 0.0f);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
void
bbox3::extend(const vector3& v)
{
    if (v.x < vmin.x) vmin.x = v.x;
    if (v.x > vmax.x) vmax.x = v.x;
    if (v.y < vmin.y) vmin.y = v.y;
    if (v.y > vmax.y) vmax.y = v.y;
    if (v.z < vmin.z) vmin.z = v.z;
    if (v.z > vmax.z) vmax.z = v.z;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
bbox3::extend(float x, float y, float z)
{
    if (x < vmin.x) vmin.x = x;
    if (x > vmax.x) vmax.x = x;
    if (y < vmin.y) vmin.y = y;
    if (y > vmax.y) vmax.y = y;
    if (z < vmin.z) vmin.z = z;
    if (z > vmax.z) vmax.z = z;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
bbox3::extend(const bbox3& box)
{
    if (box.vmin.x < vmin.x) vmin.x = box.vmin.x;
    if (box.vmin.y < vmin.y) vmin.y = box.vmin.y;
    if (box.vmin.z < vmin.z) vmin.z = box.vmin.z;
    if (box.vmax.x > vmax.x) vmax.x = box.vmax.x;
    if (box.vmax.y > vmax.y) vmax.y = box.vmax.y;
    if (box.vmax.z > vmax.z) vmax.z = box.vmax.z;
}

//------------------------------------------------------------------------------
/**
    Transforms this axis aligned bounding by the 4x4 matrix. This bounding
    box must be axis aligned with the matrix, the resulting bounding
    will be axis aligned in the matrix' "destination" space.

    E.g. if you have a bounding box in model space 'modelBox', and a
    'modelView' matrix, the operation
    
    modelBox.transform(modelView)

    would transform the bounding box into view space.
*/
inline
void
bbox3::transform(const matrix44& m)
{
    /*  ?? BUG ??
    // get own extents vector
    vector3 extents = this->extents();
    vector3 center  = this->center();

    // Extent the matrix' (x,y,z) components by our own extent
    // vector. 
    
    matrix44 extentMatrix(
        m.M11 * extents.x, m.M12 * extents.x, m.M13 * extents.x, 0.0f,
        m.M21 * extents.y, m.M22 * extents.y, m.M23 * extents.y, 0.0f,
        m.M31 * extents.z, m.M32 * extents.z, m.M33 * extents.z, 0.0f,
        m.M41 + center.x,  m.M42 + center.y,  m.M43 + center.z,  1.0f);
   
    this->set(extentMatrix);
    */

    vector3 temp, min, max, corners[8];
    bool first = true;
    int i;
        
    corners[0]   = this->vmin;
    corners[1].x = this->vmin.x; corners[1].y = this->vmax.y; corners[1].z = this->vmin.z;
    corners[2].x = this->vmax.x; corners[2].y = this->vmax.y; corners[2].z = this->vmin.z;
    corners[3].x = this->vmax.x; corners[3].y = this->vmin.y; corners[3].z = this->vmin.z;            
    corners[4]   = this->vmax;
    corners[5].x = this->vmin.x; corners[5].y = this->vmax.y; corners[5].z = this->vmax.z;
    corners[6].x = this->vmin.x; corners[6].y = this->vmin.y; corners[6].z = this->vmax.z;
    corners[7].x = this->vmax.x; corners[7].y = this->vmin.y; corners[7].z = this->vmax.z;

    for(i = 0; i < 8; ++i)
    {
        // Transform and check extents
        temp = m * corners[i];
        if (first || temp.x > max.x)   max.x = temp.x;
        if (first || temp.y > max.y)   max.y = temp.y;
        if (first || temp.z > max.z)   max.z = temp.z;
        if (first || temp.x < min.x)   min.x = temp.x;
        if (first || temp.y < min.y)   min.y = temp.y;
        if (first || temp.z < min.z)   min.z = temp.z;
        first = false;
    }

    this->vmin = min;
    this->vmax = max;
}

//------------------------------------------------------------------------------
/**
    Check for intersection of 2 axis aligned bounding boxes. The 
    bounding boxes must live in the same coordinate space.
*/
inline
bool
bbox3::intersects(const bbox3& box) const
{
    if ((this->vmax.x < box.vmin.x) || (this->vmin.x > box.vmax.x) ||
        (this->vmax.y < box.vmin.y) || (this->vmin.y > box.vmax.y) ||
        (this->vmax.z < box.vmin.z) || (this->vmin.z > box.vmax.z))
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    Check if the parameter bounding box is completely contained in this
    bounding box.
*/
inline
bool
bbox3::contains(const bbox3& box) const
{
    if ((this->vmin.x < box.vmin.x) && (this->vmax.x >= box.vmax.x) &&
        (this->vmin.y < box.vmin.y) && (this->vmax.y >= box.vmax.y) &&
        (this->vmin.z < box.vmin.z) && (this->vmax.z >= box.vmax.z))
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Check if position is inside bounding box.

    - 27-Nov-03     floh    typo bugfix!
*/
inline
bool
bbox3::contains(const vector3& v) const
{
    if ((this->vmin.x < v.x) && (this->vmax.x >= v.x) &&
        (this->vmin.y < v.y) && (this->vmax.y >= v.y) &&
        (this->vmin.z < v.z) && (this->vmax.z >= v.z))
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Return box/box clip status.
*/
inline
bbox3::ClipStatus
bbox3::clipstatus(const bbox3& other) const
{
    if (this->contains(other))
    {
        return Inside;
    }
    else if (this->intersects(other))
    {
        return Clipped;
    }
    else 
    {
        return Outside;
    }
}

//------------------------------------------------------------------------------
/**
    Check for intersection with a view volume defined by a view-projection
    matrix.
*/
inline
bbox3::ClipStatus
bbox3::clipstatus(const matrix44& viewProjection) const
{
    int andFlags = 0xffff;
    int orFlags  = 0;
    int i;
    static vector4 v0;
    static vector4 v1;
    for (i = 0; i < 8; i++)
    {
        int clip = 0;
        v0.w = 1.0f;
        if (i & 1) v0.x = this->vmin.x;
        else       v0.x = this->vmax.x;
        if (i & 2) v0.y = this->vmin.y;
        else       v0.y = this->vmax.y;
        if (i & 4) v0.z = this->vmin.z;
        else       v0.z = this->vmax.z;

        v1 = viewProjection * v0;

        if (v1.x < -v1.w)       clip |= ClipLeft;
        else if (v1.x > v1.w)   clip |= ClipRight;
        if (v1.y < -v1.w)       clip |= ClipBottom;
        else if (v1.y > v1.w)   clip |= ClipTop;
        if (v1.z < -v1.w)       clip |= ClipFar;
        else if (v1.z > v1.w)   clip |= ClipNear;
        andFlags &= clip;
        orFlags  |= clip;
    }
    if (0 == orFlags)       return Inside;
    else if (0 != andFlags) return Outside;
    else                    return Clipped;
}

//------------------------------------------------------------------------------
/**
    Create a transform matrix which would transform a unit cube to this
    bounding box.
*/
inline
matrix44
bbox3::to_matrix44() const
{
    matrix44 m;
    m.scale(this->size());
    m.translate(this->center());
    return m;
}

//------------------------------------------------------------------------------
/**
*/
inline
float
bbox3::diagonal_size() const
{
    return vector3::distance(this->vmin, this->vmax);
}

//------------------------------------------------------------------------------
#endif
