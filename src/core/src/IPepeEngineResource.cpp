#include "PepeEnginePrecompiledHeaders.h"
#include "IPepeEngineResource.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
IPepeEngineResource::IPepeEngineResource(tstring strName) : CPepeEngineShared()
{
    m_bIsLoaded = false;
    m_strName   = strName;
}

// -----------------------------------------------------------------------------------------
IPepeEngineResource::~IPepeEngineResource()
{
}

// -----------------------------------------------------------------------------------------
bool IPepeEngineResource::isLoaded() const
{
    return m_bIsLoaded;
}

// -----------------------------------------------------------------------------------------
void IPepeEngineResource::setLoaded(bool bLoaded)
{
    m_bIsLoaded = bLoaded;
}

// -----------------------------------------------------------------------------------------
const tstring& IPepeEngineResource::getName() const
{
    return m_strName;
}

_PEPE_ENGINE_END