#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineColor.h"

_PEPE_ENGINE_START

const CPepeEngineColor CPepeEngineColor::BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const CPepeEngineColor CPepeEngineColor::WHITE(1.0f, 1.0f, 1.0f, 1.0f);

// -----------------------------------------------------------------------------------------
bool CPepeEngineColor::operator ==(const CPepeEngineColor& rhs) const
{
    return (r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a);
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineColor::operator!=(const CPepeEngineColor& rhs) const
{
    return !(*this == rhs);
}

// -----------------------------------------------------------------------------------------
RGBA CPepeEngineColor::getAsRGBA(void) const
{
    unsigned char   val8;
    unsigned int    val32 = 0;

    // Convert to 32bit pattern
    // (RGBA = 8888)

    // Red
    val8    = static_cast<unsigned char>(r * 255);
    val32   = val8 << 24;

    // Green
    val8    = static_cast<unsigned char>(g * 255);
    val32   += val8 << 16;

    // Blue
    val8    = static_cast<unsigned char>(b * 255);
    val32   += val8 << 8;

    // Alpha
    val8    = static_cast<unsigned char>(a * 255);
    val32   += val8;

    return val32;
}

// -----------------------------------------------------------------------------------------
ARGB CPepeEngineColor::getAsARGB(void) const
{
    unsigned char   val8;
    unsigned int    val32 = 0;

    // Convert to 32bit pattern
    // (ARGB = 8888)

    // Alpha
    val8    = static_cast<unsigned char>(a * 255);
    val32   = val8 << 24;

    // Red
    val8    = static_cast<unsigned char>(r * 255);
    val32   += val8 << 16;

    // Green
    val8    = static_cast<unsigned char>(g * 255);
    val32   += val8 << 8;

    // Blue
    val8 = static_cast<unsigned char>(b * 255);
    val32 += val8;

    return val32;
}

// -----------------------------------------------------------------------------------------
BGRA CPepeEngineColor::getAsBGRA(void) const
{
    unsigned char   val8;
    unsigned int    val32 = 0;

    // Convert to 32bit pattern
    // (ARGB = 8888)

    // Blue
    val8    = static_cast<unsigned char>(b * 255);
    val32   = val8 << 24;

    // Green
    val8    = static_cast<unsigned char>(g * 255);
    val32   += val8 << 16;

    // Red
    val8    = static_cast<unsigned char>(r * 255);
    val32   += val8 << 8;

    // Alpha
    val8    = static_cast<unsigned char>(a * 255);
    val32   += val8;

    return val32;
}

// -----------------------------------------------------------------------------------------
ABGR CPepeEngineColor::getAsABGR(void) const
{
    unsigned char   val8;
    unsigned int    val32 = 0;

    // Convert to 32bit pattern
    // (ABRG = 8888)

    // Alpha
    val8    = static_cast<unsigned char>(a * 255);
    val32   = val8 << 24;

    // Blue
    val8    = static_cast<unsigned char>(b * 255);
    val32   += val8 << 16;

    // Green
    val8    = static_cast<unsigned char>(g * 255);
    val32   += val8 << 8;

    // Red
    val8    = static_cast<unsigned char>(r * 255);
    val32   += val8;

    return val32;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineColor::setAsRGBA(const RGBA val)
{
    unsigned int val32 = val;

    // Convert from 32bit pattern
    // (RGBA = 8888)

    // Red
    r = ((val32 >> 24) & 0xFF) / 255.0f;

    // Green
    g = ((val32 >> 16) & 0xFF) / 255.0f;

    // Blue
    b = ((val32 >> 8) & 0xFF) / 255.0f;

    // Alpha
    a = (val32 & 0xFF) / 255.0f;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineColor::setAsARGB(const ARGB val)
{
    unsigned int val32 = val;

    // Convert from 32bit pattern
    // (ARGB = 8888)

    // Alpha
    a = ((val32 >> 24) & 0xFF) / 255.0f;

    // Red
    r = ((val32 >> 16) & 0xFF) / 255.0f;

    // Green
    g = ((val32 >> 8) & 0xFF) / 255.0f;

    // Blue
    b = (val32 & 0xFF) / 255.0f;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineColor::setAsBGRA(const BGRA val)
{
    unsigned int val32 = val;

    // Convert from 32bit pattern
    // (ARGB = 8888)

    // Blue
    b = ((val32 >> 24) & 0xFF) / 255.0f;

    // Green
    g = ((val32 >> 16) & 0xFF) / 255.0f;

    // Red
    r = ((val32 >> 8) & 0xFF) / 255.0f;

    // Alpha
    a = (val32 & 0xFF) / 255.0f;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineColor::setAsABGR(const ABGR val)
{
    unsigned int val32 = val;

    // Convert from 32bit pattern
    // (ABGR = 8888)

    // Alpha
    a = ((val32 >> 24) & 0xFF) / 255.0f;

    // Blue
    b = ((val32 >> 16) & 0xFF) / 255.0f;

    // Green
    g = ((val32 >> 8) & 0xFF) / 255.0f;

    // Red
    r = (val32 & 0xFF) / 255.0f;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineColor::saturate(void)
{
    if (r < 0) {
        r = 0;
    } else if (r > 1) {
        r = 1;
    }

    if (g < 0) {
        g = 0;
    } else if (g > 1) {
        g = 1;
    }

    if (b < 0) {
        b = 0;
    } else if (b > 1) {
        b = 1;
    }

    if (a < 0) {
        a = 0;
    } else if (a > 1) {
        a = 1;
    }
}

// -----------------------------------------------------------------------------------------
CPepeEngineColor CPepeEngineColor::saturateCopy(void) const
{
    CPepeEngineColor ret = *this;
    ret.saturate();
    return ret;
}

_PEPE_ENGINE_END