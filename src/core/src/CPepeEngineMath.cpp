#include "PepeEnginePrecompiledHeaders.h"
#include "asm_math.h"
#include "CPepeEngineMath.h"

_PEPE_ENGINE_START

_PepeEngineExport const float CPepeEngineMath::POS_INFINITY = std::numeric_limits<float>::infinity();
_PepeEngineExport const float CPepeEngineMath::NEG_INFINITY = -std::numeric_limits<float>::infinity();
_PepeEngineExport const float CPepeEngineMath::PI           = float(4.0 * atan(1.0));
_PepeEngineExport const float CPepeEngineMath::TWO_PI       = float(2.0 * PI);
_PepeEngineExport const float CPepeEngineMath::HALF_PI      = float(0.5 * PI);
_PepeEngineExport const float CPepeEngineMath::fDeg2Rad     = PI / float(180.0);
_PepeEngineExport const float CPepeEngineMath::fRad2Deg     = float(180.0) / PI;

// -----------------------------------------------------------------------------------------
float CPepeEngineMath::invSqrt(float f)
{
    return asm_invSqrt(f);
}

// -----------------------------------------------------------------------------------------
float CPepeEngineMath::Sqrt(float f)
{
    return asm_sqrt(f);
}

// -----------------------------------------------------------------------------------------
Radian CPepeEngineMath::ACos(float fValue)
{
    if (-1.0 < fValue) {
        if (fValue < 1.0) {
            return Radian(acos(fValue));
        } else {
            return Radian(0.0);
        }
    } else {
        return Radian(PI);
    }
}

// -----------------------------------------------------------------------------------------
Radian CPepeEngineMath::ASin(float fValue)
{
    if (-1.0 < fValue) {
        if (fValue < 1.0) {
            return Radian(asin(fValue));
        } else {
            return Radian(HALF_PI);
        }
    } else {
        return Radian(-HALF_PI);
    }
}

// -----------------------------------------------------------------------------------------
float CPepeEngineMath::Sign(float fValue)
{
    if (fValue > 0.0) {
        return 1.0;
    }

    if (fValue < 0.0) {
        return -1.0;
    }

    return 0.0;
}

// -----------------------------------------------------------------------------------------
float CPepeEngineMath::degreesToRadians(float degrees)
{
    return degrees * fDeg2Rad;
}

// -----------------------------------------------------------------------------------------
float CPepeEngineMath::radiansToDegrees(float radians)
{
    return radians * fRad2Deg;
}

// -----------------------------------------------------------------------------------------
float CPepeEngineMath::unitRandom()
{
    return asm_rand() / asm_rand_max();
}

// -----------------------------------------------------------------------------------------
float CPepeEngineMath::rangeRandom(float fLow, float fHigh)
{
    return (fHigh - fLow) * unitRandom() + fLow;
}

// -----------------------------------------------------------------------------------------
float CPepeEngineMath::symmetricRandom()
{
    return 2.0f * unitRandom() - 1.0f;
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEngineMath::calculateFaceNormal(
    const CPepeEngineVector3& v1,
    const CPepeEngineVector3& v2,
    const CPepeEngineVector3& v3
)
{
    CPepeEngineVector3 normal = (v2 - v1).crossProduct(v3 - v1);
    normal.normalise();
    return normal;
}


// -----------------------------------------------------------------------------------------
bool CPepeEngineMath::floatEqual(float a, float b, float tolerance)
{
    if (fabs(b - a) <= tolerance) {
        return true;
    } else {
        return false;
    }
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineMath::intersects(const CPepeEngineSphere& sphere, const CPepeEngineAxisAlignedBox& box)
{
    if (box.isNull()) {
        return false;
    }

    if (box.isInfinite()) {
        return true;
    }

    // Use splitting planes
    const CPepeEngineVector3& center    = sphere.getCenter();
    float radius                        = sphere.getRadius();
    const CPepeEngineVector3& min       = box.getMinimum();
    const CPepeEngineVector3& max       = box.getMaximum();

    // Arvo's algorithm
    float s, d = 0;

    for (int i = 0; i < 3; ++i) {
        if (center.ptr()[i] < min.ptr()[i]) {
            s = center.ptr()[i] - min.ptr()[i];
            d += s * s;
        } else if(center.ptr()[i] > max.ptr()[i]) {
            s = center.ptr()[i] - max.ptr()[i];
            d += s * s;
        }
    }

    return d <= radius * radius;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineMath::intersects(const CPepeEnginePlane& plane, const CPepeEngineAxisAlignedBox& box)
{
    return (plane.getSide(box) == CPepeEnginePlane::BOTH_SIDE);
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineMath::intersects(const CPepeEngineSphere& sphere, const CPepeEnginePlane& plane)
{
    return (CPepeEngineMath::Abs(plane.getDistance(sphere.getCenter())) <= sphere.getRadius());
}

_PEPE_ENGINE_END