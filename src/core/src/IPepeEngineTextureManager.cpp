#include "PepeEnginePrecompiledHeaders.h"
#include "IPepeEngineTextureManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
IPepeEngineTextureManager* CPepeEngineSingleton<IPepeEngineTextureManager>::ms_singleton = 0;

IPepeEngineTextureManager& IPepeEngineTextureManager::getSingleton(void)
{
    assert(ms_singleton);
    return *ms_singleton;
}

IPepeEngineTextureManager* IPepeEngineTextureManager::getSingletonPtr(void)
{
    return ms_singleton;
}

_PEPE_ENGINE_END