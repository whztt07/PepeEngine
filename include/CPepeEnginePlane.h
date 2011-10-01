/**
 * Project: PepeEngine
 * Tier:    Frontend
 * File:    CPepeEnginePlane.h
 *
 * @brief   Declaration of CPepeEnginePlane class.
 *
 *  Original free version by:
 *  Magic Software, Inc.
 *  http://www.geometrictools.com/
 *  Copyright (c) 2000, All Rights Reserved
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-08-19
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEPLANE_H
#define CPEPEENGINEPLANE_H

#include "PepeEngineInit.h"

#include "CPepeEngineVector3.h"

_PEPE_ENGINE_START

/**
 *  Defines a plane in 3D space.
 *
 *  A plane is defined in 3D space by the equation
 *  Ax + By + Cz + D = 0

 *  This equates to a vector (the normal of the plane, whose x, y
 *  and z components equate to the coefficients A, B and C
 *  respectively), and a constant (D) which is the distance along
 *  the normal you have to go to move the plane back to the origin.
 */
class _PepeEngineExport CPepeEnginePlane
{
public:

    CPepeEngineVector3 normal;
    float d;

    /**
     *  Default constructor - sets everything to 0.
     */
    CPepeEnginePlane();
    CPepeEnginePlane(const CPepeEnginePlane& rhs);

    /**
     *  Construct a plane through a normal, and a distance to move the plane along the normal.
     */
    CPepeEnginePlane(const CPepeEngineVector3& rkNormal, float fConstant);
    CPepeEnginePlane(const CPepeEngineVector3& rkNormal, const CPepeEngineVector3& rkPoint);
    CPepeEnginePlane(const CPepeEngineVector3& rkPoint0, const CPepeEngineVector3& rkPoint1, const CPepeEngineVector3& rkPoint2);

    /**
     *  The "positive side" of the plane is the half space to which the
     *  plane normal points. The "negative side" is the other half
     *  space. The flag "no side" indicates the plane itself.
     */
    enum Side {
        NO_SIDE,
        POSITIVE_SIDE,
        NEGATIVE_SIDE,
        BOTH_SIDE
    };

    Side getSide(const CPepeEngineVector3& rkPoint) const;

    /**
     *  Returns the side where the aligneBox is. the flag BOTH_SIDE indicates an intersecting box.
     *  one corner ON the plane is sufficient to consider the box and the plane intersecting.
    */
    Side getSide (const CPepeEngineAxisAlignedBox& rkBox) const;

    /**
     *  Returns which side of the plane that the given box lies on.
     *  The box is defined as centre/half-size pairs for effectively.
     *
     *  @param centre The centre of the box.
     *  @param halfSize The half-size of the box.
     *
     *  @return
     *      POSITIVE_SIDE if the box complete lies on the "positive side" of the plane,
     *      NEGATIVE_SIDE if the box complete lies on the "negative side" of the plane,
     *      and BOTH_SIDE if the box intersects the plane.
    */
    Side getSide(const CPepeEngineVector3& centre, const CPepeEngineVector3& halfSize) const;

    /**
     *  This is a pseudodistance. The sign of the return value is
     *  positive if the point is on the positive side of the plane,
     *  negative if the point is on the negative side, and zero if the
     *  point is on the plane.
     *
     *  The absolute value of the return value is the true distance only
     *  when the plane normal is a unit length vector.
     */
    float getDistance(const CPepeEngineVector3& rkPoint) const;

    /**
     *  Redefine this plane based on 3 points.
     */
    void redefine(
        const CPepeEngineVector3& rkPoint0,
        const CPepeEngineVector3& rkPoint1,
        const CPepeEngineVector3& rkPoint2
    );

    /**
     *  Redefine this plane based on a normal and a point.
     */
    void redefine(const CPepeEngineVector3& rkNormal, const CPepeEngineVector3& rkPoint);

    /**
     *  Project a vector onto the plane.
     *  This gives you the element of the input vector that is perpendicular
     *  to the normal of the plane. You can get the element which is parallel
     *  to the normal of the plane by subtracting the result of this method
     *  from the original vector, since parallel + perpendicular = original.
     *
     *  @param v The input vector
     */
    CPepeEngineVector3 projectVector(const CPepeEngineVector3& v) const;

    /**
     *  Normalises the plane.
     *
     *  This method normalises the plane's normal and the length scale of d
     *  is as well.
     *
     *  This function will not crash for zero-sized vectors, but there
     *  will be no changes made to their components.
     *
     *  @return The previous length of the plane's normal.
     */
    float normalise(void);

    /**
     *  Comparison operator
     */
    bool operator==(const CPepeEnginePlane& rhs) const {
        return (rhs.d == d && rhs.normal == normal);
    }
    bool operator!=(const CPepeEnginePlane& rhs) const {
        return (rhs.d != d && rhs.normal != normal);
    }
};
typedef std::vector<CPepeEnginePlane> PlaneList;

_PEPE_ENGINE_END

#endif