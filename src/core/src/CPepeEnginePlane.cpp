#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEnginePlane.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEnginePlane::CPepeEnginePlane()
{
    normal  = CPepeEngineVector3::ZERO;
    d       = 0.0f;
}

// -----------------------------------------------------------------------------------------
CPepeEnginePlane::CPepeEnginePlane(const CPepeEnginePlane& rhs)
{
    normal  = rhs.normal;
    d       = rhs.d;
}

// -----------------------------------------------------------------------------------------
CPepeEnginePlane::CPepeEnginePlane(const CPepeEngineVector3& rkNormal, float fConstant)
{
    normal  = rkNormal;
    d       = -fConstant;
}

// -----------------------------------------------------------------------------------------
CPepeEnginePlane::CPepeEnginePlane(const CPepeEngineVector3& rkNormal, const CPepeEngineVector3& rkPoint)
{
    redefine(rkNormal, rkPoint);
}

// -----------------------------------------------------------------------------------------
CPepeEnginePlane::CPepeEnginePlane(
    const CPepeEngineVector3& rkPoint0,
    const CPepeEngineVector3& rkPoint1,
    const CPepeEngineVector3& rkPoint2)
{
    redefine(rkPoint0, rkPoint1, rkPoint2);
}

// -----------------------------------------------------------------------------------------
CPepeEnginePlane::Side CPepeEnginePlane::getSide(const CPepeEngineVector3& rkPoint) const
{
    float fDistance = getDistance(rkPoint);

    if (fDistance < 0.0) {
        return CPepeEnginePlane::NEGATIVE_SIDE;
    }

    if (fDistance > 0.0) {
        return CPepeEnginePlane::POSITIVE_SIDE;
    }

    return CPepeEnginePlane::NO_SIDE;
}

// -----------------------------------------------------------------------------------------
CPepeEnginePlane::Side CPepeEnginePlane::getSide(const CPepeEngineAxisAlignedBox& box) const
{
    if (box.isNull()) {
        return NO_SIDE;
    }

    if (box.isInfinite()) {
        return BOTH_SIDE;
    }

    return getSide(box.getCenter(), box.getHalfSize());
}

// -----------------------------------------------------------------------------------------
CPepeEnginePlane::Side CPepeEnginePlane::getSide(
    const CPepeEngineVector3& centre, const CPepeEngineVector3& halfSize) const
{
    /*
        Calculate the distance between box centre and the plane
    */
    float dist = getDistance(centre);

    /*
        Calculate the maximise allows absolute distance for
        the distance between box centre and plane
    */
    float maxAbsDist = normal.absDotProduct(halfSize);

    if (dist < -maxAbsDist) {
        return CPepeEnginePlane::NEGATIVE_SIDE;
    }

    if (dist > +maxAbsDist) {
        return CPepeEnginePlane::POSITIVE_SIDE;
    }

    return CPepeEnginePlane::BOTH_SIDE;
}

// -----------------------------------------------------------------------------------------
float CPepeEnginePlane::getDistance(const CPepeEngineVector3& rkPoint) const
{
    return normal.dotProduct(rkPoint) + d;
}

// -----------------------------------------------------------------------------------------
void CPepeEnginePlane::redefine(
    const CPepeEngineVector3& rkPoint0,
    const CPepeEngineVector3& rkPoint1,
    const CPepeEngineVector3& rkPoint2)
{
    CPepeEngineVector3 kEdge1 = rkPoint1 - rkPoint0;
    CPepeEngineVector3 kEdge2 = rkPoint2 - rkPoint0;
    normal = kEdge1.crossProduct(kEdge2);
    normal.normalise();
    d = -normal.dotProduct(rkPoint0);
}

// -----------------------------------------------------------------------------------------
void CPepeEnginePlane::redefine(const CPepeEngineVector3& rkNormal, const CPepeEngineVector3& rkPoint)
{
    normal  = rkNormal;
    d       = -rkNormal.dotProduct(rkPoint);
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEnginePlane::projectVector(const CPepeEngineVector3& p) const
{
    // We know plane normal is unit length, so use simple method
    CPepeEngineMatrix3 xform;
    xform[0][0] = 1.0f - normal.x * normal.x;
    xform[0][1] = -normal.x * normal.y;
    xform[0][2] = -normal.x * normal.z;
    xform[1][0] = -normal.y * normal.x;
    xform[1][1] = 1.0f - normal.y * normal.y;
    xform[1][2] = -normal.y * normal.z;
    xform[2][0] = -normal.z * normal.x;
    xform[2][1] = -normal.z * normal.y;
    xform[2][2] = 1.0f - normal.z * normal.z;
    return xform * p;
}

// -----------------------------------------------------------------------------------------
float CPepeEnginePlane::normalise(void)
{
    float fLength = normal.length();

    // Will also work for zero-sized vectors, but will change nothing
    if (fLength > 1e-08f) {
        float fInvLength = 1.0f / fLength;
        normal *= fInvLength;
        d *= fInvLength;
    }

    return fLength;
}

_PEPE_ENGINE_END