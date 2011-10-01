#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineDynamicLibraryManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
ResourcePtr CPepeEngineDynamicLibraryManager::create(const tstring& strName)
{
    ResourcePtr pDynamicLibrary = getByName(strName);

    if (pDynamicLibrary.isNull()) {
        pDynamicLibrary = new CPepeEngineDynamicLibrary(strName);

        add(strName, pDynamicLibrary);
    }

    return pDynamicLibrary;
}

// -----------------------------------------------------------------------------------------
template<> CPepeEngineDynamicLibraryManager* CPepeEngineSingleton<CPepeEngineDynamicLibraryManager>::ms_singleton = 0;

CPepeEngineDynamicLibraryManager& CPepeEngineDynamicLibraryManager::getSingleton(void)
{
    assert(ms_singleton);
    return *ms_singleton;
}

CPepeEngineDynamicLibraryManager* CPepeEngineDynamicLibraryManager::getSingletonPtr(void)
{
    return ms_singleton;
}

_PEPE_ENGINE_END