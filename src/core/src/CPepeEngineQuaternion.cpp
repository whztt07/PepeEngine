#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineQuaternion.h"
#include "CPepeEngineVector3.h"

_PEPE_ENGINE_START

const float CPepeEngineQuaternion::ms_fEpsilon = float(1e-03);
const CPepeEngineQuaternion CPepeEngineQuaternion::ZERO(0.0, 0.0, 0.0, 0.0);
const CPepeEngineQuaternion CPepeEngineQuaternion::IDENTITY(1.0, 0.0, 0.0, 0.0);

// -----------------------------------------------------------------------------------------
void CPepeEngineQuaternion::fromRotationMatrix(const CPepeEngineMatrix3& kRot)
{
	/** 
	 *	Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	 *	article "Quaternion Calculus and Fast Animation".
	 */

	float fTrace = kRot[0][0] + kRot[1][1] + kRot[2][2];
	float fRoot;

	if (fTrace > 0.0)
	{
		// |w| > 1/2, may as well choose w > 1/2
		fRoot	= CPepeEngineMath::Sqrt(fTrace + 1.0f);  // 2w
		w		= 0.5f * fRoot;
		fRoot	= 0.5f / fRoot;  // 1/(4w)
		x		= (kRot[2][1] - kRot[1][2]) * fRoot;
		y		= (kRot[0][2] - kRot[2][0]) * fRoot;
		z		= (kRot[1][0] - kRot[0][1]) * fRoot;
	}
	else
	{
		// |w| <= 1/2
		static size_t s_iNext[3]	= {1, 2, 0};
		size_t i					= 0;

		if (kRot[1][1] > kRot[0][0])
		{
			i = 1;
		}

		if (kRot[2][2] > kRot[i][i])
		{
			i = 2;
		}

		size_t j = s_iNext[i];
		size_t k = s_iNext[j];

		fRoot				= CPepeEngineMath::Sqrt(kRot[i][i] - kRot[j][j] - kRot[k][k] + 1.0f);
		float* apkQuat[3]	= {&x, &y, &z};
		*apkQuat[i]			= 0.5f * fRoot;
		fRoot				= 0.5f / fRoot;
		w					= (kRot[k][j] - kRot[j][k]) * fRoot;
		*apkQuat[j]			= (kRot[j][i] + kRot[i][j]) * fRoot;
		*apkQuat[k]			= (kRot[k][i] + kRot[i][k]) * fRoot;
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineQuaternion::toRotationMatrix(CPepeEngineMatrix3& kRot) const
{
	float fTx  = 2.0f * x;
	float fTy  = 2.0f * y;
	float fTz  = 2.0f * z;
	float fTwx = fTx * w;
	float fTwy = fTy * w;
	float fTwz = fTz * w;
	float fTxx = fTx * x;
	float fTxy = fTy * x;
	float fTxz = fTz * x;
	float fTyy = fTy * y;
	float fTyz = fTz * y;
	float fTzz = fTz * z;

	kRot[0][0] = 1.0f - (fTyy + fTzz);
	kRot[0][1] = fTxy - fTwz;
	kRot[0][2] = fTxz + fTwy;
	kRot[1][0] = fTxy + fTwz;
	kRot[1][1] = 1.0f - (fTxx + fTzz);
	kRot[1][2] = fTyz - fTwx;
	kRot[2][0] = fTxz - fTwy;
	kRot[2][1] = fTyz + fTwx;
	kRot[2][2] = 1.0f - (fTxx + fTyy);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineQuaternion::fromAngleAxis(const Radian& rfAngle, const CPepeEngineVector3& rkAxis)
{
	/** 
	 *	assert:  axis[] is unit length
	 *
	 *	The quaternion representing the rotation is
	 *	q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)
	 */

	Radian fHalfAngle(0.5 * rfAngle);
	float fSin = CPepeEngineMath::Sin(fHalfAngle);
	w = CPepeEngineMath::Cos(fHalfAngle);
	x = fSin * rkAxis.x;
	y = fSin * rkAxis.y;
	z = fSin * rkAxis.z;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineQuaternion::toAngleAxis(Radian& rfAngle, CPepeEngineVector3& rkAxis) const
{
	// The quaternion representing the rotation is
	//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

	float fSqrLength = x * x + y * y + z * z;
	if (fSqrLength > 0.0)
	{
		rfAngle = 2.0f * CPepeEngineMath::ACos(w);
		float fInvLength = CPepeEngineMath::invSqrt(fSqrLength);
		rkAxis.x = x * fInvLength;
		rkAxis.y = y * fInvLength;
		rkAxis.z = z * fInvLength;
	}
	else
	{
		// angle is 0 (mod 2*pi), so any axis will do
		rfAngle		= Radian(0.0);
		rkAxis.x	= 1.0;
		rkAxis.y	= 0.0;
		rkAxis.z	= 0.0;
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineQuaternion::toAngleAxis(Degree& dAngle, CPepeEngineVector3& rkAxis) const 
{
	Radian rAngle;
	toAngleAxis(rAngle, rkAxis);
	dAngle = rAngle;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineQuaternion::fromAngleAxis(const float& rfAngle, const CPepeEngineVector3& rkAxis)
{
	fromAngleAxis(Radian(rfAngle), rkAxis);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineQuaternion::fromAxes(const CPepeEngineVector3* akAxis)
{
	CPepeEngineMatrix3 kRot;

	for (size_t iCol = 0; iCol < 3; iCol++)
	{
		kRot[0][iCol] = akAxis[iCol].x;
		kRot[1][iCol] = akAxis[iCol].y;
		kRot[2][iCol] = akAxis[iCol].z;
	}

	fromRotationMatrix(kRot);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineQuaternion::fromAxes(const CPepeEngineVector3& xaxis, const CPepeEngineVector3& yaxis, const CPepeEngineVector3& zaxis)
{
	CPepeEngineMatrix3 kRot;

	kRot[0][0] = xaxis.x;
	kRot[1][0] = xaxis.y;
	kRot[2][0] = xaxis.z;

	kRot[0][1] = yaxis.x;
	kRot[1][1] = yaxis.y;
	kRot[2][1] = yaxis.z;

	kRot[0][2] = zaxis.x;
	kRot[1][2] = zaxis.y;
	kRot[2][2] = zaxis.z;

	fromRotationMatrix(kRot);

}

// -----------------------------------------------------------------------------------------
void CPepeEngineQuaternion::toAxes(CPepeEngineVector3* akAxis) const
{
	CPepeEngineMatrix3 kRot;

	toRotationMatrix(kRot);

	for (size_t iCol = 0; iCol < 3; iCol++)
	{
		akAxis[iCol].x = kRot[0][iCol];
		akAxis[iCol].y = kRot[1][iCol];
		akAxis[iCol].z = kRot[2][iCol];
	}
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEngineQuaternion::xAxis(void) const
{
	//float fTx  = 2.0*x;
	float fTy  = 2.0f * y;
	float fTz  = 2.0f * z;
	float fTwy = fTy * w;
	float fTwz = fTz * w;
	float fTxy = fTy * x;
	float fTxz = fTz * x;
	float fTyy = fTy * y;
	float fTzz = fTz * z;

	return CPepeEngineVector3(1.0f - (fTyy + fTzz), fTxy + fTwz, fTxz - fTwy);
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEngineQuaternion::yAxis(void) const
{
	float fTx  = 2.0f * x;
	float fTy  = 2.0f * y;
	float fTz  = 2.0f * z;
	float fTwx = fTx * w;
	float fTwz = fTz * w;
	float fTxx = fTx * x;
	float fTxy = fTy * x;
	float fTyz = fTz * y;
	float fTzz = fTz * z;

	return CPepeEngineVector3(fTxy - fTwz, 1.0f - (fTxx + fTzz), fTyz + fTwx);
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEngineQuaternion::zAxis(void) const
{
	float fTx  = 2.0f * x;
	float fTy  = 2.0f * y;
	float fTz  = 2.0f * z;
	float fTwx = fTx * w;
	float fTwy = fTy * w;
	float fTxx = fTx * x;
	float fTxz = fTz * x;
	float fTyy = fTy * y;
	float fTyz = fTz * y;

	return CPepeEngineVector3(fTxz + fTwy, fTyz - fTwx, 1.0f - (fTxx + fTyy));
}

// -----------------------------------------------------------------------------------------
void CPepeEngineQuaternion::toAxes(CPepeEngineVector3& xaxis, CPepeEngineVector3& yaxis, CPepeEngineVector3& zaxis) const
{
	CPepeEngineMatrix3 kRot;

	toRotationMatrix(kRot);

	xaxis.x = kRot[0][0];
	xaxis.y = kRot[1][0];
	xaxis.z = kRot[2][0];

	yaxis.x = kRot[0][1];
	yaxis.y = kRot[1][1];
	yaxis.z = kRot[2][1];

	zaxis.x = kRot[0][2];
	zaxis.y = kRot[1][2];
	zaxis.z = kRot[2][2];
}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion CPepeEngineQuaternion::operator+(const CPepeEngineQuaternion& rkQ) const
{
	return CPepeEngineQuaternion(w + rkQ.w, x + rkQ.x, y + rkQ.y, z + rkQ.z);
}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion CPepeEngineQuaternion::operator-(const CPepeEngineQuaternion& rkQ) const
{
	return CPepeEngineQuaternion(w - rkQ.w, x - rkQ.x, y - rkQ.y, z - rkQ.z);
}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion CPepeEngineQuaternion::operator*(const CPepeEngineQuaternion& rkQ) const
{
	// NOTE:  Multiplication is not generally commutative, so in most
	// cases p*q != q*p.

	return CPepeEngineQuaternion(
		w * rkQ.w - x * rkQ.x - y * rkQ.y - z * rkQ.z,
		w * rkQ.x + x * rkQ.w + y * rkQ.z - z * rkQ.y,
		w * rkQ.y + y * rkQ.w + z * rkQ.x - x * rkQ.z,
		w * rkQ.z + z * rkQ.w + x * rkQ.y - y * rkQ.x
	);
}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion CPepeEngineQuaternion::operator*(float fScalar) const
{
	return CPepeEngineQuaternion(fScalar * w, fScalar * x, fScalar * y, fScalar * z);
}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion CPepeEngineQuaternion::operator-() const
{
	return CPepeEngineQuaternion(-w, -x, -y, -z);
}

// -----------------------------------------------------------------------------------------
float CPepeEngineQuaternion::dot(const CPepeEngineQuaternion& rkQ) const
{
	return w * rkQ.w + x * rkQ.x + y * rkQ.y + z * rkQ.z;
}

// -----------------------------------------------------------------------------------------
float CPepeEngineQuaternion::norm() const
{
	return w * w + x * x + y * y + z * z;
}

// -----------------------------------------------------------------------------------------
float CPepeEngineQuaternion::normalise(void)
{
	float len		= norm();
	float factor	= 1.0f / CPepeEngineMath::Sqrt(len);
	*this = *this * factor;
	return len;
}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion CPepeEngineQuaternion::inverse() const
{
	float fNorm = w * w + x * x + y * y + z * z;
	if (fNorm > 0.0)
	{
		float fInvNorm = 1.0f / fNorm;
		return CPepeEngineQuaternion(w * fInvNorm, -x * fInvNorm, -y * fInvNorm, -z * fInvNorm);
	}
	else
	{		
		return ZERO;
	}
}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion CPepeEngineQuaternion::unitInverse() const
{
	// assert:  'this' is unit length
	return CPepeEngineQuaternion(w, -x, -y, -z);
}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion CPepeEngineQuaternion::exp() const
{
	// If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
	// exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
	// use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

	Radian fAngle(CPepeEngineMath::Sqrt(x * x + y * y + z * z));
	float fSin = CPepeEngineMath::Sin(fAngle);

	CPepeEngineQuaternion kResult;
	kResult.w = CPepeEngineMath::Cos(fAngle);

	if (CPepeEngineMath::Abs(fSin) >= ms_fEpsilon)
	{
		float fCoeff = fSin / (fAngle.valueRadians());
		kResult.x = fCoeff * x;
		kResult.y = fCoeff * y;
		kResult.z = fCoeff * z;
	}
	else
	{
		kResult.x = x;
		kResult.y = y;
		kResult.z = z;
	}

	return kResult;
}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion CPepeEngineQuaternion::log() const
{
	// If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
	// log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
	// sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

	CPepeEngineQuaternion kResult;
	kResult.w = 0.0;

	if (CPepeEngineMath::Abs(w) < 1.0)
	{
		Radian fAngle(CPepeEngineMath::ACos(w));
		float fSin = CPepeEngineMath::Sin(fAngle);
		if (CPepeEngineMath::Abs(fSin) >= ms_fEpsilon)
		{
			float fCoeff	= fAngle.valueRadians() / fSin;
			kResult.x		= fCoeff * x;
			kResult.y		= fCoeff * y;
			kResult.z		= fCoeff * z;
			return kResult;
		}
	}

	kResult.x = x;
	kResult.y = y;
	kResult.z = z;

	return kResult;
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3 CPepeEngineQuaternion::operator*(const CPepeEngineVector3& v) const
{
	// nVidia SDK implementation
	CPepeEngineVector3 uv, uuv;
	CPepeEngineVector3 qvec(x, y, z);
	uv	= qvec.crossProduct(v);
	uuv = qvec.crossProduct(uv);
	uv	*= (2.0f * w);
	uuv *= 2.0f;

	return v + uv + uuv;
}

// -----------------------------------------------------------------------------------------
Radian CPepeEngineQuaternion::getRoll(bool reprojectAxis) const
{
	if (reprojectAxis)
	{
		// roll = atan2(localx.y, localx.x)
		// pick parts of xAxis() implementation that we need
		float fTx  = 2.0f * x;
		float fTy  = 2.0f * y;
		float fTz  = 2.0f * z;
		float fTwz = fTz * w;
		float fTxy = fTy * x;
		float fTyy = fTy * y;
		float fTzz = fTz * z;

		// CPepeEngineVector3(1.0-(fTyy + fTzz), fTxy + fTwz, fTxz - fTwy);
		return Radian(CPepeEngineMath::ATan2(fTxy + fTwz, 1.0f - (fTyy + fTzz)));
	}
	else
	{
		return Radian(CPepeEngineMath::ATan2(2 * (x * y + w * z), w * w + x * x - y * y - z * z));
	}
}

// -----------------------------------------------------------------------------------------
Radian CPepeEngineQuaternion::getPitch(bool reprojectAxis) const
{
	if (reprojectAxis)
	{
		// pitch = atan2(localy.z, localy.y)
		// pick parts of yAxis() implementation that we need
		float fTx  = 2.0f * x;
		float fTy  = 2.0f * y;
		float fTz  = 2.0f * z;
		float fTwx = fTx * w;
		float fTxx = fTx * x;
		float fTyz = fTz * y;
		float fTzz = fTz * z;

		// CPepeEngineVector3(fTxy-fTwz, 1.0-(fTxx+fTzz), fTyz+fTwx);
		return Radian(CPepeEngineMath::ATan2(fTyz + fTwx, 1.0f - (fTxx + fTzz)));
	}
	else
	{
		// internal version
		return Radian(CPepeEngineMath::ATan2(2 * (y * z + w * x), w * w - x * x - y * y + z * z));
	}
}

// -----------------------------------------------------------------------------------------
Radian CPepeEngineQuaternion::getYaw(bool reprojectAxis) const
{
	if (reprojectAxis)
	{
		// yaw = atan2(localz.x, localz.z)
		// pick parts of zAxis() implementation that we need
		float fTx  = 2.0f * x;
		float fTy  = 2.0f * y;
		float fTz  = 2.0f * z;
		float fTwy = fTy * w;
		float fTxx = fTx * x;
		float fTxz = fTz * x;
		float fTyy = fTy * y;

		// CPepeEngineVector3(fTxz+fTwy, fTyz-fTwx, 1.0-(fTxx+fTyy));

		return Radian(CPepeEngineMath::ATan2(fTxz + fTwy, 1.0f - (fTxx + fTyy)));
	}
	else
	{
		// internal version
		return Radian(CPepeEngineMath::ASin(-2 * (x * z - w * y)));
	}
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineQuaternion::equals(const CPepeEngineQuaternion& rhs, const Radian& tolerance) const
{
	float fCos		= dot(rhs);
	Radian angle	= CPepeEngineMath::ACos(fCos);

	return (CPepeEngineMath::Abs(angle.valueRadians()) <= tolerance.valueRadians())	|| CPepeEngineMath::floatEqual(angle.valueRadians(), CPepeEngineMath::PI, tolerance.valueRadians());


}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion CPepeEngineQuaternion::slerp(
	float fT, const CPepeEngineQuaternion& rkP,
	const CPepeEngineQuaternion& rkQ, bool shortestPath)
{
	float fCos = rkP.dot(rkQ);
	CPepeEngineQuaternion rkT;

	// Do we need to invert rotation?
	if (fCos < 0.0f && shortestPath)
	{
		fCos	= -fCos;
		rkT		= -rkQ;
	}
	else
	{
		rkT = rkQ;
	}

	if (CPepeEngineMath::Abs(fCos) < 1 - ms_fEpsilon)
	{
		// Standard case (slerp)
		float fSin		= CPepeEngineMath::Sqrt(1 - CPepeEngineMath::Sqr(fCos));
		Radian fAngle	= CPepeEngineMath::ATan2(fSin, fCos);
		float fInvSin	= 1.0f / fSin;
		float fCoeff0	= CPepeEngineMath::Sin((1.0f - fT) * fAngle) * fInvSin;
		float fCoeff1	= CPepeEngineMath::Sin(fT * fAngle) * fInvSin;
		return fCoeff0 * rkP + fCoeff1 * rkT;
	}
	else
	{
		// There are two situations:
		// 1. "rkP" and "rkQ" are very close (fCos ~= +1), so we can do a linear
		//    interpolation safely.
		// 2. "rkP" and "rkQ" are almost inverse of each other (fCos ~= -1), there
		//    are an infinite number of possibilities interpolation. but we haven't
		//    have method to fix this case, so just use linear interpolation here.
		CPepeEngineQuaternion t = (1.0f - fT) * rkP + fT * rkT;
		// taking the complement requires renormalisation
		t.normalise();
		return t;
	}
}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion CPepeEngineQuaternion::slerpExtraSpins (
	float fT, const CPepeEngineQuaternion& rkP, const CPepeEngineQuaternion& rkQ, int iExtraSpins)
{
	float fCos = rkP.dot(rkQ);
	Radian fAngle(CPepeEngineMath::ACos(fCos));

	if (CPepeEngineMath::Abs(fAngle.valueRadians()) < ms_fEpsilon)
	{
		return rkP;
	}

	float fSin = CPepeEngineMath::Sin(fAngle);
	Radian fPhase(CPepeEngineMath::PI * iExtraSpins * fT);
	float fInvSin = 1.0f / fSin;
	float fCoeff0 = CPepeEngineMath::Sin((1.0f - fT) * fAngle - fPhase) * fInvSin;
	float fCoeff1 = CPepeEngineMath::Sin(fT * fAngle + fPhase) * fInvSin;
	return fCoeff0 * rkP + fCoeff1 * rkQ;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineQuaternion::intermediate(
	const CPepeEngineQuaternion& rkQ0, const CPepeEngineQuaternion& rkQ1, const CPepeEngineQuaternion& rkQ2,
	CPepeEngineQuaternion& rkA, CPepeEngineQuaternion& rkB)
{
	// assert:  q0, q1, q2 are unit quaternions

	CPepeEngineQuaternion kQ0inv	= rkQ0.unitInverse();
	CPepeEngineQuaternion kQ1inv	= rkQ1.unitInverse();
	CPepeEngineQuaternion rkP0		= kQ0inv*rkQ1;
	CPepeEngineQuaternion rkP1		= kQ1inv * rkQ2;
	CPepeEngineQuaternion kArg		= 0.25 * (rkP0.log() - rkP1.log());
	CPepeEngineQuaternion kMinusArg = -kArg;

	rkA = rkQ1 * kArg.exp();
	rkB = rkQ1 * kMinusArg.exp();
}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion CPepeEngineQuaternion::squad(
	float fT, const CPepeEngineQuaternion& rkP, const CPepeEngineQuaternion& rkA,
	const CPepeEngineQuaternion& rkB, const CPepeEngineQuaternion& rkQ, bool shortestPath)
{
	float fSlerpT = 2.0f * fT * (1.0f - fT);
	CPepeEngineQuaternion kSlerpP = slerp(fT, rkP, rkQ, shortestPath);
	CPepeEngineQuaternion kSlerpQ = slerp(fT, rkA, rkB);
	return slerp(fSlerpT, kSlerpP ,kSlerpQ);
}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion CPepeEngineQuaternion::nlerp(
	float fT, const CPepeEngineQuaternion& rkP,
	const CPepeEngineQuaternion& rkQ, bool shortestPath
)
{
	CPepeEngineQuaternion result;
	float fCos = rkP.dot(rkQ);
	if (fCos < 0.0f && shortestPath)
	{
		result = rkP + fT * ((-rkQ) - rkP);
	}
	else
	{
		result = rkP + fT * (rkQ - rkP);
	}
	result.normalise();
	return result;
}

_PEPE_ENGINE_END