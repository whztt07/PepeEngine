/**
 * Project: PepeEngine
 * Tier:    Frontend
 * File:    CPepeEngineMatrix4.h
 *
 * @brief   Declaration of CPepeEngineMatrix4 class.
 *
 *  Original free version by:
 *  Magic Software, Inc.
 *  http://www.geometrictools.com/
 *  Copyright (c) 2000, All Rights Reserved
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-07-22
 * @copyright   Copyright(c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEMATRIX4_H
#define CPEPEENGINEMATRIX4_H

#include "PepeEngineInit.h"

#include "CPepeEngineVector3.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineMatrix4
{
protected:
    /**
     *  The matrix entries, indexed by [row][col].
     */
    union {
        float m[4][4];
        float _m[16];
    };
public:

    friend CPepeEngineXFileMeshLoader;

    static const CPepeEngineMatrix4 ZERO;
    static const CPepeEngineMatrix4 IDENTITY;
    /**
     *  Useful little matrix which takes 2D clipspace {-1, 1} to {0,1}
     *  and inverts the Y.
     */
    static const CPepeEngineMatrix4 CLIPSPACE2DTOIMAGESPACE;

    inline CPepeEngineMatrix4() {
    }

    inline CPepeEngineMatrix4(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33) {
        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[0][3] = m03;
        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[1][3] = m13;
        m[2][0] = m20;
        m[2][1] = m21;
        m[2][2] = m22;
        m[2][3] = m23;
        m[3][0] = m30;
        m[3][1] = m31;
        m[3][2] = m32;
        m[3][3] = m33;
    }

    /**
     *  Creates a standard 4x4 transformation matrix with a zero translation part from a rotation/scaling 3x3 matrix.
     */
    inline CPepeEngineMatrix4(const CPepeEngineMatrix3& m3x3);

    /**
     *  Creates a standard 4x4 transformation matrix with a zero translation part from a rotation/scaling CPepeEngineQuaternion.
     */
    inline CPepeEngineMatrix4(const CPepeEngineQuaternion& rot);

    inline float* operator[](size_t iRow) {
        assert(iRow < 4);
        return m[iRow];
    }

    inline const float *const operator[](size_t iRow) const {
        assert(iRow < 4);
        return m[iRow];
    }

    inline CPepeEngineMatrix4 concatenate(const CPepeEngineMatrix4 &m2) const {
        CPepeEngineMatrix4 r;
        r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
        r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
        r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
        r.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

        r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
        r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
        r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
        r.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

        r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
        r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
        r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
        r.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

        r.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
        r.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
        r.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
        r.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];

        return r;
    }

    /**
     *  Matrix concatenation using '*'.
     */
    inline CPepeEngineMatrix4 operator*(const CPepeEngineMatrix4 &m2) const {
        return concatenate(m2);
    }

    /**
     *  Vector transformation using '*'.
     *  Transforms the given 3-D vector by the matrix, projecting the
     *  result back into <i>w</i> = 1.
     *  This means that the initial <i>w</i> is considered to be 1.0,
     *  and then all the tree elements of the resulting 3-D vector are
     *  divided by the resulting <i>w</i>.
     */
    inline CPepeEngineVector3 operator*(const CPepeEngineVector3 &v) const {
        CPepeEngineVector3 r;

        float fInvW = 1.0f / (m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3]);

        r.x = (m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3]) * fInvW;
        r.y = (m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3]) * fInvW;
        r.z = (m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]) * fInvW;

        return r;
    }

    /**
     *  Matrix addition.
     */
    inline CPepeEngineMatrix4 operator+(const CPepeEngineMatrix4 &m2) const {
        CPepeEngineMatrix4 r;

        r.m[0][0] = m[0][0] + m2.m[0][0];
        r.m[0][1] = m[0][1] + m2.m[0][1];
        r.m[0][2] = m[0][2] + m2.m[0][2];
        r.m[0][3] = m[0][3] + m2.m[0][3];

        r.m[1][0] = m[1][0] + m2.m[1][0];
        r.m[1][1] = m[1][1] + m2.m[1][1];
        r.m[1][2] = m[1][2] + m2.m[1][2];
        r.m[1][3] = m[1][3] + m2.m[1][3];

        r.m[2][0] = m[2][0] + m2.m[2][0];
        r.m[2][1] = m[2][1] + m2.m[2][1];
        r.m[2][2] = m[2][2] + m2.m[2][2];
        r.m[2][3] = m[2][3] + m2.m[2][3];

        r.m[3][0] = m[3][0] + m2.m[3][0];
        r.m[3][1] = m[3][1] + m2.m[3][1];
        r.m[3][2] = m[3][2] + m2.m[3][2];
        r.m[3][3] = m[3][3] + m2.m[3][3];

        return r;
    }

    /**
     *  Matrix subtraction.
     */
    inline CPepeEngineMatrix4 operator-(const CPepeEngineMatrix4 &m2) const {
        CPepeEngineMatrix4 r;
        r.m[0][0] = m[0][0] - m2.m[0][0];
        r.m[0][1] = m[0][1] - m2.m[0][1];
        r.m[0][2] = m[0][2] - m2.m[0][2];
        r.m[0][3] = m[0][3] - m2.m[0][3];

        r.m[1][0] = m[1][0] - m2.m[1][0];
        r.m[1][1] = m[1][1] - m2.m[1][1];
        r.m[1][2] = m[1][2] - m2.m[1][2];
        r.m[1][3] = m[1][3] - m2.m[1][3];

        r.m[2][0] = m[2][0] - m2.m[2][0];
        r.m[2][1] = m[2][1] - m2.m[2][1];
        r.m[2][2] = m[2][2] - m2.m[2][2];
        r.m[2][3] = m[2][3] - m2.m[2][3];

        r.m[3][0] = m[3][0] - m2.m[3][0];
        r.m[3][1] = m[3][1] - m2.m[3][1];
        r.m[3][2] = m[3][2] - m2.m[3][2];
        r.m[3][3] = m[3][3] - m2.m[3][3];

        return r;
    }

    inline const float* ptr() const {
        return _m;
    }

    /**
     *  Tests 2 matrices for equality.
     */
    inline bool operator==(const CPepeEngineMatrix4& m2) const {
        if (
            m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
            m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
            m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
            m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3]) {
            return false;
        }

        return true;
    }

    /**
     *  Tests 2 matrices for inequality.
     */
    inline bool operator!=(const CPepeEngineMatrix4& m2) const {
        if (
            m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
            m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
            m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
            m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3]) {
            return true;
        }

        return false;
    }

    /**
     *  Assignment from 3x3 matrix.
     */
    inline void operator=(const CPepeEngineMatrix3& mat3);

    inline CPepeEngineMatrix4 transpose(void) const {
        return CPepeEngineMatrix4(
                   m[0][0], m[1][0], m[2][0], m[3][0],
                   m[0][1], m[1][1], m[2][1], m[3][1],
                   m[0][2], m[1][2], m[2][2], m[3][2],
                   m[0][3], m[1][3], m[2][3], m[3][3]
               );
    }

    // ---------  Translation Transformation --------------------------------

    /**
     *  Sets the translation transformation part of the matrix.
     */
    inline void setTrans(const CPepeEngineVector3& v) {
        m[0][3] = v.x;
        m[1][3] = v.y;
        m[2][3] = v.z;
    }

    /**
     *  Extracts the translation transformation part of the matrix.
     */
    inline CPepeEngineVector3 getTrans() const {
        return CPepeEngineVector3(m[0][3], m[1][3], m[2][3]);
    }

    /**
     *  Builds a translation matrix
     */
    inline void makeTrans(const CPepeEngineVector3& v) {
        m[0][0] = 1.0;
        m[0][1] = 0.0;
        m[0][2] = 0.0;
        m[0][3] = v.x;
        m[1][0] = 0.0;
        m[1][1] = 1.0;
        m[1][2] = 0.0;
        m[1][3] = v.y;
        m[2][0] = 0.0;
        m[2][1] = 0.0;
        m[2][2] = 1.0;
        m[2][3] = v.z;
        m[3][0] = 0.0;
        m[3][1] = 0.0;
        m[3][2] = 0.0;
        m[3][3] = 1.0;
    }

    inline void makeTrans(float tx, float ty, float tz) {
        m[0][0] = 1.0;
        m[0][1] = 0.0;
        m[0][2] = 0.0;
        m[0][3] = tx;
        m[1][0] = 0.0;
        m[1][1] = 1.0;
        m[1][2] = 0.0;
        m[1][3] = ty;
        m[2][0] = 0.0;
        m[2][1] = 0.0;
        m[2][2] = 1.0;
        m[2][3] = tz;
        m[3][0] = 0.0;
        m[3][1] = 0.0;
        m[3][2] = 0.0;
        m[3][3] = 1.0;
    }

    /**
     *  Gets a translation matrix.
     */
    inline static CPepeEngineMatrix4 getTrans(const CPepeEngineVector3& v) {
        CPepeEngineMatrix4 r;

        r.m[0][0] = 1.0;
        r.m[0][1] = 0.0;
        r.m[0][2] = 0.0;
        r.m[0][3] = v.x;
        r.m[1][0] = 0.0;
        r.m[1][1] = 1.0;
        r.m[1][2] = 0.0;
        r.m[1][3] = v.y;
        r.m[2][0] = 0.0;
        r.m[2][1] = 0.0;
        r.m[2][2] = 1.0;
        r.m[2][3] = v.z;
        r.m[3][0] = 0.0;
        r.m[3][1] = 0.0;
        r.m[3][2] = 0.0;
        r.m[3][3] = 1.0;

        return r;
    }

    /**
     *  Gets a translation matrix - variation for not using a vector.
     */
    inline static CPepeEngineMatrix4 getTrans(float t_x, float t_y, float t_z) {
        CPepeEngineMatrix4 r;

        r.m[0][0] = 1.0;
        r.m[0][1] = 0.0;
        r.m[0][2] = 0.0;
        r.m[0][3] = t_x;
        r.m[1][0] = 0.0;
        r.m[1][1] = 1.0;
        r.m[1][2] = 0.0;
        r.m[1][3] = t_y;
        r.m[2][0] = 0.0;
        r.m[2][1] = 0.0;
        r.m[2][2] = 1.0;
        r.m[2][3] = t_z;
        r.m[3][0] = 0.0;
        r.m[3][1] = 0.0;
        r.m[3][2] = 0.0;
        r.m[3][3] = 1.0;

        return r;
    }

    // ---------  Scale Transformation ------------------------------------

    /**
     *  Sets the scale part of the matrix.
     */
    inline void setScale(const CPepeEngineVector3& v) {
        m[0][0] = v.x;
        m[1][1] = v.y;
        m[2][2] = v.z;
    }

    /**
     *  Gets a scale matrix.
     */
    inline static CPepeEngineMatrix4 getScale(const CPepeEngineVector3& v) {
        CPepeEngineMatrix4 r;
        r.m[0][0] = v.x;
        r.m[0][1] = 0.0;
        r.m[0][2] = 0.0;
        r.m[0][3] = 0.0;
        r.m[1][0] = 0.0;
        r.m[1][1] = v.y;
        r.m[1][2] = 0.0;
        r.m[1][3] = 0.0;
        r.m[2][0] = 0.0;
        r.m[2][1] = 0.0;
        r.m[2][2] = v.z;
        r.m[2][3] = 0.0;
        r.m[3][0] = 0.0;
        r.m[3][1] = 0.0;
        r.m[3][2] = 0.0;
        r.m[3][3] = 1.0;

        return r;
    }

    /**
     *  Gets a scale matrix - variation for not using a vector.
     */
    inline static CPepeEngineMatrix4 getScale(float s_x, float s_y, float s_z) {
        CPepeEngineMatrix4 r;
        r.m[0][0] = s_x;
        r.m[0][1] = 0.0;
        r.m[0][2] = 0.0;
        r.m[0][3] = 0.0;
        r.m[1][0] = 0.0;
        r.m[1][1] = s_y;
        r.m[1][2] = 0.0;
        r.m[1][3] = 0.0;
        r.m[2][0] = 0.0;
        r.m[2][1] = 0.0;
        r.m[2][2] = s_z;
        r.m[2][3] = 0.0;
        r.m[3][0] = 0.0;
        r.m[3][1] = 0.0;
        r.m[3][2] = 0.0;
        r.m[3][3] = 1.0;

        return r;
    }

    /**
     *  Extracts the rotation / scaling part of the Matrix as a 3x3 matrix.
     *  @param m3x3 Destination CPepeEngineMatrix3
     */
    inline void extract3x3Matrix(CPepeEngineMatrix3& m3x3) const;

    /**
     *  Extracts the rotation / scaling part as a quaternion from the Matrix.
     */
    inline CPepeEngineQuaternion extractQuaternion() const;

    inline CPepeEngineMatrix4 operator*(float scalar) const {
        return CPepeEngineMatrix4(
                   scalar * m[0][0], scalar * m[0][1], scalar * m[0][2], scalar * m[0][3],
                   scalar * m[1][0], scalar * m[1][1], scalar * m[1][2], scalar * m[1][3],
                   scalar * m[2][0], scalar * m[2][1], scalar * m[2][2], scalar * m[2][3],
                   scalar * m[3][0], scalar * m[3][1], scalar * m[3][2], scalar * m[3][3]);
    }

    CPepeEngineMatrix4 adjoint() const;
    float determinant() const;
    CPepeEngineMatrix4 inverse() const;

    /**
     *  Building a CPepeEngineMatrix4 from orientation / scale / position.
     *  Transform is performed in the order scale, rotate, translation, i.e. translation is independent
     *  of orientation axes, scale does not affect size of translation, rotation and scaling are always
     *  centered on the origin.
     */
    void makeTransform(const CPepeEngineVector3& position, const CPepeEngineVector3& scale, const CPepeEngineQuaternion& orientation);

    /**
     *  Building an inverse CPepeEngineMatrix4 from orientation / scale / position.
     *  As makeTransform except it build the inverse given the same data as makeTransform, so
     *  performing -translation, -rotate, 1/scale in that order.
     */
    void makeInverseTransform(const CPepeEngineVector3& position, const CPepeEngineVector3& scale, const CPepeEngineQuaternion& orientation);

    /**
     *  Check whether or not the matrix is affine matrix.
     *  An affine matrix is a 4x4 matrix with row 3 equal to(0, 0, 0, 1),
     *  e.g. no projective coefficients.
     */
    inline bool isAffine(void) const {
        return m[3][0] == 0 && m[3][1] == 0 && m[3][2] == 0 && m[3][3] == 1;
    }

    /**
     *  Returns the inverse of the affine matrix.
     *  The matrix must be an affine matrix.
     *  @see CPepeEngineMatrix4::isAffine.
     */
    CPepeEngineMatrix4 inverseAffine(void) const;

    /**
     *  Concatenate two affine matrix.
     *  The matrices must be affine matrix.
     *  @see CPepeEngineMatrix4::isAffine.
     */
    inline CPepeEngineMatrix4 concatenateAffine(const CPepeEngineMatrix4 &m2) const {
        assert(isAffine() && m2.isAffine());

        return CPepeEngineMatrix4(
                   m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0],
                   m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1],
                   m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2],
                   m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3],

                   m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0],
                   m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1],
                   m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2],
                   m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3],

                   m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0],
                   m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1],
                   m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2],
                   m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3],

                   0, 0, 0, 1);
    }

    /**
     *  3-D Vector transformation specially for affine matrix.
     *  Transforms the given 3-D vector by the matrix, projecting the
     *  result back into <i>w</i> = 1.
     *
     *  The matrix must be an affine matrix.
     *  @see CPepeEngineMatrix4::isAffine.
     */
    inline CPepeEngineVector3 transformAffine(const CPepeEngineVector3& v) const {
        assert(isAffine());

        return CPepeEngineVector3(
                   m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3],
                   m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
                   m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]);
    }

};

_PEPE_ENGINE_END

#endif