#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineImageManager.h"
#include "CPepeEngineLogManager.h"
#include "FreeImage/FreeImage.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message)
{
    // Callback method as required by FreeImage to report problems
    const char* typeName = FreeImage_GetFormatFromFIF(fif);

    if (typeName) {
        CPepeEngineLogManager::getSingleton().logMessage(
            _T("FreeImage error: '") + _TSTR(message) + _T("' when loading format ")
            + _TSTR(typeName)
        );
    } else {
        CPepeEngineLogManager::getSingleton().logMessage(
            _T("FreeImage error: '") + _TSTR(message)
        );
    }

}

// -----------------------------------------------------------------------------------------
CPepeEngineImageManager::CPepeEngineImageManager()
{
    FreeImage_Initialise(false);
    FreeImage_SetOutputMessage(FreeImageErrorHandler);
}

// -----------------------------------------------------------------------------------------
CPepeEngineImageManager::~CPepeEngineImageManager()
{
    FreeImage_DeInitialise();
}

// -----------------------------------------------------------------------------------------
ResourcePtr CPepeEngineImageManager::create(const tstring& strName)
{
    ResourcePtr pImage = getByName(strName);

    if (pImage.isNull()) {
        pImage = new CPepeEngineImage(strName);

        add(strName, pImage);
    }

    return pImage;
}

// -----------------------------------------------------------------------------------------
template<> CPepeEngineImageManager* CPepeEngineSingleton<CPepeEngineImageManager>::ms_singleton = 0;

CPepeEngineImageManager& CPepeEngineImageManager::getSingleton(void)
{
    assert(ms_singleton);
    return *ms_singleton;
}

CPepeEngineImageManager* CPepeEngineImageManager::getSingletonPtr(void)
{
    return ms_singleton;
}

_PEPE_ENGINE_END