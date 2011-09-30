/**
 * Project: PepeEngine
 * Tier:	Frontend
 * File:	CPepeEngineVector4.h   
 *
 * @brief	Declaration of CPepeEngineVector4 class.
 *
 *	Original free version by:
 *	Magic Software, Inc.
 *	http://www.geometrictools.com/
 *	Copyright (c) 2000, All Rights Reserved
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-09-22
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEVECTOR4_H
#define CPEPEENGINEVECTOR4_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineVector4
{
public:
	float x, y, z, w;
	
	static const CPepeEngineVector4 ZERO;

	inline CPepeEngineVector4()
	{
	}

	inline CPepeEngineVector4 (const float fX, const float fY, const float fZ, const float fW)
		: x(fX), y(fY), z(fZ), w(fW)
	{
	}

	inline explicit CPepeEngineVector4(const float afCoordinate[4])
		: x(afCoordinate[0]),
		y(afCoordinate[1]),
		z(afCoordinate[2]),
		w(afCoordinate[3])
	{
	}

	inline explicit CPepeEngineVector4(const int afCoordinate[4])
	{
		x = (float)afCoordinate[0];
		y = (float)afCoordinate[1];
		z = (float)afCoordinate[2];
		w = (float)afCoordinate[3];
	}

	inline explicit CPepeEngineVector4(float* const r)
		: x(r[0]), y(r[1]), z(r[2]), w(r[3])
	{
	}

	inline explicit CPepeEngineVector4(const float scaler)
		: x(scaler)
		, y(scaler)
		, z(scaler)
		, w(scaler)
	{
	}

	inline explicit CPepeEngineVector4(const CPepeEngineVector3& rhs)
		: x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
	{
	}

	inline CPepeEngineVector4(const CPepeEngineVector4& rkVector)
		: x(rkVector.x), y(rkVector.y), z(rkVector.z), w (rkVector.w)
	{
	}

	inline float operator [] (const size_t i) const
	{
		assert(i < 4);

		return *(&x+i);
	}

	inline float& operator [] (const size_t i)
	{
		assert(i < 4);

		return *(&x+i);
	}

	/**
	 *	Pointer accessor for direct copying
	 */
	inline float* ptr()
	{
		return &x;
	}

	/**
	 *	Pointer accessor for direct copying
	 */
	inline const float* ptr() const
	{
		return &x;
	}

	/** 
	 *	Assigns the value of the other CPepeEngineVector.
	 *	@param rkVector The other CPepeEngineVector
	 */
	inline CPepeEngineVector4& operator = (const CPepeEngineVector4& rkVector)
	{
		x = rkVector.x;
		y = rkVector.y;
		z = rkVector.z;
		w = rkVector.w;

		return *this;
	}

	inline CPepeEngineVector4& operator = (const float fScalar)
	{
		x = fScalar;
		y = fScalar;
		z = fScalar;
		w = fScalar;
		return *this;
	}

	inline bool operator == (const CPepeEngineVector4& rkVector) const
	{
		return (x == rkVector.x &&
			y == rkVector.y &&
			z == rkVector.z &&
			w == rkVector.w);
	}

	inline bool operator != (const CPepeEngineVector4& rkVector) const
	{
		return (x != rkVector.x ||
			y != rkVector.y ||
			z != rkVector.z ||
			w != rkVector.w);
	}

	inline CPepeEngineVector4& operator = (const CPepeEngineVector3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = 1.0f;
		return *this;
	}

	/**	@{ Arithmetic operations */
	inline CPepeEngineVector4 operator + (const CPepeEngineVector4& rkVector) const
	{
		return CPepeEngineVector4(
			x + rkVector.x,
			y + rkVector.y,
			z + rkVector.z,
			w + rkVector.w);
	}

	inline CPepeEngineVector4 operator - (const CPepeEngineVector4& rkVector) const
	{
		return CPepeEngineVector4(
			x - rkVector.x,
			y - rkVector.y,
			z - rkVector.z,
			w - rkVector.w);
	}

	inline CPepeEngineVector4 operator * (const float fScalar) const
	{
		return CPepeEngineVector4(
			x * fScalar,
			y * fScalar,
			z * fScalar,
			w * fScalar);
	}

	inline CPepeEngineVector4 operator * (const CPepeEngineVector4& rhs) const
	{
		return CPepeEngineVector4(
			rhs.x * x,
			rhs.y * y,
			rhs.z * z,
			rhs.w * w);
	}

	inline CPepeEngineVector4 operator / (const float fScalar) const
	{
		assert(fScalar != 0.0f);

		float fInv = 1.0f / fScalar;

		return CPepeEngineVector4(
			x * fInv,
			y * fInv,
			z * fInv,
			w * fInv);
	}

	inline CPepeEngineVector4 operator / (const CPepeEngineVector4& rhs) const
	{
		return CPepeEngineVector4(
			x / rhs.x,
			y / rhs.y,
			z / rhs.z,
			w / rhs.w);
	}

	inline const CPepeEngineVector4& operator + () const
	{
		return *this;
	}

	inline CPepeEngineVector4 operator - () const
	{
		return CPepeEngineVector4(-x, -y, -z, -w);
	}

	_PepeEngineExport inline friend CPepeEngineVector4 operator * (const float fScalar, const CPepeEngineVector4& rkVector)
	{
		return CPepeEngineVector4(
			fScalar * rkVector.x,
			fScalar * rkVector.y,
			fScalar * rkVector.z,
			fScalar * rkVector.w);
	}

	_PepeEngineExport inline friend CPepeEngineVector4 operator / (const float fScalar, const CPepeEngineVector4& rkVector)
	{
		return CPepeEngineVector4(
			fScalar / rkVector.x,
			fScalar / rkVector.y,
			fScalar / rkVector.z,
			fScalar / rkVector.w);
	}

	_PepeEngineExport inline friend CPepeEngineVector4 operator + (const CPepeEngineVector4& lhs, const float rhs)
	{
		return CPepeEngineVector4(
			lhs.x + rhs,
			lhs.y + rhs,
			lhs.z + rhs,
			lhs.w + rhs);
	}

	_PepeEngineExport inline friend CPepeEngineVector4 operator + (const float lhs, const CPepeEngineVector4& rhs)
	{
		return CPepeEngineVector4(
			lhs + rhs.x,
			lhs + rhs.y,
			lhs + rhs.z,
			lhs + rhs.w);
	}

	_PepeEngineExport inline friend CPepeEngineVector4 operator - (const CPepeEngineVector4& lhs, float rhs)
	{
		return CPepeEngineVector4(
			lhs.x - rhs,
			lhs.y - rhs,
			lhs.z - rhs,
			lhs.w - rhs);
	}

	_PepeEngineExport inline friend CPepeEngineVector4 operator - (const float lhs, const CPepeEngineVector4& rhs)
	{
		return CPepeEngineVector4(
			lhs - rhs.x,
			lhs - rhs.y,
			lhs - rhs.z,
			lhs - rhs.w);
	}
	/**	@} */

	/**	@{ Arithmetic updates */
	inline CPepeEngineVector4& operator += (const CPepeEngineVector4& rkVector)
	{
		x += rkVector.x;
		y += rkVector.y;
		z += rkVector.z;
		w += rkVector.w;

		return *this;
	}

	inline CPepeEngineVector4& operator -= (const CPepeEngineVector4& rkVector)
	{
		x -= rkVector.x;
		y -= rkVector.y;
		z -= rkVector.z;
		w -= rkVector.w;

		return *this;
	}

	inline CPepeEngineVector4& operator *= (const float fScalar)
	{
		x *= fScalar;
		y *= fScalar;
		z *= fScalar;
		w *= fScalar;
		return *this;
	}

	inline CPepeEngineVector4& operator += (const float fScalar)
	{
		x += fScalar;
		y += fScalar;
		z += fScalar;
		w += fScalar;
		return *this;
	}

	inline CPepeEngineVector4& operator -= (const float fScalar)
	{
		x -= fScalar;
		y -= fScalar;
		z -= fScalar;
		w -= fScalar;
		return *this;
	}

	inline CPepeEngineVector4& operator *= (const CPepeEngineVector4& rkVector)
	{
		x *= rkVector.x;
		y *= rkVector.y;
		z *= rkVector.z;
		w *= rkVector.w;

		return *this;
	}

	inline CPepeEngineVector4& operator /= (const float fScalar)
	{
		assert(fScalar != 0.0f);

		float fInv = 1.0f / fScalar;

		x *= fInv;
		y *= fInv;
		z *= fInv;
		w *= fInv;

		return *this;
	}

	inline CPepeEngineVector4& operator /= (const CPepeEngineVector4& rkVector)
	{
		x /= rkVector.x;
		y /= rkVector.y;
		z /= rkVector.z;
		w /= rkVector.w;

		return *this;
	}
	/**	@} */

	/** 
	 *	Calculates the dot (scalar) product of this CPepeEngineVector with another.
	 *	@param	vec CPepeEngineVector with which to calculate the dot product (together with this one).
	 *	@return A float representing the dot product value.
	 */
	inline float dotProduct(const CPepeEngineVector4& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
	}	

};

_PEPE_ENGINE_END

#endif