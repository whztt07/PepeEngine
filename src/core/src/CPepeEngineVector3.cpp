#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineVector3.h"

_PEPE_ENGINE_START

_PepeEngineExport const CPepeEngineVector3 CPepeEngineVector3::ZERO(0, 0, 0);
_PepeEngineExport const CPepeEngineVector3 CPepeEngineVector3::UNIT_X(1, 0, 0);
_PepeEngineExport const CPepeEngineVector3 CPepeEngineVector3::UNIT_Y(0, 1, 0);
_PepeEngineExport const CPepeEngineVector3 CPepeEngineVector3::UNIT_Z(0, 0, 1);
_PepeEngineExport const CPepeEngineVector3 CPepeEngineVector3::NEGATIVE_UNIT_X(-1,  0,  0);
_PepeEngineExport const CPepeEngineVector3 CPepeEngineVector3::NEGATIVE_UNIT_Y( 0, -1,  0);
_PepeEngineExport const CPepeEngineVector3 CPepeEngineVector3::NEGATIVE_UNIT_Z( 0,  0, -1);
_PepeEngineExport const CPepeEngineVector3 CPepeEngineVector3::UNIT_SCALE(1, 1, 1);

// -----------------------------------------------------------------------------------------
float CPepeEngineVector3::length() const
{
	return CPepeEngineMath::Sqrt(x * x + y * y + z * z);
}

// -----------------------------------------------------------------------------------------
float CPepeEngineVector3::absDotProduct(const CPepeEngineVector3& vec) const
{
	return CPepeEngineMath::Abs(x * vec.x) + CPepeEngineMath::Abs(y * vec.y) + CPepeEngineMath::Abs(z * vec.z);
}

// -----------------------------------------------------------------------------------------
float CPepeEngineVector3::normalise()
{
	float fLength = CPepeEngineMath::Sqrt(x * x + y * y + z * z);

	// Will also work for zero-sized vectors, but will change nothing
	if (fLength > 1e-08)
	{
		float fInvLength = 1.0f / fLength;
		x *= fInvLength;
		y *= fInvLength;
		z *= fInvLength;
	}

	return fLength;
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEngineVector3::perpendicular(void) const
{
	static const float fSquareZero = float(1e-06 * 1e-06);

	CPepeEngineVector3 perp = this->crossProduct(CPepeEngineVector3::UNIT_X);

	// Check length
	if(perp.squaredLength() < fSquareZero)
	{
		/**
		*	This vector is the Y axis multiplied by a scalar, so we have
		*	to use another axis.
		*/
		perp = this->crossProduct(CPepeEngineVector3::UNIT_Y);
	}
	perp.normalise();

	return perp;
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEngineVector3::randomDeviant(
	const Radian& angle,
	const CPepeEngineVector3& up) const
{
	CPepeEngineVector3 newUp;

	if (up == CPepeEngineVector3::ZERO)
	{
		// Generate an up vector
		newUp = this->perpendicular();
	}
	else
	{
		newUp = up;
	}

	// Rotate up vector by random amount around this
	CPepeEngineQuaternion q;
	q.fromAngleAxis(Radian(CPepeEngineMath::unitRandom() * CPepeEngineMath::TWO_PI), *this);
	newUp = q * newUp;

	// Finally rotate this by given angle around randomised up
	q.fromAngleAxis(angle, newUp);
	return q * (*this);
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEngineVector3::randomDeviant(float angle, const CPepeEngineVector3& up) const
{
	return randomDeviant(Radian(angle), up);
}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion CPepeEngineVector3::getRotationTo(
		const CPepeEngineVector3& dest,
		const CPepeEngineVector3& fallbackAxis) const
{
	// Based on Stan Melax's article in Game Programming Gems
	CPepeEngineQuaternion q;
	// Copy, since cannot modify local
	CPepeEngineVector3 v0 = *this;
	CPepeEngineVector3 v1 = dest;
	v0.normalise();
	v1.normalise();

	float d = v0.dotProduct(v1);
	// If dot == 1, vectors are the same
	if (d >= 1.0f)
	{
		return CPepeEngineQuaternion::IDENTITY;
	}
	if (d < (1e-6f - 1.0f))
	{
		if (fallbackAxis != CPepeEngineVector3::ZERO)
		{
			// rotate 180 degrees about the fallback axis
			q.fromAngleAxis(Radian(CPepeEngineMath::PI), fallbackAxis);
		}
		else
		{
			// Generate an axis
			CPepeEngineVector3 axis = CPepeEngineVector3::UNIT_X.crossProduct(*this);
			if (axis.isZeroLength()) // pick another if colinear
			{
				axis = CPepeEngineVector3::UNIT_Y.crossProduct(*this);
			}

			axis.normalise();
			q.fromAngleAxis(Radian(CPepeEngineMath::PI), axis);
		}
	}
	else
	{
		float s = CPepeEngineMath::Sqrt((1+d)*2);
		float invs = 1 / s;

		CPepeEngineVector3 c = v0.crossProduct(v1);

		q.x = c.x * invs;
		q.y = c.y * invs;
		q.z = c.z * invs;
		q.w = s * 0.5f;
		q.normalise();
	}
	return q;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineVector3::positionEquals(const CPepeEngineVector3& rhs, float tolerance) const
{
	return CPepeEngineMath::floatEqual(x, rhs.x, tolerance) &&
		CPepeEngineMath::floatEqual(y, rhs.y, tolerance) &&
		CPepeEngineMath::floatEqual(z, rhs.z, tolerance);

}

// -----------------------------------------------------------------------------------------
bool CPepeEngineVector3::directionEquals(const CPepeEngineVector3& rhs, const Radian& tolerance) const
{
	float dot		= dotProduct(rhs);
	Radian angle	= CPepeEngineMath::ACos(dot);

	return CPepeEngineMath::Abs(angle.valueRadians()) <= tolerance.valueRadians();
}

_PEPE_ENGINE_END