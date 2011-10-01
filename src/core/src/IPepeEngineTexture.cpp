#include "PepeEnginePrecompiledHeaders.h"
#include "IPepeEngineTexture.h"

_PEPE_ENGINE_START

tstring IPepeEngineTexture::CubeMapNames[6] = {
    _T("_UP"),
    _T("_BK"),
    _T("_DN"),
    _T("_FR"),
    _T("_LF"),
    _T("_RT")
};

// -----------------------------------------------------------------------------------------
void IPepeEngineTexture::setMipMapsNum(size_t nMipMaps)
{
    m_nMipMaps = m_nMipMapsRequested = nMipMaps;
}

// -----------------------------------------------------------------------------------------
size_t IPepeEngineTexture::getMipMapsNum() const
{
    return m_nMipMaps;
}

// -----------------------------------------------------------------------------------------
bool IPepeEngineTexture::isMipMapsHardwareGenerated() const
{
    return m_bMipMapsHardwareGenerated;
}

// -----------------------------------------------------------------------------------------
TexturePtr::TexturePtr() : CPepeEngineSharedPtr<IPepeEngineTexture>()
{
}

// -----------------------------------------------------------------------------------------
TexturePtr::TexturePtr(IPepeEngineTexture* r) : CPepeEngineSharedPtr<IPepeEngineTexture>(r)
{
}

// -----------------------------------------------------------------------------------------
TexturePtr::TexturePtr(const TexturePtr& r) : CPepeEngineSharedPtr<IPepeEngineTexture>(r)
{
}

// -----------------------------------------------------------------------------------------
TexturePtr::TexturePtr(const ResourcePtr& r) : CPepeEngineSharedPtr<IPepeEngineTexture>()
{
    ptr = (IPepeEngineTexture*)r.data();

    if (ptr) {
        ptr->ref();
    }
}

// -----------------------------------------------------------------------------------------
TexturePtr& TexturePtr::operator=(const ResourcePtr& r)
{
    if (ptr == (IPepeEngineTexture*)r.data()) {
        return *this;
    }

    if (ptr) {
        ptr->deref();
    }

    ptr = (IPepeEngineTexture*)r.data();

    if (ptr) {
        ptr->ref();
    }

    return *this;
}

_PEPE_ENGINE_END