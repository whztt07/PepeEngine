#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineAxisAlignedBox.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineAxisAlignedBox::CPepeEngineAxisAlignedBox() : m_pCorners(0)
{
	// Default to a null box 
	setMinimum(-0.5, -0.5, -0.5);
	setMaximum(0.5, 0.5, 0.5);
	m_extent = EXTENT_NULL;
}

// -----------------------------------------------------------------------------------------
CPepeEngineAxisAlignedBox::CPepeEngineAxisAlignedBox(const CPepeEngineAxisAlignedBox & rkBox) : m_pCorners(0)
{
	if (rkBox.isNull())
	{
		setNull();
	}
	else if (rkBox.isInfinite())
	{
		setInfinite();
	}
	else
	{
		setExtents(rkBox.m_minimum, rkBox.m_maximum);
	}
}

// -----------------------------------------------------------------------------------------
CPepeEngineAxisAlignedBox::CPepeEngineAxisAlignedBox(const CPepeEngineVector3& min, const CPepeEngineVector3& max) : m_pCorners(0)
{
	setExtents(min, max);
}

// -----------------------------------------------------------------------------------------
CPepeEngineAxisAlignedBox::CPepeEngineAxisAlignedBox(
	float mx, float my, float mz,
	float Mx, float My, float Mz) : m_pCorners(0)
{
	setExtents(mx, my, mz, Mx, My, Mz);
}

// -----------------------------------------------------------------------------------------
CPepeEngineAxisAlignedBox& CPepeEngineAxisAlignedBox::operator=(const CPepeEngineAxisAlignedBox& rhs)
{
	// Specifically override to avoid copying m_pCorners
	if (rhs.isNull())
	{
		setNull();
	}
	else if (rhs.isInfinite())
	{
		setInfinite();
	}
	else
	{
		setExtents(rhs.m_minimum, rhs.m_maximum);
	}

	return *this;
}

// -----------------------------------------------------------------------------------------
CPepeEngineAxisAlignedBox::~CPepeEngineAxisAlignedBox()
{
	if (m_pCorners)
	{
		delete[] m_pCorners;
	}
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVector3& CPepeEngineAxisAlignedBox::getMinimum(void) const
{ 
	return m_minimum; 
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3& CPepeEngineAxisAlignedBox::getMinimum(void)
{ 
	return m_minimum; 
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVector3& CPepeEngineAxisAlignedBox::getMaximum(void) const
{ 
	return m_maximum;
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3& CPepeEngineAxisAlignedBox::getMaximum(void)
{ 
	return m_maximum;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::setMinimum(const CPepeEngineVector3& vec)
{
	m_extent = EXTENT_FINITE;
	m_minimum = vec;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::setMinimum(float x, float y, float z)
{
	m_extent	= EXTENT_FINITE;
	m_minimum.x = x;
	m_minimum.y = y;
	m_minimum.z = z;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::setMinimumX(float x)
{
	m_minimum.x = x;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::setMinimumY(float y)
{
	m_minimum.y = y;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::setMinimumZ(float z)
{
	m_minimum.z = z;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::setMaximum(const CPepeEngineVector3& vec)
{
	m_extent	= EXTENT_FINITE;
	m_maximum	= vec;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::setMaximum(float x, float y, float z)
{
	m_extent	= EXTENT_FINITE;
	m_maximum.x = x;
	m_maximum.y = y;
	m_maximum.z = z;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::setMaximumX(float x)
{
	m_maximum.x = x;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::setMaximumY(float y)
{
	m_maximum.y = y;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::setMaximumZ(float z)
{
	m_maximum.z = z;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::setExtents(const CPepeEngineVector3& min, const CPepeEngineVector3& max)
{		
	assert((min.x <= max.x && min.y <= max.y && min.z <= max.z) &&
		"The minimum corner of the box must be less than or equal to maximum corner");

	m_extent	= EXTENT_FINITE;
	m_minimum	= min;
	m_maximum	= max;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::setExtents(
	float mx, float my, float mz,
	float Mx, float My, float Mz)
{
	assert((mx <= Mx && my <= My && mz <= Mz) &&
		"The minimum corner of the box must be less than or equal to maximum corner");

	m_extent = EXTENT_FINITE;

	m_minimum.x = mx;
	m_minimum.y = my;
	m_minimum.z = mz;

	m_maximum.x = Mx;
	m_maximum.y = My;
	m_maximum.z = Mz;

}

// -----------------------------------------------------------------------------------------
const CPepeEngineVector3* CPepeEngineAxisAlignedBox::getAllCorners(void) const
{
	assert((m_extent == EXTENT_FINITE) && "Can't get corners of a null or infinite AAB");

	/* 
	The order of these items is, using right-handed co-ordinates:
	Minimum Z face, starting with Min(all), then anticlockwise
	around face (looking onto the face)
	Maximum Z face, starting with Max(all), then anticlockwise
	around face (looking onto the face)
	Only for optimization/compatibility.
	*/
	if (!m_pCorners)
	{
		m_pCorners = new CPepeEngineVector3[8];
	}

	m_pCorners[0] = m_minimum;
	m_pCorners[1].x = m_minimum.x; m_pCorners[1].y = m_maximum.y; m_pCorners[1].z = m_minimum.z;
	m_pCorners[2].x = m_maximum.x; m_pCorners[2].y = m_maximum.y; m_pCorners[2].z = m_minimum.z;
	m_pCorners[3].x = m_maximum.x; m_pCorners[3].y = m_minimum.y; m_pCorners[3].z = m_minimum.z;            

	m_pCorners[4] = m_maximum;
	m_pCorners[5].x = m_minimum.x; m_pCorners[5].y = m_maximum.y; m_pCorners[5].z = m_maximum.z;
	m_pCorners[6].x = m_minimum.x; m_pCorners[6].y = m_minimum.y; m_pCorners[6].z = m_maximum.z;
	m_pCorners[7].x = m_maximum.x; m_pCorners[7].y = m_minimum.y; m_pCorners[7].z = m_maximum.z;

	return m_pCorners;
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEngineAxisAlignedBox::getCorner(CornerEnum cornerToGet) const
{
	switch(cornerToGet)
	{
	case FAR_LEFT_BOTTOM:
		return m_minimum;
	case FAR_LEFT_TOP:
		return CPepeEngineVector3(m_minimum.x, m_maximum.y, m_minimum.z);
	case FAR_RIGHT_TOP:
		return CPepeEngineVector3(m_maximum.x, m_maximum.y, m_minimum.z);
	case FAR_RIGHT_BOTTOM:
		return CPepeEngineVector3(m_maximum.x, m_minimum.y, m_minimum.z);
	case NEAR_RIGHT_BOTTOM:
		return CPepeEngineVector3(m_maximum.x, m_minimum.y, m_maximum.z);
	case NEAR_LEFT_BOTTOM:
		return CPepeEngineVector3(m_minimum.x, m_minimum.y, m_maximum.z);
	case NEAR_LEFT_TOP:
		return CPepeEngineVector3(m_minimum.x, m_maximum.y, m_maximum.z);
	case NEAR_RIGHT_TOP:
		return m_maximum;
	default:
		return CPepeEngineVector3();
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::merge(const CPepeEngineAxisAlignedBox& rhs)
{
	// Do nothing if rhs null, or this is infinite
	if ((rhs.m_extent == EXTENT_NULL) || (m_extent == EXTENT_INFINITE))
	{
		return;
	}
	// Otherwise if rhs is infinite, make this infinite, too
	else if (rhs.m_extent == EXTENT_INFINITE)
	{
		m_extent = EXTENT_INFINITE;
	}
	// Otherwise if current null, just take rhs
	else if (m_extent == EXTENT_NULL)
	{
		setExtents(rhs.m_minimum, rhs.m_maximum);
	}
	// Otherwise merge
	else
	{
		CPepeEngineVector3 min = m_minimum;
		CPepeEngineVector3 max = m_maximum;
		max.makeCeil(rhs.m_maximum);
		min.makeFloor(rhs.m_minimum);

		setExtents(min, max);
	}

}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::merge(const CPepeEngineVector3& point)
{
	switch (m_extent)
	{
	case EXTENT_NULL: // if null, use this point
		setExtents(point, point);
		return;

	case EXTENT_FINITE:
		m_maximum.makeCeil(point);
		m_minimum.makeFloor(point);
		return;

	case EXTENT_INFINITE: // if infinite, makes no difference
		return;
	}

	assert(false && "Never reached");
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::transform(const CPepeEngineMatrix4& matrix)
{
	// Do nothing if current null or infinite
	if (m_extent != EXTENT_FINITE) return;

	CPepeEngineVector3 oldMin, oldMax, currentCorner;

	// Getting the old values so that we can use the existing merge method.
	oldMin = m_minimum;
	oldMax = m_maximum;

	// reset
	setNull();

	/*
	We sequentially compute the corners in the following order :
	0, 6, 5, 1, 2, 4 ,7 , 3
	This sequence allows us to only change one member at a time to get at all corners.

	For each one, we transform it using the matrix
	Which gives the resulting point and merge the resulting point.

	First corner 
	min min min
	*/
	currentCorner = oldMin;	
	
	merge(matrix * currentCorner);

	// min,min,max
	currentCorner.z = oldMax.z;
	merge(matrix * currentCorner);

	// min max max
	currentCorner.y = oldMax.y;
	merge(matrix * currentCorner);

	// min max min
	currentCorner.z = oldMin.z;
	merge(matrix * currentCorner);

	// max max min
	currentCorner.x = oldMax.x;
	merge(matrix * currentCorner);

	// max max max
	currentCorner.z = oldMax.z;
	merge(matrix * currentCorner);

	// max min max
	currentCorner.y = oldMin.y;
	merge(matrix * currentCorner);

	// max min min
	currentCorner.z = oldMin.z;
	merge(matrix * currentCorner); 	
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::transformAffine(const CPepeEngineMatrix4& m)
{
	assert(m.isAffine());

	// Do nothing if current null or infinite
	if (m_extent != EXTENT_FINITE) return;

	CPepeEngineVector3 centre	= getCenter();
	CPepeEngineVector3 halfSize = getHalfSize();

	CPepeEngineVector3 newCentre = m.transformAffine(centre);
	CPepeEngineVector3 newHalfSize(
		CPepeEngineMath::Abs(m[0][0]) * halfSize.x + CPepeEngineMath::Abs(m[0][1]) * halfSize.y + CPepeEngineMath::Abs(m[0][2]) * halfSize.z, 
		CPepeEngineMath::Abs(m[1][0]) * halfSize.x + CPepeEngineMath::Abs(m[1][1]) * halfSize.y + CPepeEngineMath::Abs(m[1][2]) * halfSize.z,
		CPepeEngineMath::Abs(m[2][0]) * halfSize.x + CPepeEngineMath::Abs(m[2][1]) * halfSize.y + CPepeEngineMath::Abs(m[2][2]) * halfSize.z);

	setExtents(newCentre - newHalfSize, newCentre + newHalfSize);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::setNull()
{
	m_extent = EXTENT_NULL;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineAxisAlignedBox::isNull(void) const
{
	return (m_extent == EXTENT_NULL);
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineAxisAlignedBox::isFinite(void) const
{
	return (m_extent == EXTENT_FINITE);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::setInfinite()
{
	m_extent = EXTENT_INFINITE;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineAxisAlignedBox::isInfinite(void) const
{
	return (m_extent == EXTENT_INFINITE);
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineAxisAlignedBox::intersects(const CPepeEngineAxisAlignedBox& b2) const
{
	// Early-fail for nulls
	if (this->isNull() || b2.isNull())
	{
		return false;
	}

	// Early-success for infinites
	if (this->isInfinite() || b2.isInfinite())
	{
		return true;
	}

	// Use up to 6 separating planes
	if (m_maximum.x < b2.m_minimum.x)
		return false;
	if (m_maximum.y < b2.m_minimum.y)
		return false;
	if (m_maximum.z < b2.m_minimum.z)
		return false;

	if (m_minimum.x > b2.m_maximum.x)
		return false;
	if (m_minimum.y > b2.m_maximum.y)
		return false;
	if (m_minimum.z > b2.m_maximum.z)
		return false;

	// otherwise, must be intersecting
	return true;

}

// -----------------------------------------------------------------------------------------
CPepeEngineAxisAlignedBox CPepeEngineAxisAlignedBox::intersection(const CPepeEngineAxisAlignedBox& b2) const
{
	if (this->isNull() || b2.isNull())
	{
		return CPepeEngineAxisAlignedBox();
	}
	else if (this->isInfinite())
	{
		return b2;
	}
	else if (b2.isInfinite())
	{
		return *this;
	}

	CPepeEngineVector3 intMin = m_minimum;
	CPepeEngineVector3 intMax = m_maximum;

	intMin.makeCeil(b2.getMinimum());
	intMax.makeFloor(b2.getMaximum());

	// Check intersection isn't null
	if (intMin.x < intMax.x &&
		intMin.y < intMax.y &&
		intMin.z < intMax.z)
	{
		return CPepeEngineAxisAlignedBox(intMin, intMax);
	}

	return CPepeEngineAxisAlignedBox();
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAxisAlignedBox::volume(void) const
{
	switch (m_extent)
	{
	case EXTENT_NULL:
		return 0.0f;

	case EXTENT_FINITE:
		{
			CPepeEngineVector3 diff = m_maximum - m_minimum;
			return diff.x * diff.y * diff.z;
		}

	case EXTENT_INFINITE:
		return CPepeEngineMath::POS_INFINITY;

	default: // shut up compiler
		assert(false && "Never reached");
		return 0.0f;
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAxisAlignedBox::scale(const CPepeEngineVector3& s)
{
	// Do nothing if current null or infinite
	if (m_extent != EXTENT_FINITE)
	{
		return;
	}

	// NB assumes centered on origin
	CPepeEngineVector3 min = m_minimum * s;
	CPepeEngineVector3 max = m_maximum * s;
	setExtents(min, max);
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineAxisAlignedBox::intersects(const CPepeEngineVector3& v) const
{
	switch (m_extent)
	{
	case EXTENT_NULL:
		return false;

	case EXTENT_FINITE:
		return(
			v.x >= m_minimum.x  &&  v.x <= m_maximum.x  && 
			v.y >= m_minimum.y  &&  v.y <= m_maximum.y  && 
			v.z >= m_minimum.z  &&  v.z <= m_maximum.z
			);

	case EXTENT_INFINITE:
		return true;

	default: // shut up compiler
		assert(false && "Never reached");
		return false;
	}
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineAxisAlignedBox::intersects(const CPepeEngineSphere& s) const
{
	return CPepeEngineMath::intersects(s, *this); 
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineAxisAlignedBox::intersects(const CPepeEnginePlane& p) const
{
	return CPepeEngineMath::intersects(p, *this);
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEngineAxisAlignedBox::getCenter(void) const
{
	assert((m_extent == EXTENT_FINITE) && "Can't get center of a null or infinite AAB");

	return CPepeEngineVector3(
		(m_maximum.x + m_minimum.x) * 0.5f,
		(m_maximum.y + m_minimum.y) * 0.5f,
		(m_maximum.z + m_minimum.z) * 0.5f
	);
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEngineAxisAlignedBox::getSize(void) const
{
	switch (m_extent)
	{
	case EXTENT_NULL:
		return CPepeEngineVector3::ZERO;

	case EXTENT_FINITE:
		return m_maximum - m_minimum;

	case EXTENT_INFINITE:
		return CPepeEngineVector3(
			CPepeEngineMath::POS_INFINITY,
			CPepeEngineMath::POS_INFINITY,
			CPepeEngineMath::POS_INFINITY
			);

	default: // shut up compiler
		assert(false && "Never reached");
		return CPepeEngineVector3::ZERO;
	}
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEngineAxisAlignedBox::getHalfSize(void) const
{
	switch (m_extent)
	{
	case EXTENT_NULL:
		return CPepeEngineVector3::ZERO;

	case EXTENT_FINITE:
		return (m_maximum - m_minimum) * 0.5;

	case EXTENT_INFINITE:
		return CPepeEngineVector3(
			CPepeEngineMath::POS_INFINITY,
			CPepeEngineMath::POS_INFINITY,
			CPepeEngineMath::POS_INFINITY
			);

	default: // shut up compiler
		assert(false && "Never reached");
		return CPepeEngineVector3::ZERO;
	}
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineAxisAlignedBox::contains(const CPepeEngineVector3& v) const
{
	if (isNull())
	{
		return false;
	}

	if (isInfinite())
	{
		return true;
	}

	return m_minimum.x <= v.x && v.x <= m_maximum.x &&
		m_minimum.y <= v.y && v.y <= m_maximum.y &&
		m_minimum.z <= v.z && v.z <= m_maximum.z;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineAxisAlignedBox::contains(const CPepeEngineAxisAlignedBox& other) const
{
	if (other.isNull() || this->isInfinite())
		return true;

	if (this->isNull() || other.isInfinite())
		return false;

	return this->m_minimum.x <= other.m_minimum.x &&
		this->m_minimum.y <= other.m_minimum.y &&
		this->m_minimum.z <= other.m_minimum.z &&
		other.m_maximum.x <= this->m_maximum.x &&
		other.m_maximum.y <= this->m_maximum.y &&
		other.m_maximum.z <= this->m_maximum.z;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineAxisAlignedBox::operator==(const CPepeEngineAxisAlignedBox& rhs) const
{
	if (this->m_extent != rhs.m_extent)
	{
		return false;
	}

	if (!this->isFinite())
	{
		return true;
	}

	return this->m_minimum == rhs.m_minimum && this->m_maximum == rhs.m_maximum;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineAxisAlignedBox::operator!=(const CPepeEngineAxisAlignedBox& rhs) const
{
	return !(*this == rhs);
}

_PEPE_ENGINE_END