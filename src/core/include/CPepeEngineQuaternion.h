/**
 * Project: PepeEngine
 * Tier:	Frontend
 * File:	CPepeEngineQuaternion.h   
 *
 * @brief	Declaration of CPepeEngineQuaternion class.
 *
 *	Original free version by:
 *	Magic Software, Inc.
 *	http://www.geometrictools.com/
 *	Copyright (c) 2000, All Rights Reserved
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-06-07
 * @copyright   Copyright(c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEQUATERNION_H
#define CPEPEENGINEQUATERNION_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineQuaternion 
{	
public:

	/** 
	 *	Cutoff for sine near zero
	 */
	static const float ms_fEpsilon;

	/** 
	 *	@{ special values
	 */
	static const CPepeEngineQuaternion ZERO;
	static const CPepeEngineQuaternion IDENTITY;
	/** @} */

	float w, x, y, z;

	inline CPepeEngineQuaternion(
		float fW = 1.0,
		float fX = 0.0, float fY = 0.0, float fZ = 0.0)
	{
		w = fW;
		x = fX;
		y = fY;
		z = fZ;
	}

	inline CPepeEngineQuaternion(const CPepeEngineQuaternion& rkQ)
	{
		w = rkQ.w;
		x = rkQ.x;
		y = rkQ.y;
		z = rkQ.z;
	}

	/** 
	 *	Construct a quaternion from a rotation matrix
	 */
	inline CPepeEngineQuaternion(const CPepeEngineMatrix3& rot)
	{
		this->fromRotationMatrix(rot);
	}

	/** 
	 *	Construct a quaternion from an angle/axis
	 */	
	inline CPepeEngineQuaternion(const float& rfAngle, const CPepeEngineVector3& rkAxis)
	{
		this->fromAngleAxis(rfAngle, rkAxis);
	}
	
	/** 
	 *	Construct a quaternion from an angle/axis
	 */
    inline CPepeEngineQuaternion(const Radian& rfAngle, const CPepeEngineVector3& rkAxis)
	{
		this->fromAngleAxis(rfAngle, rkAxis);
	}

	/** 
	 *	Construct a quaternion from 3 orthonormal local axes
	 */
	inline CPepeEngineQuaternion(const CPepeEngineVector3& xaxis, const CPepeEngineVector3& yaxis, const CPepeEngineVector3& zaxis)
	{
		this->fromAxes(xaxis, yaxis, zaxis);
	}

	/** 
	 *	Construct a quaternion from 3 orthonormal local axes
	 */
	inline CPepeEngineQuaternion(const CPepeEngineVector3* akAxis)
	{
		this->fromAxes(akAxis);
	}

	/** 
	 *	Construct a quaternion from 4 manual w/x/y/z values
	 */
	inline CPepeEngineQuaternion(float* valptr)
	{
		memcpy(&w, valptr, sizeof(float) * 4);
	}

	/** 
	 *	Array accessor operator
	 */
	inline float operator[](const size_t i) const
	{
		assert(i < 4);

		return *(&w+i);
	}

	/** 
	 *	Array accessor operator
	 */
	inline float& operator[](const size_t i)
	{
		assert(i < 4);

		return *(&w+i);
	}

	/** 
	 *	Pointer accessor for direct copying
	 */
	inline float* ptr()
	{
		return &w;
	}

	/** 
	 *	Pointer accessor for direct copying
	 */
	inline const float* ptr() const
	{
		return &w;
	}

	void fromRotationMatrix(const CPepeEngineMatrix3& kRot);
	void toRotationMatrix(CPepeEngineMatrix3& kRot) const;
	void fromAngleAxis(const Radian& rfAngle, const CPepeEngineVector3& rkAxis);
	void toAngleAxis(Radian& rfAngle, CPepeEngineVector3& rkAxis) const;

	inline void toAngleAxis(Degree& dAngle, CPepeEngineVector3& rkAxis) const;

	inline void fromAngleAxis(const float& rfAngle, const CPepeEngineVector3& rkAxis);

	void fromAxes(const CPepeEngineVector3* akAxis);
	void fromAxes(const CPepeEngineVector3& xAxis, const CPepeEngineVector3& yAxis, const CPepeEngineVector3& zAxis);
	void toAxes(CPepeEngineVector3* akAxis) const;
	void toAxes(CPepeEngineVector3& xAxis, CPepeEngineVector3& yAxis, CPepeEngineVector3& zAxis) const;
	
	/**
	 *	Get the local x-axis
	 */
	CPepeEngineVector3 xAxis(void) const;

	/** 
	 *	Get the local y-axis
	 */
	CPepeEngineVector3 yAxis(void) const;

	/** 
	 *	Get the local z-axis
	 */
	CPepeEngineVector3 zAxis(void) const;

	inline CPepeEngineQuaternion& operator=(const CPepeEngineQuaternion& rkQ)
	{
		w = rkQ.w;
		x = rkQ.x;
		y = rkQ.y;
		z = rkQ.z;
		return *this;
	}
	CPepeEngineQuaternion operator+(const CPepeEngineQuaternion& rkQ) const;
	CPepeEngineQuaternion operator-(const CPepeEngineQuaternion& rkQ) const;
	CPepeEngineQuaternion operator*(const CPepeEngineQuaternion& rkQ) const;
	CPepeEngineQuaternion operator*(float fScalar) const;
	
	_PepeEngineExport friend CPepeEngineQuaternion operator*(float fScalar, const CPepeEngineQuaternion& rkQ);

	CPepeEngineQuaternion operator-() const;

	inline bool operator==(const CPepeEngineQuaternion& rhs) const
	{
		return (rhs.x == x) &&(rhs.y == y) && (rhs.z == z) &&(rhs.w == w);
	}

	inline bool operator!=(const CPepeEngineQuaternion& rhs) const
	{
		return !operator==(rhs);
	}

	/** 
	 *	@{ Functions of a quaternion
	 */

	/** dot product */
	float dot(const CPepeEngineQuaternion& rkQ) const;  

	/** squared-length */
	float norm() const;  

	/** 
	 *	Normalises this quaternion, and returns the previous length
	 */
	float normalise(void); 

	/** 
	 *	Apply to non-zero quaternion
	 */
	CPepeEngineQuaternion inverse() const;  

	/** 
	 *	Apply to unit-length quaternion
	 */
	CPepeEngineQuaternion unitInverse() const;  
	CPepeEngineQuaternion exp() const;
	CPepeEngineQuaternion log() const;
	/** @} */

	/** 
	 *	rotation of a vector by a quaternion
	 */
	CPepeEngineVector3 operator*(const CPepeEngineVector3& rkVector) const;

	/** 
	 *	Calculate the local roll element of this quaternion.
	 *
	 *	@param reprojectAxis By default the method returns the 'intuitive' result
	 *	that is, if you projected the local Y of the quaterion onto the X and
	 *	Y axes, the angle between them is returned. If set to false though, the
	 *	result is the actual yaw that will be used to implement the quaternion,
	 *	which is the shortest possible path to get to the same orientation and 
	 *	may involve less axial rotation. 
	 */
	Radian getRoll(bool reprojectAxis = true) const;

	/** 
	 *	Calculate the local pitch element of this quaternion
	 *
	 *	@param reprojectAxis By default the method returns the 'intuitive' result
	 *	that is, if you projected the local Z of the quaterion onto the X and
	 *	Y axes, the angle between them is returned. If set to true though, the
	 *	result is the actual yaw that will be used to implement the quaternion,
	 *	which is the shortest possible path to get to the same orientation and 
	 *	may involve less axial rotation. 
	 */
	Radian getPitch(bool reprojectAxis = true) const;

	/** 
	 *	Calculate the local yaw element of this quaternion
	 *
	 *	@param reprojectAxis By default the method returns the 'intuitive' result
	 *	that is, if you projected the local Z of the quaterion onto the X and
	 *	Z axes, the angle between them is returned. If set to true though, the
	 *	result is the actual yaw that will be used to implement the quaternion,
	 *	which is the shortest possible path to get to the same orientation and 
	 *	may involve less axial rotation. 
	 */
	Radian getYaw(bool reprojectAxis = true) const;	

	/** 
	 *	Equality with tolerance(tolerance is max angle difference)
	 */
	bool equals(const CPepeEngineQuaternion& rhs, const Radian& tolerance) const;

	/** 
	 *	Spherical linear interpolation
	 */
	static CPepeEngineQuaternion slerp(float fT, const CPepeEngineQuaternion& rkP,
		const CPepeEngineQuaternion& rkQ, bool shortestPath = false);

	static CPepeEngineQuaternion slerpExtraSpins(float fT,
		const CPepeEngineQuaternion& rkP, const CPepeEngineQuaternion& rkQ,
		int iExtraSpins);

	/** 
	 *	Setup for spherical quadratic interpolation
	 */
	static void intermediate(const CPepeEngineQuaternion& rkQ0,
		const CPepeEngineQuaternion& rkQ1, const CPepeEngineQuaternion& rkQ2,
		CPepeEngineQuaternion& rka, CPepeEngineQuaternion& rkB);

	/** 
	 *	Spherical quadratic interpolation
	 */
	static CPepeEngineQuaternion squad(float fT, const CPepeEngineQuaternion& rkP,
		const CPepeEngineQuaternion& rkA, const CPepeEngineQuaternion& rkB,
		const CPepeEngineQuaternion& rkQ, bool shortestPath = false);

	/** 
	 *	Normalised linear interpolation - faster but less accurate(non-constant rotation velocity)
	 */
	static CPepeEngineQuaternion nlerp(float fT, const CPepeEngineQuaternion& rkP, 
		const CPepeEngineQuaternion& rkQ, bool shortestPath = false);

	
	_PepeEngineExport friend CPepeEngineQuaternion operator*(float fScalar, const CPepeEngineQuaternion& rkQ)
	{
		return CPepeEngineQuaternion(fScalar * rkQ.w, fScalar * rkQ.x, fScalar * rkQ.y,	fScalar * rkQ.z);
	}
};

_PEPE_ENGINE_END

#endif