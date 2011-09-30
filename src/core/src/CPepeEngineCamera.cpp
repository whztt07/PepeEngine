#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineCamera.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineCamera::CPepeEngineCamera()
{
	m_position			= CPepeEngineVector3::ZERO;	
	m_orientation		= CPepeEngineQuaternion::IDENTITY;
	m_bNeedViewUpdate	= true;

	updateView();
}

// -----------------------------------------------------------------------------------------
CPepeEngineCamera::~CPepeEngineCamera()
{
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::lookAt(const CPepeEngineVector3& point)
{
	setDirection(point - m_position);
	m_bNeedViewUpdate = true;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::lookAt(float x, float y, float z)
{
	lookAt(CPepeEngineVector3(x, y, z));
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::lookAt(const IPepeEngineRenderable& entity)
{
	lookAt(entity.getWorldPosition());
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::rotate(const Radian& angle, const CPepeEngineVector3& axis)
{	
	CPepeEngineQuaternion q;
	q.fromAngleAxis(angle, axis);
	rotate(q);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::rotate(const CPepeEngineQuaternion& q)
{	
	m_orientation		= q * m_orientation;
	m_bNeedViewUpdate	= true;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::roll(const Radian& angle)
{
	CPepeEngineVector3 zAxis = m_orientation * CPepeEngineVector3::UNIT_Z;
	rotate(angle, zAxis);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::yaw(const Radian& angle)
{
	CPepeEngineVector3 yAxis = m_orientation * CPepeEngineVector3::UNIT_Y;
	rotate(angle, yAxis);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::pitch(const Radian& angle)
{
	CPepeEngineVector3 xAxis = m_orientation * CPepeEngineVector3::UNIT_X;
	rotate(angle, xAxis);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::move(const CPepeEngineVector3& v)
{
	m_position			+= v;
	m_bNeedViewUpdate	= true;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::moveRelative(const CPepeEngineVector3& v)
{
	CPepeEngineVector3 vec	= m_orientation * v;
	m_position				+= vec;
	m_bNeedViewUpdate		= true;
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEngineCamera::getUp() const
{
	return m_orientation * CPepeEngineVector3::UNIT_Y;
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEngineCamera::getRight() const
{
	return m_orientation * CPepeEngineVector3::UNIT_X;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVector3& CPepeEngineCamera::getPosition() const 
{
	return m_position;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::setPosition(const CPepeEngineVector3& position) 
{		
	m_position			= position;
	m_bNeedViewUpdate	= true;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::setPosition(float x, float y, float z) 
{		
	CPepeEngineVector3 vec(x, y, z);
	setPosition(vec);	
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEngineCamera::getDirection() const
{
	return m_orientation * -CPepeEngineVector3::UNIT_Z;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::setDirection(const CPepeEngineVector3& dir)
{	
	if (dir == CPepeEngineVector3::ZERO) return;

	/*
		Remember, camera points down -Z of local axes!
		Therefore reverse direction of direction vector before determining local Z
	*/

	CPepeEngineVector3 zAdjustVec = -dir;	
	zAdjustVec.normalise();

	// Get axes from current quaternion
	CPepeEngineVector3 axes[3];
	
	m_orientation.toAxes(axes);
	CPepeEngineQuaternion rotQuat;
	if ((axes[2] + zAdjustVec).squaredLength() <  0.00005f) 
	{
		/* 
			Oops, a 180 degree turn (infinite possible rotation axes)
			Default to yaw i.e. use current UP
		*/
		rotQuat.fromAngleAxis(Radian(CPepeEngineMath::PI), axes[1]);
	}
	else
	{
		// Derive shortest arc to new direction
		rotQuat = axes[2].getRotationTo(zAdjustVec);

	}
	m_orientation		= rotQuat * m_orientation;
	m_bNeedViewUpdate	= true;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::setDirection(float x, float y, float z) 
{		
	CPepeEngineVector3 vec(x, y, z);
	setDirection(vec);	
}

// -----------------------------------------------------------------------------------------
const CPepeEngineQuaternion& CPepeEngineCamera::getOrientation() const 
{
	return m_orientation;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::setOrientation(const CPepeEngineQuaternion& orientation) 
{		
	m_orientation		= orientation;
	m_bNeedViewUpdate	= true;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCamera::updateView() const
{
	if (m_bNeedViewUpdate)
	{
		/*
			Update the view matrix		
			View matrix is:

			[ Lx  Uy  Dz  Tx  ]
			[ Lx  Uy  Dz  Ty  ]
			[ Lx  Uy  Dz  Tz  ]
			[ 0   0   0   1   ]
			
			Where T = -(Transposed(Rot) * Pos)
			This is most efficiently done using 3x3 Matrices
			Get orientation from quaternion
		*/

		CPepeEngineMatrix3 rot;						
		m_orientation.toRotationMatrix(rot);

		// Make the translation relative to new axes
		CPepeEngineMatrix3 rotT		= rot.transpose();
		CPepeEngineVector3 trans	= -rotT * m_position;

		// Make final matrix
		m_viewMatrix		= CPepeEngineMatrix4::IDENTITY;
		m_viewMatrix		= rotT; // fills upper 3x3
		m_viewMatrix[0][3]	= trans.x;
		m_viewMatrix[1][3]	= trans.y;
		m_viewMatrix[2][3]	= trans.z;
		m_bNeedViewUpdate	= false;
		m_bFrustumOutOfDate	= true;
	}
}

// -----------------------------------------------------------------------------------------
const CPepeEngineMatrix4& CPepeEngineCamera::getTransformMatrix() const 
{
	updateView(); 
	return m_viewMatrix;
}

// -----------------------------------------------------------------------------------------
const CPepeEnginePlane* CPepeEngineCamera::getFrustumPlanes(void)
{
	updateView();
	CPepeEngineFrustum::updateFrustumPlanes(m_viewMatrix);

	return m_frustumPlanes;
}

// -----------------------------------------------------------------------------------------
const CPepeEnginePlane& CPepeEngineCamera::getFrustumPlane(unsigned short nPlane)
{
	updateView();
	CPepeEngineFrustum::updateFrustumPlanes(m_viewMatrix);

	return m_frustumPlanes[nPlane];
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineCamera::isVisible(const CPepeEngineAxisAlignedBox& bound, FrustumPlane* culledBy) const
{
		// Null boxes always invisible
	if (bound.isNull()) return false;

	// Infinite boxes always visible
	if (bound.isInfinite()) return true;

	// Make any pending updates to the calculated frustum planes
	CPepeEngineFrustum::updateFrustumPlanes(getTransformMatrix());

	// Get centre of the box
	CPepeEngineVector3 centre = bound.getCenter();
	// Get the half-size of the box
	CPepeEngineVector3 halfSize = bound.getHalfSize();

	// For each plane, see if all points are on the negative side
	// If so, object is not visible
	for (int plane = 0; plane < 6; ++plane)
	{
		// Skip far plane if infinite view frustum
		if (plane == FRUSTUM_PLANE_FAR && m_fFarDist == 0)
		{
			continue;
		}

		CPepeEnginePlane::Side side = m_frustumPlanes[plane].getSide(centre, halfSize);
		if (side == CPepeEnginePlane::NEGATIVE_SIDE)
		{
			// ALL corners on negative side therefore out of view
			if (culledBy)
			{
				*culledBy = (FrustumPlane)plane;
			}
			return false;
		}
	}

	return true;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineCamera::isVisible(const CPepeEngineSphere& sphere, FrustumPlane* culledBy) const
{
	// Make any pending updates to the calculated frustum planes
	CPepeEngineFrustum::updateFrustumPlanes(getTransformMatrix());

	// For each plane, see if sphere is on negative side
	// If so, object is not visible
	for (int plane = 0; plane < 6; ++plane)
	{
		// Skip far plane if infinite view frustum
		if (plane == FRUSTUM_PLANE_FAR && m_fFarDist == 0)
		{
			continue;
		}

		// If the distance from sphere center to plane is negative, and 'more negative' 
		// than the radius of the sphere, sphere is outside frustum
		if (m_frustumPlanes[plane].getDistance(sphere.getCenter()) < -sphere.getRadius())
		{
			// ALL corners on negative side therefore out of view
			if (culledBy)
			{
				*culledBy = (FrustumPlane)plane;
			}
			return false;
		}
	}
	return true;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineCamera::isVisible(const CPepeEngineVector3& vert, FrustumPlane* culledBy) const
{
	// Make any pending updates to the calculated frustum planes
	CPepeEngineFrustum::updateFrustumPlanes(getTransformMatrix());

	// For each plane, see if all points are on the negative side
	// If so, object is not visible
	for (int plane = 0; plane < 6; ++plane)
	{
		// Skip far plane if infinite view frustum
		if (plane == FRUSTUM_PLANE_FAR && m_fFarDist == 0)
			continue;

		if (m_frustumPlanes[plane].getSide(vert) == CPepeEnginePlane::NEGATIVE_SIDE)
		{
			// ALL corners on negative side therefore out of view
			if (culledBy)
			{
				*culledBy = (FrustumPlane)plane;
			}
			return false;
		}

	}

	return true;
}

_PEPE_ENGINE_END