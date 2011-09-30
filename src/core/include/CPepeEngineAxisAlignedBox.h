/**
 * Project:		PepeEngine
 * Tier:		Frontend
 * File:		CPepeEngineAxisAlignedBox.h   
 *
 * @brief		Declaration of CPepeEngineAxisAlignedBox class.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-07-29
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef CPEPEENGINEAXISALIGNEDBOX_H
#define CPEPEENGINEAXISALIGNEDBOX_H

#include "PepeEngineInit.h"

#include "CPepeEngineVector3.h"

_PEPE_ENGINE_START

enum Extent
{
	EXTENT_NULL,
	EXTENT_FINITE,
	EXTENT_INFINITE
};

/**
*		   1-----2
*		  /|    /|
*		 / |   / |
*		5-----4  |
*		|  0--|--3
*		| /   | /
*		|/    |/
*		6-----7
*/
typedef enum 
{
	FAR_LEFT_BOTTOM		= 0,
	FAR_LEFT_TOP		= 1,
	FAR_RIGHT_TOP		= 2,
	FAR_RIGHT_BOTTOM	= 3,
	NEAR_RIGHT_BOTTOM	= 7,
	NEAR_LEFT_BOTTOM	= 6,
	NEAR_LEFT_TOP		= 5,
	NEAR_RIGHT_TOP		= 4
} CornerEnum;


class _PepeEngineExport CPepeEngineAxisAlignedBox
{	
public:	
	inline								CPepeEngineAxisAlignedBox();
	inline								CPepeEngineAxisAlignedBox(const CPepeEngineAxisAlignedBox & rkBox);

	inline								CPepeEngineAxisAlignedBox(
											const CPepeEngineVector3& min, const CPepeEngineVector3& max);

	inline								CPepeEngineAxisAlignedBox(
											float mx, float my, float mz,
											float Mx, float My, float Mz);

	inline CPepeEngineAxisAlignedBox&	operator=(const CPepeEngineAxisAlignedBox& rhs);
	inline								~CPepeEngineAxisAlignedBox();


	/** 
	 *	Gets the minimum corner of the box.
	 */
	inline const CPepeEngineVector3&	getMinimum(void) const;

	/** 
	 *	Gets a modifiable version of the minimum
	 *	corner of the box.
	 */
	inline CPepeEngineVector3&			getMinimum(void);

	/** 
	 *	Gets the maximum corner of the box.
	 */
	inline const CPepeEngineVector3&	getMaximum(void) const;

	/** 
	 *	Gets a modifiable version of the maximum
	 *	corner of the box.
	 */
	inline CPepeEngineVector3&			getMaximum(void);


	/** 
	 *	Sets the minimum corner of the box.
	 */
	inline void							setMinimum(const CPepeEngineVector3& vec);
	inline void							setMinimum(float x, float y, float z);

	/** 
	 *	Changes one of the components of the minimum corner of the box
	 *	used to resize only one dimension of the box
	 */
	inline void							setMinimumX(float x);
	inline void							setMinimumY(float y);
	inline void							setMinimumZ(float z);
	/** 
	 *	Sets the maximum corner of the box.
	 */
	inline void							setMaximum(const CPepeEngineVector3& vec);

	inline void							setMaximum(float x, float y, float z);

	/** 
	 *	Changes one of the components of the maximum corner of the box
	 *	used to resize only one dimension of the box
	 */
	inline void							setMaximumX(float x);
	inline void							setMaximumY(float y);
	inline void							setMaximumZ(float z);

	/** 
	 *	Sets both minimum and maximum extents at once.
	 */
	inline void							setExtents(const CPepeEngineVector3& min, const CPepeEngineVector3& max);
	inline void							setExtents(
											float mx, float my, float mz,
											float Mx, float My, float Mz);

	/** 
	 *	Returns a pointer to an array of 8 corner points, useful for
	 *	collision vs. non-aligned objects.
	 *
	 *	If the order of these corners is important, they are as
	 *	follows: The 4 points of the minimum Z face (note that
	 *	because PepeEngine uses right-handed coordinates, the minimum Z is
	 *	at the 'back' of the box) starting with the minimum point of
	 *	all, then anticlockwise around this face (if you are looking
	 *	onto the face from outside the box). Then the 4 points of the
	 *	maximum Z face, starting with maximum point of all, then
	 *	anticlockwise around this face (looking onto the face from
	 *	outside the box). Like this:
	 *	<pre>
	 *		   1-----2
	 *		  /|    /|
	 *		 / |   / |
	 *		5-----4  |
	 *		|  0--|--3
	 *		| /   | /
	 *		|/    |/
	 *		6-----7
	 *	</pre>	
	 */
	inline const CPepeEngineVector3*	getAllCorners(void) const;

	/** 
	 *	Gets the position of one of the corners
	 */
	inline CPepeEngineVector3			getCorner(CornerEnum cornerToGet) const;


	/** 
	 *	Merges the passed in box into the current box. The result is the
	 *	box which encompasses both.
	 */
	inline void							merge(const CPepeEngineAxisAlignedBox& rhs);

	/**
	 *	Extends the box to encompass the specified point (if needed).
	 */
	inline void							merge(const CPepeEngineVector3& point);

	/** 
	 *	Transforms the box according to the matrix supplied.
	 *
	 *	By calling this method you get the axis-aligned box which
	 *	surrounds the transformed version of this box. Therefore each
	 *	corner of the box is transformed by the matrix, then the
	 *	extents are mapped back onto the axes to produce another
	 *	AABB. Useful when you have a local AABB for an object which
	 *	is then transformed.
	 */
	inline void							transform(const CPepeEngineMatrix4& matrix);

	/** 
	 *	Transforms the box according to the affine matrix supplied.
	 *
	 *	By calling this method you get the axis-aligned box which
	 *	surrounds the transformed version of this box. Therefore each
	 *	corner of the box is transformed by the matrix, then the
	 *	extents are mapped back onto the axes to produce another
	 *	AABB. Useful when you have a local AABB for an object which
	 *	is then transformed.
	 *
	 *	The matrix must be an affine matrix. 
	 *	@see CPepeEngineMatrix4::isAffine.
	 */
	inline void							transformAffine(const CPepeEngineMatrix4& m);	

	/** 
	 *	Sets the box to a 'null' value i.e. not a box.
	 */
	inline void							setNull();

	/** 
	 *	Returns true if the box is null i.e. empty.
	 */
	inline bool							isNull(void) const;

	/** 
	 *	Returns true if the box is finite.
	 */
	inline bool							isFinite(void) const;

	/** 
	 *	Sets the box to 'infinite'
	 */
	inline void							setInfinite();

	/** 
	 *	Returns true if the box is infinite.
	 */
	inline bool							isInfinite(void) const;

	/** 
	 *	Returns whether or not this box intersects another. 
	 */
	inline bool							intersects(const CPepeEngineAxisAlignedBox& b2) const;

	/** 
	 *	Calculate the area of intersection of this box and another
	 */
	inline CPepeEngineAxisAlignedBox	intersection(const CPepeEngineAxisAlignedBox& b2) const;

	/** 
	 *	Calculate the volume of this box
	 */
	float								volume(void) const;

	/** 
	 *	Scales the AABB by the vector given. 
	 */
	inline void							scale(const CPepeEngineVector3& s);
	
	/** 
	 *	Tests whether the vector point is within this box. 
	 */
	inline bool							intersects(const CPepeEngineVector3& v) const;
	/** 
	 *	Tests whether this box intersects a sphere. 
	 */
	inline bool							intersects(const CPepeEngineSphere& s) const;

	/** 
	 *	Tests whether this box intersects a plane. 
	 */
	inline bool							intersects(const CPepeEnginePlane& p) const;

	/** 
	 *	Gets the centre of the box
	 */
	inline CPepeEngineVector3			getCenter(void) const;

	/** 
	 *	Gets the size of the box
	 */
	inline CPepeEngineVector3			getSize(void) const;

	/** 
	 *	Gets the half-size of the box
	 */
	inline CPepeEngineVector3			getHalfSize(void) const;

	/** 
	 *	Tests whether the given point contained by this box.
	 */
	inline bool							contains(const CPepeEngineVector3& v) const;

	/** 
	 *	Tests whether another box contained by this box.
	 */
	inline bool							contains(const CPepeEngineAxisAlignedBox& other) const;

	/** 
	 *	Tests 2 boxes for equality.
	 */
	inline bool							operator==(const CPepeEngineAxisAlignedBox& rhs) const;

	/** 
	 *	Tests 2 boxes for inequality.
	 */
	inline bool							operator!=(const CPepeEngineAxisAlignedBox& rhs) const;
protected:	
	CPepeEngineVector3					m_minimum;
	CPepeEngineVector3					m_maximum;
	Extent								m_extent;

	mutable CPepeEngineVector3*			m_pCorners;
};

_PEPE_ENGINE_END

#endif