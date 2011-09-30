/**
* Project:	PepeEngine
* Tier:		Frontend
* File:		CPepeEngineMatrix3.h   
*
* @brief	Declaration of CPepeEngineMatrix3 class.
*
*	Original free version by:
*	Magic Software, Inc.
*	http://www.geometrictools.com/
*	Copyright (c) 2000, All Rights Reserved
*
* @author		Piotr 'pepe' Picheta
* @date			2008-07-22
* @copyright	Copyright (c) 2008 Piotr Picheta
*
* @version 1.0
*/

#ifndef CPEPEENGINEMATRIX3_H
#define CPEPEENGINEMATRIX3_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineMatrix3
{	
protected:

	float m[3][3];	

	friend class CPepeEngineMatrix4;

	/** 
	 *	Support for singular value decomposition
	 */
	static const float			ms_fSvdEpsilon;
	static const unsigned int	ms_iSvdMaxIterations;

	/** 
	 *	Support for eigensolver
	 */
	void tridiagonal(float afDiag[3], float afSubDiag[3]);
	bool QLAlgorithm(float afDiag[3], float afSubDiag[3]);

	static void bidiagonalize(CPepeEngineMatrix3& kA, CPepeEngineMatrix3& kL, CPepeEngineMatrix3& kR);
	static void golubKahanStep(CPepeEngineMatrix3& kA, CPepeEngineMatrix3& kL,	CPepeEngineMatrix3& kR);

	/** 
	 *	Support for spectral norm
	 */
	static float maxCubicRoot(float afCoeff[3]);
public:

	static const double				EPSILON;
	static const CPepeEngineMatrix3	ZERO;
	static const CPepeEngineMatrix3	IDENTITY;

	inline CPepeEngineMatrix3 () {}

	inline explicit CPepeEngineMatrix3(const float arr[3][3])
	{
		memcpy(m, arr, 9 * sizeof(float));
	}

	inline CPepeEngineMatrix3(const CPepeEngineMatrix3& rkMatrix)
	{
		memcpy(m, rkMatrix.m, 9 * sizeof(float));
	}

	CPepeEngineMatrix3(
		float fEntry00, float fEntry01, float fEntry02,
		float fEntry10, float fEntry11, float fEntry12,
		float fEntry20, float fEntry21, float fEntry22
	)
	{
		m[0][0] = fEntry00;
		m[0][1] = fEntry01;
		m[0][2] = fEntry02;
		m[1][0] = fEntry10;
		m[1][1] = fEntry11;
		m[1][2] = fEntry12;
		m[2][0] = fEntry20;
		m[2][1] = fEntry21;
		m[2][2] = fEntry22;
	}

	/**
	 *	Member access, allows use of construct mat[r][c]
	 */
	inline float* operator[](size_t iRow) const
	{
		return (float*)m[iRow];
	}
	
	CPepeEngineVector3 getColumn(size_t iCol) const;
	void setColumn(size_t iCol, const CPepeEngineVector3& vec);
	void fromAxes(const CPepeEngineVector3& xAxis, const CPepeEngineVector3& yAxis, const CPepeEngineVector3& zAxis);

	/** 
	 *	Assignment and comparison
	 */
	inline CPepeEngineMatrix3& operator=(const CPepeEngineMatrix3& rkMatrix)
	{
		memcpy(m, rkMatrix.m, 9 * sizeof(float));
		return *this;
	}

	bool operator==(const CPepeEngineMatrix3& rkMatrix) const;
	inline bool operator!=(const CPepeEngineMatrix3& rkMatrix) const
	{
		return !operator==(rkMatrix);
	}

	/** @{ Arithmetic operations */
	CPepeEngineMatrix3 operator+(const CPepeEngineMatrix3& rkMatrix) const;
	CPepeEngineMatrix3 operator-(const CPepeEngineMatrix3& rkMatrix) const;
	CPepeEngineMatrix3 operator*(const CPepeEngineMatrix3& rkMatrix) const;
	CPepeEngineMatrix3 operator-() const;
	/** @} */

	/** 
	 *	matrix * vector [3x3 * 3x1 = 3x1]
	 */
	CPepeEngineVector3 operator*(const CPepeEngineVector3& rkVector) const;

	/** 
	 *	vector * matrix [1x3 * 3x3 = 1x3]	
	 */
	_PepeEngineExport friend CPepeEngineVector3 operator*(const CPepeEngineVector3& rkVector, const CPepeEngineMatrix3& rkMatrix)
	{
		CPepeEngineVector3 kProd;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			kProd[iRow] =
				rkVector.x * rkMatrix.m[0][iRow] +
				rkVector.y * rkMatrix.m[1][iRow] +
				rkVector.z * rkMatrix.m[2][iRow];
		}
		return kProd;
	}

	/** 
	 *	matrix * scalar
	 */
	CPepeEngineMatrix3 operator*(float fScalar) const;

	/** 
	 *	scalar * matrix
	 */
	_PepeEngineExport friend CPepeEngineMatrix3 operator*(float fScalar, const CPepeEngineMatrix3& rkMatrix)
	{
		CPepeEngineMatrix3 kProd;
		for (size_t iRow = 0; iRow < 3; iRow++)
		{
			for (size_t iCol = 0; iCol < 3; iCol++)
			{
				kProd[iRow][iCol] = fScalar * rkMatrix.m[iRow][iCol];
			}
		}
		return kProd;
	}

	/** 
	 *	@{utilities
	 */
	CPepeEngineMatrix3 transpose() const;
	bool inverse(CPepeEngineMatrix3& rkInverse, float fTolerance = 1e-06) const;
	CPepeEngineMatrix3 inverse(float fTolerance = 1e-06) const;
	float determinant() const;
	/** @} */
   
	void singularValueDecomposition(CPepeEngineMatrix3& kL, CPepeEngineVector3& kS, CPepeEngineMatrix3& kR) const;
	void singularValueComposition(const CPepeEngineMatrix3& kL, const CPepeEngineVector3& kS, const CPepeEngineMatrix3& kR);

	/** 
	 *	Gram-Schmidt orthonormalization (applied to columns of rotation matrix)
	 */
    void orthonormalize();

	/** 
	 *	orthogonal Q, diagonal D, upper triangular U stored as (u01, u02, u12)
	 */
	void QDUDecomposition(CPepeEngineMatrix3& rkQ, CPepeEngineVector3& rkD, CPepeEngineVector3& rkU) const;

	float spectralNorm() const;

	/** 
	 *	Matrix must be orthonormal
	 */
	void toAxisAngle(CPepeEngineVector3& rkAxis, Radian& rfAngle) const;

	inline void toAxisAngle(CPepeEngineVector3& rkAxis, Degree& rfAngle) const;

	void fromAxisAngle(const CPepeEngineVector3& rkAxis, const Radian& fRadians);

	inline void toAxisAngle (CPepeEngineVector3& rkAxis, float& rfRadians) const;

	inline void fromAxisAngle(const CPepeEngineVector3& rkAxis, float fRadians);

	/** 
	 *	The matrix must be orthonormal.  The decomposition is yaw*pitch*roll
	 *	where yaw is rotation about the Up vector, pitch is rotation about the
	 *	Right axis, and roll is rotation about the Direction axis.
	 */
	bool toEulerAnglesXYZ(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const;
	bool toEulerAnglesXZY(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const;
	bool toEulerAnglesYXZ(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const;
	bool toEulerAnglesYZX(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const;
	bool toEulerAnglesZXY(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const;
	bool toEulerAnglesZYX(Radian& rfYAngle, Radian& rfPAngle, Radian& rfRAngle) const;
	void fromEulerAnglesXYZ(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
	void fromEulerAnglesXZY(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
	void fromEulerAnglesYXZ(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
	void fromEulerAnglesYZX(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
	void fromEulerAnglesZXY(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);
	void fromEulerAnglesZYX(const Radian& fYAngle, const Radian& fPAngle, const Radian& fRAngle);

	inline bool toEulerAnglesXYZ(float& rfYAngle, float& rfPAngle,	float& rfRAngle) const;
	inline bool toEulerAnglesXZY(float& rfYAngle, float& rfPAngle, float& rfRAngle) const;
	inline bool toEulerAnglesYXZ(float& rfYAngle, float& rfPAngle,	float& rfRAngle) const;
	inline bool toEulerAnglesYZX(float& rfYAngle, float& rfPAngle,	float& rfRAngle) const;
	inline bool toEulerAnglesZXY(float& rfYAngle, float& rfPAngle,	float& rfRAngle) const;
	inline bool toEulerAnglesZYX(float& rfYAngle, float& rfPAngle,	float& rfRAngle) const;
	inline void fromEulerAnglesXYZ(float fYAngle, float fPAngle, float fRAngle);
	inline void fromEulerAnglesXZY(float fYAngle, float fPAngle, float fRAngle);
	inline void fromEulerAnglesYXZ(float fYAngle, float fPAngle, float fRAngle);
	inline void fromEulerAnglesYZX(float fYAngle, float fPAngle, float fRAngle);
	inline void fromEulerAnglesZXY(float fYAngle, float fPAngle, float fRAngle);
	inline void fromEulerAnglesZYX(float fYAngle, float fPAngle, float fRAngle);

	/** 
	 *	Eigensolver, matrix must be symmetric
	 */
	void eigenSolveSymmetric(float afEigenvalue[3],	CPepeEngineVector3 akEigenvector[3]) const;

	static void tensorProduct(const CPepeEngineVector3& rkU, const CPepeEngineVector3& rkV, CPepeEngineMatrix3& rkProduct);	
};

_PEPE_ENGINE_END

#endif