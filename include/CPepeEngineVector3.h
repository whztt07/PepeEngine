/**
 * Project: PepeEngine
 * Tier:    Frontend
 * File:    CPepeEngineVector3.h
 *
 * @brief   Declaration of CPepeEngineVector3 class.
 *
 *  Original free version by:
 *  Magic Software, Inc.
 *  http://www.geometrictools.com/
 *  Copyright (c) 2000, All Rights Reserved
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-06-07
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEVECTOR3_H
#define CPEPEENGINEVECTOR3_H

#include "PepeEngineInit.h"

#include "CPepeEngineMath.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineVector3
{
public:

    // special points
    static const CPepeEngineVector3 ZERO;
    static const CPepeEngineVector3 UNIT_X;
    static const CPepeEngineVector3 UNIT_Y;
    static const CPepeEngineVector3 UNIT_Z;
    static const CPepeEngineVector3 NEGATIVE_UNIT_X;
    static const CPepeEngineVector3 NEGATIVE_UNIT_Y;
    static const CPepeEngineVector3 NEGATIVE_UNIT_Z;
    static const CPepeEngineVector3 UNIT_SCALE;

    float x, y, z;

    inline CPepeEngineVector3() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    inline CPepeEngineVector3(const float fX, const float fY, const float fZ)
        : x(fX), y(fY), z(fZ) {}

    inline explicit CPepeEngineVector3(const float afCoordinate[3])
        : x(afCoordinate[0]), y(afCoordinate[1]), z(afCoordinate[2]) {}

    inline explicit CPepeEngineVector3(const int afCoordinate[3]) {
        x = (float)afCoordinate[0];
        y = (float)afCoordinate[1];
        z = (float)afCoordinate[2];
    }

    inline explicit CPepeEngineVector3(float* const r)
        : x(r[0]), y(r[1]), z(r[2]) {}

    inline explicit CPepeEngineVector3(const float scaler)
        : x(scaler), y(scaler) , z(scaler)  {}


    inline CPepeEngineVector3(const CPepeEngineVector3& rkVector)
        : x(rkVector.x), y(rkVector.y), z(rkVector.z) {}

    inline float operator[](const size_t i) const {
        assert(i < 3);

        return *(&x + i);
    }

    inline float& operator[](const size_t i) {
        assert(i < 3);

        return *(&x + i);
    }

    /**
     *  Pointer accessor for direct copying
     */
    inline float* ptr() {
        return &x;
    }

    /**
     *  Pointer accessor for direct copying
     */
    inline const float* ptr() const {
        return &x;
    }

    /**
     *  Assigns the value of the other vector.
     *
     *  @param rkVector The other vector
     */
    inline CPepeEngineVector3& operator=(const CPepeEngineVector3& rkVector) {
        x = rkVector.x;
        y = rkVector.y;
        z = rkVector.z;

        return *this;
    }

    inline CPepeEngineVector3& operator=(const float fScaler) {
        x = fScaler;
        y = fScaler;
        z = fScaler;

        return *this;
    }

    inline bool operator==(const CPepeEngineVector3& rkVector) const {
        return (x == rkVector.x && y == rkVector.y && z == rkVector.z);
    }

    inline bool operator!=(const CPepeEngineVector3& rkVector) const {
        return (x != rkVector.x || y != rkVector.y || z != rkVector.z);
    }

    /**
     *  @ {Arithmetic operations
     */
    inline CPepeEngineVector3 operator+(const CPepeEngineVector3& rkVector) const {
        return CPepeEngineVector3(x + rkVector.x, y + rkVector.y, z + rkVector.z);
    }

    inline CPepeEngineVector3 operator-(const CPepeEngineVector3& rkVector) const {
        return CPepeEngineVector3(x - rkVector.x, y - rkVector.y, z - rkVector.z);
    }

    inline CPepeEngineVector3 operator*(const float fScalar) const {
        return CPepeEngineVector3(x * fScalar, y * fScalar, z * fScalar);
    }

    inline CPepeEngineVector3 operator*(const CPepeEngineVector3& rhs) const {
        return CPepeEngineVector3(x * rhs.x, y * rhs.y, z * rhs.z);
    }

    inline CPepeEngineVector3 operator/(const float fScalar) const {
        assert(fScalar != 0.0);

        float fInv = 1.0f / fScalar;

        return CPepeEngineVector3(x * fInv, y * fInv, z * fInv);
    }

    inline CPepeEngineVector3 operator/(const CPepeEngineVector3& rhs) const {
        return CPepeEngineVector3(x / rhs.x, y / rhs.y, z / rhs.z);
    }

    inline const CPepeEngineVector3& operator+() const {
        return *this;
    }

    inline CPepeEngineVector3 operator-() const {
        return CPepeEngineVector3(-x, -y, -z);
    }
    /** @} */

    /**
     *  @{ Overloaded operators to help CPepeEngineVector3
     */
    _PepeEngineExport inline friend CPepeEngineVector3 operator*(const float fScalar, const CPepeEngineVector3& rkVector) {
        return CPepeEngineVector3(fScalar * rkVector.x, fScalar * rkVector.y, fScalar * rkVector.z);
    }

    _PepeEngineExport inline friend CPepeEngineVector3 operator/(const float fScalar, const CPepeEngineVector3& rkVector) {
        return CPepeEngineVector3(fScalar / rkVector.x, fScalar / rkVector.y, fScalar / rkVector.z);
    }

    _PepeEngineExport inline friend CPepeEngineVector3 operator+(const CPepeEngineVector3& lhs, const float rhs) {
        return CPepeEngineVector3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
    }

    _PepeEngineExport inline friend CPepeEngineVector3 operator+(const float lhs, const CPepeEngineVector3& rhs) {
        return CPepeEngineVector3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
    }

    _PepeEngineExport inline friend CPepeEngineVector3 operator-(const CPepeEngineVector3& lhs, const float rhs) {
        return CPepeEngineVector3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
    }

    _PepeEngineExport inline friend CPepeEngineVector3 operator-(const float lhs, const CPepeEngineVector3& rhs) {
        return CPepeEngineVector3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
    }
    /** @} */

    /**
     *  @{ Arithmetic updates
     */
    inline CPepeEngineVector3& operator+=(const CPepeEngineVector3& rkVector) {
        x += rkVector.x;
        y += rkVector.y;
        z += rkVector.z;

        return *this;
    }

    inline CPepeEngineVector3& operator+=(const float fScalar) {
        x += fScalar;
        y += fScalar;
        z += fScalar;
        return *this;
    }

    inline CPepeEngineVector3& operator-=(const CPepeEngineVector3& rkVector) {
        x -= rkVector.x;
        y -= rkVector.y;
        z -= rkVector.z;

        return *this;
    }

    inline CPepeEngineVector3& operator-=(const float fScalar) {
        x -= fScalar;
        y -= fScalar;
        z -= fScalar;
        return *this;
    }

    inline CPepeEngineVector3& operator*=(const float fScalar) {
        x *= fScalar;
        y *= fScalar;
        z *= fScalar;
        return *this;
    }

    inline CPepeEngineVector3& operator*=(const CPepeEngineVector3& rkVector) {
        x *= rkVector.x;
        y *= rkVector.y;
        z *= rkVector.z;

        return *this;
    }

    inline CPepeEngineVector3& operator/=(const float fScalar) {
        assert(fScalar != 0.0);

        float fInv = 1.0f / fScalar;

        x *= fInv;
        y *= fInv;
        z *= fInv;

        return *this;
    }

    inline CPepeEngineVector3& operator/=(const CPepeEngineVector3& rkVector) {
        x /= rkVector.x;
        y /= rkVector.y;
        z /= rkVector.z;

        return *this;
    }
    /** @} */


    /**
     *  Returns the length (magnitude) of the vector.
     */
    inline float length() const;

    /**
     *  Returns the square of the length(magnitude) of the vector.
     */
    inline float squaredLength() const {
        return x * x + y * y + z * z;
    }

    /**
     *  Returns the distance to another vector.
     */
    inline float distance(const CPepeEngineVector3& rhs) const {
        return (*this - rhs).length();
    }

    /**
     *  Returns the square of the distance to another vector.
     */
    inline float squaredDistance(const CPepeEngineVector3& rhs) const {
        return (*this - rhs).squaredLength();
    }

    /**
     *  Calculates the dot (scalar) product of this vector with another.
     *
     *  The dot product can be used to calculate the angle between 2
     *  vectors. If both are unit vectors, the dot product is the
     *  cosine of the angle; otherwise the dot product must be
     *  divided by the product of the lengths of both vectors to get
     *  the cosine of the angle. This result can further be used to
     *  calculate the distance of a point from a plane.
     *
     *  @param vec Vector with which to calculate the dot product (together with this one).
     *  @returns A float representing the dot product value.
     */
    inline float dotProduct(const CPepeEngineVector3& vec) const {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    /**
     *  Calculates the absolute dot (scalar) product of this vector with another.
     *
     *  This function work similar dotProduct, except it use absolute value
     *  of each component of the vector to computing.
     *
     *  @param vec Vector with which to calculate the absolute dot product (together with this one).
     *  @returns A float representing the absolute dot product value.
     */
    inline float absDotProduct(const CPepeEngineVector3& vec) const;

    /**
     *  Normalises the vector.
     *
     *  This method normalises the vector such that it's
     *  length / magnitude is 1. The result is called a unit vector.
     *
     *  This function will not crash for zero-sized vectors, but there
     *  will be no changes made to their components.
     *  @returns The previous length of the vector.
    */
    inline float normalise();

    /**
     *  Calculates the cross-product of 2 vectors, i.e. the vector that
     *  lies perpendicular to them both.
     *
     *  The cross-product is normally used to calculate the normal
     *  vector of a plane, by calculating the cross-product of 2
     *  non-equivalent vectors which lie on the plane (e.g. 2 edges of a triangle).
     *
     *  @param vec Vector which, together with this one, will be used to calculate the cross-product.
     *  @returns    A vector which is the result of the cross-product. This
     *  vector will <b>NOT</b> be normalised, to maximise efficiency
     *  call CPepeEngineVector3::normalise on the result if you wish this to
     *  be done. As for which side the resultant vector will be on, the
     *  returned vector will be on the side from which the arc from 'this'
     *  to rkVector is anticlockwise, e.g. UNIT_Y.crossProduct(UNIT_Z)
     *  = UNIT_X, whilst UNIT_Z.crossProduct(UNIT_Y) = -UNIT_X.
     */
    inline CPepeEngineVector3 crossProduct(const CPepeEngineVector3& rkVector) const {
        return CPepeEngineVector3(y * rkVector.z - z * rkVector.y, z * rkVector.x - x * rkVector.z, x * rkVector.y - y * rkVector.x);
    }

    /**
     *  Returns a vector at a point half way between this and the passed in vector.
     */
    inline CPepeEngineVector3 midPoint(const CPepeEngineVector3& vec) const {
        return CPepeEngineVector3((x + vec.x) * 0.5f, (y + vec.y) * 0.5f, (z + vec.z) * 0.5f);
    }

    /**
     *  Returns true if the vector's scalar components are all greater
     *  that the ones of the vector it is compared against.
     */
    inline bool operator<(const CPepeEngineVector3& rhs) const {
        if(x < rhs.x && y < rhs.y && z < rhs.z) {
            return true;
        }

        return false;
    }

    /**
     *  Returns true if the vector's scalar components are all smaller
     *  that the ones of the vector it is compared against.
     */
    inline bool operator>(const CPepeEngineVector3& rhs) const {
        if(x > rhs.x && y > rhs.y && z > rhs.z) {
            return true;
        }

        return false;
    }

    /**
     *  Sets this vector's components to the minimum of its own and the ones of the passed in vector.
     *
     *  'Minimum' in this case means the combination of the lowest
     *  value of x, y and z from both vectors. Lowest is taken just
     *  numerically, not magnitude, so -1 < 0.
     */
    inline void makeFloor(const CPepeEngineVector3& cmp) {
        if(cmp.x < x) {
            x = cmp.x;
        }

        if(cmp.y < y) {
            y = cmp.y;
        }

        if(cmp.z < z) {
            z = cmp.z;
        }
    }

    /**
     *  Sets this vector's components to the maximum of its own and the ones of the passed in vector.
     *
     *  'Maximum' in this case means the combination of the highest
     *  value of x, y and z from both vectors. Highest is taken just
     *  numerically, not magnitude, so 1 > -3.
     */
    inline void makeCeil(const CPepeEngineVector3& cmp) {
        if(cmp.x > x) {
            x = cmp.x;
        }

        if(cmp.y > y) {
            y = cmp.y;
        }

        if(cmp.z > z) {
            z = cmp.z;
        }
    }

    /**
     *  Generates a vector perpendicular to this vector (eg an 'up' vector).
     *
     *  This method will return a vector which is perpendicular to this
     *  vector. There are an infinite number of possibilities but this
     *  method will guarantee to generate one of them. If you need more
     *  control you should use the Quaternion class.
     */
    inline CPepeEngineVector3 perpendicular(void) const;

    /**
     *  Generates a new random vector which deviates from this vector by a
     *  given angle in a random direction.
     *
     *  This method assumes that the random number generator has already
     *  been seeded appropriately.
     *
     *  @param angle The angle at which to deviate
     *  @param up Any vector perpendicular to this one (which could generated
     *  by cross-product of this vector and any other non-colinear
     *  vector). If you choose not to provide this the function will
     *  derive one on it's own, however if you provide one yourself the
     *  function will be faster (this allows you to reuse up vectors if
     *  you call this method more than once)
     *  @returns A random vector which deviates from this vector by angle. This
     *  vector will not be normalised, normalise it if you wish
    afterwards.
    */
    inline CPepeEngineVector3 randomDeviant(
        const Radian& angle,
        const CPepeEngineVector3& up = CPepeEngineVector3::ZERO) const;

    inline CPepeEngineVector3 randomDeviant(float angle, const CPepeEngineVector3& up = CPepeEngineVector3::ZERO) const;

    /**
     *  Gets the shortest arc quaternion to rotate this vector to the destination vector.
     *
     *  If you call this with a dest vector that is close to the inverse
     *  of this vector, we will rotate 180 degrees around the 'fallbackAxis'
     *  (if specified, or a generated axis if not) since in this case
     *  ANY axis of rotation is valid.
     */
    CPepeEngineQuaternion getRotationTo(
        const CPepeEngineVector3& dest,
        const CPepeEngineVector3& fallbackAxis = CPepeEngineVector3::ZERO) const;

    /**
     *  Returns true if this vector is zero length.
     */
    inline bool isZeroLength(void) const {
        float sqlen = (x * x) + (y * y) + (z * z);
        return (sqlen < (1e-06 * 1e-06));

    }

    /**
     *  As normalise, except that this vector is unaffected and the
     *  normalised vector is returned as a copy.
     */
    inline CPepeEngineVector3 normalisedCopy(void) const {
        CPepeEngineVector3 ret = *this;
        ret.normalise();
        return ret;
    }

    /**
     *  Calculates a reflection vector to the plane with the given normal .
     */
    inline CPepeEngineVector3 reflect(const CPepeEngineVector3& normal) const {
        return CPepeEngineVector3(*this - (2 * this->dotProduct(normal) * normal));
    }

    /**
     *  Returns whether this vector is within a positional tolerance
     *  of another vector.
     *
     *  @param tolerance The amount that each element of the vector may vary by
     *  and still be considered equal
     */
    inline bool positionEquals(const CPepeEngineVector3& rhs, float tolerance = 1e-03) const;

    /**
     *  Returns whether this vector is within a positional tolerance
     *  of another vector, also take scale of the vectors into account.
     *
     *  @param rhs The vector to compare with
     *  @param tolerance The amount (related to the scale of vectors) that distance
     *  of the vector may vary by and still be considered close
     */
    inline bool positionCloses(const CPepeEngineVector3& rhs, float tolerance = 1e-03f) const {
        return squaredDistance(rhs) <= (squaredLength() + rhs.squaredLength()) * tolerance;
    }

    /**
     *  Returns whether this vector is within a directional tolerance
     *  of another vector.
     *
     *  @param rhs The vector to compare with
     *  @param tolerance The maximum angle by which the vectors may vary and still be considered equal
    */
    inline bool directionEquals(const CPepeEngineVector3& rhs, const Radian& tolerance) const;

};

_PEPE_ENGINE_END

#endif