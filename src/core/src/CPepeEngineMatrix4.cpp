#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineMatrix4.h"

_PEPE_ENGINE_START

_PepeEngineExport const CPepeEngineMatrix4 CPepeEngineMatrix4::ZERO(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0 
);

_PepeEngineExport const CPepeEngineMatrix4 CPepeEngineMatrix4::IDENTITY(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1 
);

_PepeEngineExport const CPepeEngineMatrix4 CPepeEngineMatrix4::CLIPSPACE2DTOIMAGESPACE(
	0.5,	0,		0,	0.5, 
	0,		-0.5,	0,	0.5, 
	0,		0,		1,	0,
	0,		0,		0,	1
);

// -----------------------------------------------------------------------------------------
inline static float MINOR(
	const CPepeEngineMatrix4& m, const size_t r0, const size_t r1, const size_t r2, 
	const size_t c0, const size_t c1, const size_t c2)
{
	return m[r0][c0] * (m[r1][c1] * m[r2][c2] - m[r2][c1] * m[r1][c2]) -
		m[r0][c1] * (m[r1][c0] * m[r2][c2] - m[r2][c0] * m[r1][c2]) +
		m[r0][c2] * (m[r1][c0] * m[r2][c1] - m[r2][c0] * m[r1][c1]);
}

// -----------------------------------------------------------------------------------------
CPepeEngineMatrix4::CPepeEngineMatrix4(const CPepeEngineMatrix3& m3x3)
{
	operator=(IDENTITY);
	operator=(m3x3);
}

// -----------------------------------------------------------------------------------------
CPepeEngineMatrix4::CPepeEngineMatrix4(const CPepeEngineQuaternion& rot)
{
	CPepeEngineMatrix3 m3x3;
	rot.toRotationMatrix(m3x3);
	operator=(IDENTITY);
	operator=(m3x3);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMatrix4::operator=(const CPepeEngineMatrix3& mat3)
{
	m[0][0] = mat3.m[0][0]; m[0][1] = mat3.m[0][1]; m[0][2] = mat3.m[0][2];
	m[1][0] = mat3.m[1][0]; m[1][1] = mat3.m[1][1]; m[1][2] = mat3.m[1][2];
	m[2][0] = mat3.m[2][0]; m[2][1] = mat3.m[2][1]; m[2][2] = mat3.m[2][2];
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMatrix4::extract3x3Matrix(CPepeEngineMatrix3& m3x3) const
{
	m3x3.m[0][0] = m[0][0];
	m3x3.m[0][1] = m[0][1];
	m3x3.m[0][2] = m[0][2];
	m3x3.m[1][0] = m[1][0];
	m3x3.m[1][1] = m[1][1];
	m3x3.m[1][2] = m[1][2];
	m3x3.m[2][0] = m[2][0];
	m3x3.m[2][1] = m[2][1];
	m3x3.m[2][2] = m[2][2];

}

// -----------------------------------------------------------------------------------------
 CPepeEngineQuaternion CPepeEngineMatrix4::extractQuaternion() const
 {
	 CPepeEngineMatrix3 m3x3;
	 extract3x3Matrix(m3x3);
	 return CPepeEngineQuaternion(m3x3);
 }

// -----------------------------------------------------------------------------------------
CPepeEngineMatrix4 CPepeEngineMatrix4::adjoint() const
{
	return CPepeEngineMatrix4( 
		MINOR(*this, 1, 2, 3, 1, 2, 3),
		-MINOR(*this, 0, 2, 3, 1, 2, 3),
		MINOR(*this, 0, 1, 3, 1, 2, 3),
		-MINOR(*this, 0, 1, 2, 1, 2, 3),

		-MINOR(*this, 1, 2, 3, 0, 2, 3),
		MINOR(*this, 0, 2, 3, 0, 2, 3),
		-MINOR(*this, 0, 1, 3, 0, 2, 3),
		MINOR(*this, 0, 1, 2, 0, 2, 3),

		MINOR(*this, 1, 2, 3, 0, 1, 3),
		-MINOR(*this, 0, 2, 3, 0, 1, 3),
		MINOR(*this, 0, 1, 3, 0, 1, 3),
		-MINOR(*this, 0, 1, 2, 0, 1, 3),

		-MINOR(*this, 1, 2, 3, 0, 1, 2),
		MINOR(*this, 0, 2, 3, 0, 1, 2),
		-MINOR(*this, 0, 1, 3, 0, 1, 2),
		MINOR(*this, 0, 1, 2, 0, 1, 2)
	);
}

// -----------------------------------------------------------------------------------------
float CPepeEngineMatrix4::determinant() const
{
	return	m[0][0] * MINOR(*this, 1, 2, 3, 1, 2, 3) -
			m[0][1] * MINOR(*this, 1, 2, 3, 0, 2, 3) +
			m[0][2] * MINOR(*this, 1, 2, 3, 0, 1, 3) -
			m[0][3] * MINOR(*this, 1, 2, 3, 0, 1, 2);
}

// -----------------------------------------------------------------------------------------
CPepeEngineMatrix4 CPepeEngineMatrix4::inverse() const
{
	float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
	float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
	float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
	float m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

	float v0 = m20 * m31 - m21 * m30;
	float v1 = m20 * m32 - m22 * m30;
	float v2 = m20 * m33 - m23 * m30;
	float v3 = m21 * m32 - m22 * m31;
	float v4 = m21 * m33 - m23 * m31;
	float v5 = m22 * m33 - m23 * m32;

	float t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
	float t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
	float t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
	float t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

	float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

	float d00 = t00 * invDet;
	float d10 = t10 * invDet;
	float d20 = t20 * invDet;
	float d30 = t30 * invDet;

	float d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m10 * m31 - m11 * m30;
	v1 = m10 * m32 - m12 * m30;
	v2 = m10 * m33 - m13 * m30;
	v3 = m11 * m32 - m12 * m31;
	v4 = m11 * m33 - m13 * m31;
	v5 = m12 * m33 - m13 * m32;

	float d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m21 * m10 - m20 * m11;
	v1 = m22 * m10 - m20 * m12;
	v2 = m23 * m10 - m20 * m13;
	v3 = m22 * m11 - m21 * m12;
	v4 = m23 * m11 - m21 * m13;
	v5 = m23 * m12 - m22 * m13;

	float d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	return CPepeEngineMatrix4(
		d00, d01, d02, d03,
		d10, d11, d12, d13,
		d20, d21, d22, d23,
		d30, d31, d32, d33
	);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMatrix4::makeTransform(const CPepeEngineVector3& position, const CPepeEngineVector3& scale, const CPepeEngineQuaternion& orientation)
{
	// Ordering:
	//    1. Scale
	//    2. Rotate
	//    3. Translate

	CPepeEngineMatrix3 rot3x3, scale3x3;
	orientation.toRotationMatrix(rot3x3);

	scale3x3		= CPepeEngineMatrix3::ZERO;
	scale3x3[0][0]	= scale.x;
	scale3x3[1][1]	= scale.y;
	scale3x3[2][2]	= scale.z;

	// Set up final matrix with scale, rotation and translation
	*this = rot3x3 * scale3x3;
	this->setTrans(position);

	// No projection term
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMatrix4::makeInverseTransform(const CPepeEngineVector3& position, const CPepeEngineVector3& scale, const CPepeEngineQuaternion& orientation)
{
	// Invert the parameters
	CPepeEngineVector3 invTranslate = -position;
	CPepeEngineVector3 invScale(1 / scale.x, 1 / scale.y, 1 / scale.z);
	CPepeEngineQuaternion invRot = orientation.inverse();

	// Because we're inverting, order is translation, rotation, scale
	// So make translation relative to scale & rotation
	invTranslate *= invScale; // scale
	invTranslate = invRot * invTranslate; // rotate

	// Next, make a 3x3 rotation matrix and apply inverse scale
	CPepeEngineMatrix3 rot3x3, scale3x3;
	invRot.toRotationMatrix(rot3x3);
	scale3x3		= CPepeEngineMatrix3::ZERO;
	scale3x3[0][0]	= invScale.x;
	scale3x3[1][1]	= invScale.y;
	scale3x3[2][2]	= invScale.z;

	// Set up final matrix with scale, rotation and translation
	*this = scale3x3 * rot3x3;
	this->setTrans(invTranslate);

	// No projection term
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}

// -----------------------------------------------------------------------------------------
CPepeEngineMatrix4 CPepeEngineMatrix4::inverseAffine(void) const
{
	assert(isAffine());

	float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2];
	float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2];

	float t00 = m22 * m11 - m21 * m12;
	float t10 = m20 * m12 - m22 * m10;
	float t20 = m21 * m10 - m20 * m11;

	float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2];

	float invDet = 1 / (m00 * t00 + m01 * t10 + m02 * t20);

	t00 *= invDet; t10 *= invDet; t20 *= invDet;

	m00 *= invDet; m01 *= invDet; m02 *= invDet;

	float r00 = t00;
	float r01 = m02 * m21 - m01 * m22;
	float r02 = m01 * m12 - m02 * m11;

	float r10 = t10;
	float r11 = m00 * m22 - m02 * m20;
	float r12 = m02 * m10 - m00 * m12;

	float r20 = t20;
	float r21 = m01 * m20 - m00 * m21;
	float r22 = m00 * m11 - m01 * m10;

	float m03 = m[0][3], m13 = m[1][3], m23 = m[2][3];

	float r03 = - (r00 * m03 + r01 * m13 + r02 * m23);
	float r13 = - (r10 * m03 + r11 * m13 + r12 * m23);
	float r23 = - (r20 * m03 + r21 * m13 + r22 * m23);

	return CPepeEngineMatrix4(
		r00, r01, r02, r03,
		r10, r11, r12, r13,
		r20, r21, r22, r23,
		0,   0,   0,   1
	);
}

_PEPE_ENGINE_END