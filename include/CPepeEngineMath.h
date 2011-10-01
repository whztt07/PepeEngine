/**
* Project:  PepeEngine
* Tier:     Frontend
* File:     CPepeEngineMath.h
*
* @brief    Declaration of CPepeEngineMath class.
*
* @author       Piotr 'pepe' Picheta
* @date         2008-07-22
* @copyright    Copyright (c) 2008 Piotr Picheta
*
* @version 1.0
*/

#ifndef CPEPEENGINEMATH_H
#define CPEPEENGINEMATH_H

#include "PepeEngineInit.h"
#include <limits>

_PEPE_ENGINE_START

class Degree;

/**
 *  Wrapper class _PepeEngineExport which indicates a given angle value is in Radians.
 *
 *  Radian values are interchangeable with Degree values, and conversions
 *  will be done automatically between them.
 */
class Radian
{
    float mRad;
public:
    explicit Radian(float r = 0) : mRad(r) {}
    Radian (const Degree& d);
    Radian& operator=(const float& f) {
        mRad = f;
        return *this;
    }
    Radian& operator=(const Radian& r) {
        mRad = r.mRad;
        return *this;
    }
    Radian& operator=(const Degree& d);

    float valueDegrees() const; // see bottom of this file
    float valueRadians() const {
        return mRad;
    }

    const Radian& operator+() const {
        return *this;
    }
    Radian operator+(const Radian& r) const {
        return Radian(mRad + r.mRad);
    }
    Radian operator+(const Degree& d) const;
    Radian& operator+=(const Radian& r) {
        mRad += r.mRad;
        return *this;
    }
    Radian& operator+=(const Degree& d);
    Radian operator-() const {
        return Radian(-mRad);
    }
    Radian operator-(const Radian& r) const {
        return Radian(mRad - r.mRad);
    }
    Radian operator-(const Degree& d) const;
    Radian& operator-=(const Radian& r) {
        mRad -= r.mRad;
        return *this;
    }
    Radian& operator-=(const Degree& d);
    Radian operator*(float f) const {
        return Radian(mRad * f);
    }
    Radian operator*(const Radian& f) const {
        return Radian(mRad * f.mRad);
    }
    Radian& operator*=(float f) {
        mRad *= f;
        return *this;
    }
    Radian operator/(float f) const {
        return Radian(mRad / f);
    }
    Radian& operator/=(float f) {
        mRad /= f;
        return *this;
    }

    bool operator < (const Radian& r) const {
        return mRad <  r.mRad;
    }
    bool operator <=(const Radian& r) const {
        return mRad <= r.mRad;
    }
    bool operator ==(const Radian& r) const {
        return mRad == r.mRad;
    }
    bool operator !=(const Radian& r) const {
        return mRad != r.mRad;
    }
    bool operator >=(const Radian& r) const {
        return mRad >= r.mRad;
    }
    bool operator > (const Radian& r) const {
        return mRad >  r.mRad;
    }
};

/**
 *  Wrapper class _PepeEngineExport which indicates a given angle value is in Degrees.
 *  Degree values are interchangeable with Radian values, and conversions
 *  will be done automatically between them.
 */
class Degree
{
    float mDeg;
public:
    explicit Degree(float d = 0) : mDeg(d) {}
    Degree(const Radian& r) : mDeg(r.valueDegrees()) {}
    Degree& operator=(const float& f) {
        mDeg = f;
        return *this;
    }
    Degree& operator=(const Degree& d) {
        mDeg = d.mDeg;
        return *this;
    }
    Degree& operator=(const Radian& r) {
        mDeg = r.valueDegrees();
        return *this;
    }

    float valueDegrees() const {
        return mDeg;
    }
    float valueRadians() const; // see bottom of this file

    const Degree& operator+() const {
        return *this;
    }
    Degree operator+(const Degree& d) const {
        return Degree(mDeg + d.mDeg);
    }
    Degree operator+(const Radian& r) const {
        return Degree(mDeg + r.valueDegrees());
    }
    Degree& operator+=(const Degree& d) {
        mDeg += d.mDeg;
        return *this;
    }
    Degree& operator+=(const Radian& r) {
        mDeg += r.valueDegrees();
        return *this;
    }
    Degree operator-() const {
        return Degree(-mDeg);
    }
    Degree operator-(const Degree& d) const {
        return Degree(mDeg - d.mDeg);
    }
    Degree operator-(const Radian& r) const {
        return Degree(mDeg - r.valueDegrees());
    }
    Degree& operator-=(const Degree& d) {
        mDeg -= d.mDeg;
        return *this;
    }
    Degree& operator-=(const Radian& r) {
        mDeg -= r.valueDegrees();
        return *this;
    }
    Degree operator*(float f) const {
        return Degree(mDeg * f);
    }
    Degree operator*(const Degree& f) const {
        return Degree(mDeg * f.mDeg);
    }
    Degree& operator*=(float f) {
        mDeg *= f;
        return *this;
    }
    Degree operator/(float f) const {
        return Degree(mDeg / f);
    }
    Degree& operator/=(float f) {
        mDeg /= f;
        return *this;
    }

    bool operator<(const Degree& d) const {
        return mDeg <  d.mDeg;
    }
    bool operator<=(const Degree& d) const {
        return mDeg <= d.mDeg;
    }
    bool operator==(const Degree& d) const {
        return mDeg == d.mDeg;
    }
    bool operator!=(const Degree& d) const {
        return mDeg != d.mDeg;
    }
    bool operator>=(const Degree& d) const {
        return mDeg >= d.mDeg;
    }
    bool operator>(const Degree& d) const {
        return mDeg > d.mDeg;
    }
};


/*
 *  These functions could not be defined within the class _PepeEngineExport definition of class _PepeEngineExport
 *  Radian because they required class _PepeEngineExport Degree to be defined
 */
inline Radian::Radian(const Degree& d) : mRad(d.valueRadians()) {}

inline Radian& Radian::operator=(const Degree& d)
{
    mRad = d.valueRadians();
    return *this;
}

inline Radian Radian::operator+(const Degree& d) const
{
    return Radian(mRad + d.valueRadians());
}

inline Radian& Radian::operator+=(const Degree& d)
{
    mRad += d.valueRadians();
    return *this;
}

inline Radian Radian::operator-(const Degree& d) const
{
    return Radian(mRad - d.valueRadians());
}

inline Radian& Radian::operator-=(const Degree& d)
{
    mRad -= d.valueRadians();
    return *this;
}


class _PepeEngineExport CPepeEngineMath
{
public:

    static const float POS_INFINITY;
    static const float NEG_INFINITY;
    static const float PI;
    static const float TWO_PI;
    static const float HALF_PI;
    static const float fDeg2Rad;
    static const float fRad2Deg;

    static inline float Abs(float f) {
        return (float)fabs(f);
    }
    static float invSqrt(float f);
    static float Sqrt(float f);

    static inline Degree Abs(const Degree& dValue) {
        return Degree(fabs(dValue.valueDegrees()));
    }
    static inline Radian Abs(const Radian& rValue) {
        return Radian(fabs(rValue.valueRadians()));
    }
    static Radian ACos(float fValue);
    static Radian ASin(float fValue);
    static inline Radian ATan (float fValue) {
        return Radian(atan(fValue));
    }
    static inline Radian ATan2 (float fY, float fX) {
        return Radian(atan2(fY, fX));
    }
    static inline float Ceil (float fValue) {
        return float(ceil(fValue));
    }

    static inline float Cos(const Radian& fValue) {
        return float(cos(fValue.valueRadians()));
    }
    static inline float Cos(float fValue) {
        return float(cos(fValue));
    }
    static inline float Exp(float fValue) {
        return float(exp(fValue));
    }
    static inline float Floor(float fValue) {
        return float(floor(fValue));
    }
    static inline float Log(float fValue) {
        return float(log(fValue));
    }
    static inline float Pow (float fBase, float fExponent) {
        return float(pow(fBase, fExponent));
    }
    static float Sign(float fValue);
    static inline Radian Sign( const Radian& rValue ) {
        return Radian(Sign(rValue.valueRadians()));
    }
    static inline Degree Sign(const Degree& dValue) {
        return Degree(Sign(dValue.valueDegrees()));
    }
    static inline float Sin(const Radian& fValue) {
        return float(sin(fValue.valueRadians()));
    }
    static inline float Sin(float fValue) {
        return float(sin(fValue));
    }
    static inline float Sqr(float fValue) {
        return fValue * fValue;
    }
    static inline Radian Sqrt (const Radian& fValue) {
        return Radian(sqrt(fValue.valueRadians()));
    }
    static inline Degree Sqrt (const Degree& fValue) {
        return Degree(sqrt(fValue.valueDegrees()));
    }
    static inline float Tan(const Radian& fValue) {
        return float(tan(fValue.valueRadians()));
    }
    static inline float Tan(float fValue) {
        return float(tan(fValue));
    }
    static float degreesToRadians(float degrees);
    static float radiansToDegrees(float radians);

    static float unitRandom();  // in [0,1]
    static float rangeRandom(float fLow, float fHigh);  // in [fLow,fHigh]
    static float symmetricRandom();  // in [-1,1]

    static CPepeEngineVector3 calculateFaceNormal(
        const CPepeEngineVector3& v1,
        const CPepeEngineVector3& v2,
        const CPepeEngineVector3& v3
    );

    static bool floatEqual(float a, float b, float tolerance = std::numeric_limits<float>::epsilon());

    /**
     *  Sphere / box intersection test.
     */
    static bool intersects(const CPepeEngineSphere& sphere, const CPepeEngineAxisAlignedBox& box);

    /**
     *  Plane / box intersection test.
     */
    static bool intersects(const CPepeEnginePlane& plane, const CPepeEngineAxisAlignedBox& box);

    /**
     *  Sphere / plane intersection test.
     *  NB just do a plane.getDistance(sphere.getCenter()) for more detail!
     */
    static bool intersects(const CPepeEngineSphere& sphere, const CPepeEnginePlane& plane);
};

inline float Radian::valueDegrees() const
{
    return CPepeEngineMath::radiansToDegrees(mRad);
}
inline float Degree::valueRadians() const
{
    return CPepeEngineMath::degreesToRadians(mDeg);
}

inline Radian operator*(float a, const Radian& b)
{
    return Radian(a * b.valueRadians());
}

inline Radian operator/(float a, const Radian& b)
{
    return Radian(a / b.valueRadians());
}

inline Degree operator*(float a, const Degree& b)
{
    return Degree(a * b.valueDegrees());
}

inline Degree operator/(float a, const Degree& b)
{
    return Degree(a / b.valueDegrees());
}

_PEPE_ENGINE_END

#endif