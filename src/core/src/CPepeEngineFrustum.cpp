#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineFrustum.h"

_PEPE_ENGINE_START

const float CPepeEngineFrustum::INFINITE_FAR_PLANE_ADJUST = 0.00001f;

// -----------------------------------------------------------------------------------------
void CPepeEngineFrustum::updateFrustum() const
{
	if (m_bFrustumOutOfDate)
	{
		float left, right, bottom, top;
		
		/*
			Calculate general projection parameters
		*/
		Radian thetaY (m_fFOVy * 0.5f);
		float tanThetaY = CPepeEngineMath::Tan(thetaY);
		float tanThetaX = tanThetaY * m_fAspect;

		// Unknow how to apply frustum offset to orthographic camera, just ignore here
		float nearFocal		= (m_projectionType == PT_PERSPECTIVE) ? m_fNearDist : 0;	
		float half_w		= tanThetaX * m_fNearDist;
		float half_h		= tanThetaY * m_fNearDist;

		left   = - half_w;
		right  = + half_w;
		bottom = - half_h;
		top    = + half_h;

		/*
			The code below will dealing with general projection 
			parameters, similar glFrustum and glOrtho.
			Doesn't optimise manually except division operator, so the 
			code more self-explaining.
		*/

		float inv_w = 1 / (right - left);
		float inv_h = 1 / (top - bottom);
		float inv_d = 1 / (m_fFarDist - m_fNearDist);

		// Recalc if frustum params changed
		if (m_projectionType == PT_PERSPECTIVE)
		{
			// Calc matrix elements
			float A = 2 * m_fNearDist * inv_w;
			float B = 2 * m_fNearDist * inv_h;
			float C = (right + left) * inv_w;
			float D = (top + bottom) * inv_h;
			float q, qn;
			if (m_fFarDist == 0)
			{
				// Infinite far plane
				q	= CPepeEngineFrustum::INFINITE_FAR_PLANE_ADJUST - 1;
				qn	= m_fNearDist * (CPepeEngineFrustum::INFINITE_FAR_PLANE_ADJUST - 2);
			}
			else
			{
				q	= - (m_fFarDist + m_fNearDist) * inv_d;
				qn	= -2 * (m_fFarDist * m_fNearDist) * inv_d;
			}

			/*
				NB: This creates 'uniform' perspective projection matrix,
				which depth range [-1,1], right-handed rules
			
				[ A   0   C   0  ]
				[ 0   B   D   0  ]
				[ 0   0   q   qn ]
				[ 0   0   -1  0  ]
			
				A = 2 * near / (right - left)
				B = 2 * near / (top - bottom)
				C = (right + left) / (right - left)
				D = (top + bottom) / (top - bottom)
				q = - (far + near) / (far - near)
				qn = - 2 * (far * near) / (far - near)
			*/

			m_projectionMatrix			= CPepeEngineMatrix4::ZERO;
			m_projectionMatrix[0][0]	= A;
			m_projectionMatrix[0][2]	= C;
			m_projectionMatrix[1][1]	= B;
			m_projectionMatrix[1][2]	= D;
			m_projectionMatrix[2][2]	= q;
			m_projectionMatrix[2][3]	= qn;
			m_projectionMatrix[3][2]	= -1;

			
		} // perspective
		else if (m_projectionType == PT_ORTHOGRAPHIC)
		{
			float A = 2 * inv_w;
			float B = 2 * inv_h;
			float C = - (right + left) * inv_w;
			float D = - (top + bottom) * inv_h;
			float q, qn;
			if (m_fFarDist == 0)
			{
				// Can not do infinite far plane here, avoid divided zero only
				q	= -CPepeEngineFrustum::INFINITE_FAR_PLANE_ADJUST / m_fNearDist;
				qn	= -CPepeEngineFrustum::INFINITE_FAR_PLANE_ADJUST - 1;
			}
			else
			{
				q = - 2 * inv_d;
				qn = - (m_fFarDist + m_fNearDist)  * inv_d;
			}

			/*
				NB: This creates 'uniform' orthographic projection matrix,
				which depth range [-1,1], right-handed rules
			
				[ A   0   0   C  ]
				[ 0   B   0   D  ]
				[ 0   0   q   qn ]
				[ 0   0   0   1  ]
			
				A = 2 * / (right - left)
				B = 2 * / (top - bottom)
				C = - (right + left) / (right - left)
				D = - (top + bottom) / (top - bottom)
				q = - 2 / (far - near)
				qn = - (far + near) / (far - near)
			*/

			m_projectionMatrix			= CPepeEngineMatrix4::ZERO;
			m_projectionMatrix[0][0]	= A;
			m_projectionMatrix[0][3]	= C;
			m_projectionMatrix[1][1]	= B;
			m_projectionMatrix[1][3]	= D;
			m_projectionMatrix[2][2]	= q;
			m_projectionMatrix[2][3]	= qn;
			m_projectionMatrix[3][3]	= 1;
		} // ortho

		CPepeEngineCore::getSingleton().getRenderer()->convertProjectionMatrix(m_projectionMatrix, m_projectionMatrixRS); 

		m_bRecalcFrustumPlanes	= true;
		m_bFrustumOutOfDate		= false;
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineFrustum::updateFrustumPlanes(const CPepeEngineMatrix4& viewMatrix) const
{
	updateFrustum();

	if (m_bRecalcFrustumPlanes)
	{
		CPepeEngineMatrix4 combo = m_projectionMatrix * viewMatrix;

		m_frustumPlanes[FRUSTUM_PLANE_LEFT].normal.x	= combo[3][0] + combo[0][0];
		m_frustumPlanes[FRUSTUM_PLANE_LEFT].normal.y	= combo[3][1] + combo[0][1];
		m_frustumPlanes[FRUSTUM_PLANE_LEFT].normal.z	= combo[3][2] + combo[0][2];
		m_frustumPlanes[FRUSTUM_PLANE_LEFT].d			= combo[3][3] + combo[0][3];

		m_frustumPlanes[FRUSTUM_PLANE_RIGHT].normal.x	= combo[3][0] - combo[0][0];
		m_frustumPlanes[FRUSTUM_PLANE_RIGHT].normal.y	= combo[3][1] - combo[0][1];
		m_frustumPlanes[FRUSTUM_PLANE_RIGHT].normal.z	= combo[3][2] - combo[0][2];
		m_frustumPlanes[FRUSTUM_PLANE_RIGHT].d			= combo[3][3] - combo[0][3];

		m_frustumPlanes[FRUSTUM_PLANE_TOP].normal.x		= combo[3][0] - combo[1][0];
		m_frustumPlanes[FRUSTUM_PLANE_TOP].normal.y		= combo[3][1] - combo[1][1];
		m_frustumPlanes[FRUSTUM_PLANE_TOP].normal.z		= combo[3][2] - combo[1][2];
		m_frustumPlanes[FRUSTUM_PLANE_TOP].d			= combo[3][3] - combo[1][3];

		m_frustumPlanes[FRUSTUM_PLANE_BOTTOM].normal.x	= combo[3][0] + combo[1][0];
		m_frustumPlanes[FRUSTUM_PLANE_BOTTOM].normal.y	= combo[3][1] + combo[1][1];
		m_frustumPlanes[FRUSTUM_PLANE_BOTTOM].normal.z	= combo[3][2] + combo[1][2];
		m_frustumPlanes[FRUSTUM_PLANE_BOTTOM].d			= combo[3][3] + combo[1][3];

		m_frustumPlanes[FRUSTUM_PLANE_NEAR].normal.x	= combo[3][0] + combo[2][0];
		m_frustumPlanes[FRUSTUM_PLANE_NEAR].normal.y	= combo[3][1] + combo[2][1];
		m_frustumPlanes[FRUSTUM_PLANE_NEAR].normal.z	= combo[3][2] + combo[2][2];
		m_frustumPlanes[FRUSTUM_PLANE_NEAR].d			= combo[3][3] + combo[2][3];

		m_frustumPlanes[FRUSTUM_PLANE_FAR].normal.x		= combo[3][0] - combo[2][0];
		m_frustumPlanes[FRUSTUM_PLANE_FAR].normal.y		= combo[3][1] - combo[2][1];
		m_frustumPlanes[FRUSTUM_PLANE_FAR].normal.z		= combo[3][2] - combo[2][2];
		m_frustumPlanes[FRUSTUM_PLANE_FAR].d			= combo[3][3] - combo[2][3];

		// Renormalise any normals which were not unit length
		for (int i = 0; i < 6; i++) 
		{
			float length			= m_frustumPlanes[i].normal.normalise();
			m_frustumPlanes[i].d	/= length;
		}

		m_bRecalcFrustumPlanes = false;
	}
}

// -----------------------------------------------------------------------------------------
CPepeEngineFrustum::CPepeEngineFrustum()
{
	m_projectionType	= PT_PERSPECTIVE;
	m_fFOVy				= Radian(CPepeEngineMath::PI / 4.0f);
	m_fFarDist			= 100000.0f;
	m_fNearDist			= 100.0f;
	m_fAspect			= 1.33333333333333f; 	
	
	m_bRecalcFrustumPlanes	= true;
	m_bFrustumOutOfDate		= true;

	updateFrustum();
}

// -----------------------------------------------------------------------------------------
CPepeEngineFrustum::~CPepeEngineFrustum()
{
}

// -----------------------------------------------------------------------------------------
void CPepeEngineFrustum::setFOVy(const Radian& fovy)
{
	m_fFOVy				= fovy;
	m_bFrustumOutOfDate	= true;	
}

// -----------------------------------------------------------------------------------------
void CPepeEngineFrustum::setNearClipDistance(float fNearDist)
{
	if (fNearDist < 0)
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_INVALID_PARAMETERS, 
			_T("Near clip distance must be greater than zero."), 
			_T("CPepeEngineFrustum::setNearClipDistance")
		);	
	}
	m_fNearDist			= fNearDist;
	m_bFrustumOutOfDate	= true;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineFrustum::setFarClipDistance(float fFarDist)
{
	m_fFarDist			= fFarDist;
	m_bFrustumOutOfDate	= true;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineFrustum::setAspectRatio(float fAspect)
{
	m_fAspect			= fAspect;
	m_bFrustumOutOfDate	= true;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineFrustum::setProjectionType(ProjectionType projectionType)
{
	m_projectionType	= projectionType;
	m_bFrustumOutOfDate	= true;
}

// -----------------------------------------------------------------------------------------
const CPepeEnginePlane* CPepeEngineFrustum::getFrustumPlanes(void)
{
	updateFrustumPlanes(CPepeEngineMatrix4::IDENTITY);

	return m_frustumPlanes;
}

// -----------------------------------------------------------------------------------------
const CPepeEnginePlane& CPepeEngineFrustum::getFrustumPlane(unsigned short nPlane)
{
	updateFrustumPlanes(CPepeEngineMatrix4::IDENTITY);

	return m_frustumPlanes[nPlane];
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineFrustum::isVisible(const CPepeEngineAxisAlignedBox& bound, FrustumPlane* culledBy) const
{
	// Null boxes always invisible
	if (bound.isNull()) return false;

	// Infinite boxes always visible
	if (bound.isInfinite()) return true;

	// Make any pending updates to the calculated frustum planes
	updateFrustumPlanes(CPepeEngineMatrix4::IDENTITY);

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
bool CPepeEngineFrustum::isVisible(const CPepeEngineSphere& sphere, FrustumPlane* culledBy) const
{
	// Make any pending updates to the calculated frustum planes
	updateFrustumPlanes(CPepeEngineMatrix4::IDENTITY);

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
bool CPepeEngineFrustum::isVisible(const CPepeEngineVector3& vert, FrustumPlane* culledBy) const
{
	// Make any pending updates to the calculated frustum planes
	updateFrustumPlanes(CPepeEngineMatrix4::IDENTITY);

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

// -----------------------------------------------------------------------------------------
CPepeEngineMatrix4 CPepeEngineFrustum::getProjectionMatrix()
{
	updateFrustum();

	return m_projectionMatrix;
}

// -----------------------------------------------------------------------------------------
CPepeEngineMatrix4 CPepeEngineFrustum::getProjectionMatrixRS()
{
	updateFrustum();

	return m_projectionMatrixRS;
}

_PEPE_ENGINE_END