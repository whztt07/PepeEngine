/**
 * Project: PepeEngine
 * Tier:	Frontend
 * File:	CPepeEngineSphere.h   
 *
 * @brief	Declaration of CPepeEnginePlane class. 
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-08-29
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINESPHERE_H
#define CPEPEENGINESPHERE_H

#include "PepeEngineInit.h"

#include "CPepeEngineVector3.h"

_PEPE_ENGINE_START

/**
 *	A sphere primitive, mostly used for bounds checking.     
 *	A sphere in math texts is normally represented by the function
 *	x^2 + y^2 + z^2 = r^2 (for sphere's centered on the origin). Ogre stores spheres
 *	simply as a center point and a radius.
 */
class _PepeEngineExport CPepeEngineSphere
{	
protected:
	float				m_fRadius;
	CPepeEngineVector3	m_center;
public:

	/** 
	 *	Standard constructor - creates a unit sphere around the origin.
	 */
	CPepeEngineSphere() : m_fRadius(1.0), m_center(CPepeEngineVector3::ZERO) {}

	/** 
	 *	Constructor allowing arbitrary spheres. 
	 *
	 *	@param center The center point of the sphere.
	 *	@param radius The radius of the sphere.
	 */
	CPepeEngineSphere(const CPepeEngineVector3& center, float radius)
		: m_fRadius(radius), m_center(center) {}

	/** 
	 *	Returns the radius of the sphere. 
	 */
	float getRadius(void) const {return m_fRadius;}

	/** 
	 *	Sets the radius of the sphere. 
	 */
	void setRadius(float radius) {m_fRadius = radius;}

	/** 
	 *	Returns the center point of the sphere. 
	 */
	const CPepeEngineVector3& getCenter(void) const {return m_center;}

	/** 
	 *	Sets the center point of the sphere. 
	 */
	void setCenter(const CPepeEngineVector3& center) {m_center = center;}

	/** 
	 *	Returns whether or not this sphere interects another sphere. 
	 */
	bool intersects(const CPepeEngineSphere& s) const
	{
		return (s.m_center - m_center).squaredLength() <= CPepeEngineMath::Sqr(s.m_fRadius + m_fRadius);
	}

	/** 
	 *	Returns whether or not this sphere interects a box. 
	 */
	bool intersects(const CPepeEngineAxisAlignedBox& box) const
	{
		return CPepeEngineMath::intersects(*this, box);
	}

	/** 
	 *	Returns whether or not this sphere interects a plane. 
	 */
	bool intersects(const CPepeEnginePlane& plane) const
	{
		return CPepeEngineMath::intersects(*this, plane);
	}

	/** 
	 *	Returns whether or not this sphere interects a point. 
	 */
	bool intersects(const CPepeEngineVector3& v) const
	{
		return ((v - m_center).squaredLength() <= CPepeEngineMath::Sqr(m_fRadius));
	}

};

_PEPE_ENGINE_END

#endif