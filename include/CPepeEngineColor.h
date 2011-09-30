/**
 * Project:		PepeEngine
 * Tier:		Frontend
 * File:		CPepeEngineColor.h   
 *
 * @brief		Declaration of CPepeEngineColor class.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-05-12
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef CPEPEENGINECOLOR_H
#define CPEPEENGINECOLOR_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

typedef unsigned int RGBA;
typedef unsigned int ARGB;
typedef unsigned int ABGR;
typedef unsigned int BGRA;

class _PepeEngineExport CPepeEngineColor
{	
public:

	static const CPepeEngineColor BLACK;
	static const CPepeEngineColor WHITE;

	float r;
	float g;
	float b;
	float a;

	explicit CPepeEngineColor(float fR = 1.0f, float fG = 1.0f, float fB = 1.0f, float fA = 1.0f) : r(fR), g(fG), b(fB), a(fA) {}
	~CPepeEngineColor() {}

    bool operator==(const CPepeEngineColor& rhs) const;
    bool operator!=(const CPepeEngineColor& rhs) const;
    
    RGBA getAsRGBA(void) const;
    ARGB getAsARGB(void) const;
	BGRA getAsBGRA(void) const;
    ABGR getAsABGR(void) const;
	void setAsRGBA(const RGBA val);
    void setAsARGB(const ARGB val);
	void setAsBGRA(const BGRA val);
    void setAsABGR(const ABGR val);

	void saturate(void);
	CPepeEngineColor saturateCopy(void) const;

	inline void set(float r, float g, float b, float a = 1.0f)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	inline float operator [](const size_t i) const
	{
		assert(i < 4);
		return *(&r + i);
	}
		
	inline float& operator [] (const size_t i)
	{
		assert(i < 4);

		return *(&r + i);
	}
		
	inline float* ptr()
	{
		return &r;
	}

	inline const float* ptr() const
	{
		return &r;
	}
			
    inline CPepeEngineColor operator +(const CPepeEngineColor& rkVector) const
    {
		CPepeEngineColor kSum;

		kSum.r = r + rkVector.r;
		kSum.g = g + rkVector.g;
		kSum.b = b + rkVector.b;
		kSum.a = a + rkVector.a;

		return kSum;
	}

	inline CPepeEngineColor operator -(const CPepeEngineColor& rkVector) const
	{
		CPepeEngineColor kDiff;

		kDiff.r = r - rkVector.r;
		kDiff.g = g - rkVector.g;
		kDiff.b = b - rkVector.b;
		kDiff.a = a - rkVector.a;

		return kDiff;
	}

	inline CPepeEngineColor operator *(const float fScalar) const
	{
		CPepeEngineColor kProd;

		kProd.r = fScalar * r;
		kProd.g = fScalar * g;
		kProd.b = fScalar * b;
		kProd.a = fScalar * a;

		return kProd;
	}

	inline CPepeEngineColor operator *(const CPepeEngineColor& rhs) const
	{
		CPepeEngineColor kProd;

		kProd.r = rhs.r * r;
		kProd.g = rhs.g * g;
		kProd.b = rhs.b * b;
		kProd.a = rhs.a * a;

		return kProd;
	}

	inline CPepeEngineColor operator /(const CPepeEngineColor& rhs) const
	{
		CPepeEngineColor kProd;

		kProd.r = rhs.r / r;
		kProd.g = rhs.g / g;
		kProd.b = rhs.b / b;
		kProd.a = rhs.a / a;

		return kProd;
	}

    inline CPepeEngineColor operator /(const float fScalar) const
	{
		assert(fScalar != 0.0);

		CPepeEngineColor kDiv;

		float fInv = 1.0f / fScalar;
		kDiv.r = r * fInv;
		kDiv.g = g * fInv;
		kDiv.b = b * fInv;
		kDiv.a = a * fInv;

		return kDiv;
	}

	_PepeEngineExport inline friend CPepeEngineColor operator *(const float fScalar, const CPepeEngineColor& rkVector)
	{
		CPepeEngineColor kProd;

		kProd.r = fScalar * rkVector.r;
		kProd.g = fScalar * rkVector.g;
		kProd.b = fScalar * rkVector.b;
		kProd.a = fScalar * rkVector.a;

		return kProd;
	}
        
	inline CPepeEngineColor& operator +=(const CPepeEngineColor& rkVector)
    {
		r += rkVector.r;
		g += rkVector.g;
		b += rkVector.b;
		a += rkVector.a;

		return *this;
	}

	inline CPepeEngineColor& operator -=(const CPepeEngineColor& rkVector)
	{
		r -= rkVector.r;
		g -= rkVector.g;
		b -= rkVector.b;
		a -= rkVector.a;

		return *this;
	}

	inline CPepeEngineColor& operator *=(const float fScalar)
	{
		r *= fScalar;
		g *= fScalar;
		b *= fScalar;
		a *= fScalar;
		return *this;
	}

	inline CPepeEngineColor& operator /=(const float fScalar)
	{
		assert(fScalar != 0.0);

		float fInv = 1.0f / fScalar;

		r *= fInv;
		g *= fInv;
		b *= fInv;
		a *= fInv;

		return *this;
	}
};

_PEPE_ENGINE_END

#endif