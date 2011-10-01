#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineMaterialManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineMaterialManager::CPepeEngineMaterialManager() : m_nCounter(0)
{
    MaterialPtr pDefMaterial = create(_T("default material"));

    pDefMaterial->load();
    pDefMaterial->setDiffuse(CPepeEngineColor::WHITE);
    pDefMaterial->setSpecular(CPepeEngineColor::WHITE);
    pDefMaterial->setEmissive(CPepeEngineColor::WHITE);
    pDefMaterial->setAmbient(CPepeEngineColor::WHITE);
}

// -----------------------------------------------------------------------------------------
ResourcePtr CPepeEngineMaterialManager::create(const tstring& strName)
{
    ResourcePtr pMaterial = getByName(strName);

    if (pMaterial.isNull()) {
        pMaterial = new CPepeEngineMaterial(strName);

        add(strName, pMaterial);
    }

    return pMaterial;
}

// -----------------------------------------------------------------------------------------
tstring CPepeEngineMaterialManager::generateGUID()
{
    tstringstream str;
    str << _T("MATERIAL ") << m_nCounter++;
    return str.str();
}

// -----------------------------------------------------------------------------------------
template<> CPepeEngineMaterialManager* CPepeEngineSingleton<CPepeEngineMaterialManager>::ms_singleton = 0;

CPepeEngineMaterialManager& CPepeEngineMaterialManager::getSingleton(void)
{
    assert(ms_singleton);
    return *ms_singleton;
}

CPepeEngineMaterialManager* CPepeEngineMaterialManager::getSingletonPtr(void)
{
    return ms_singleton;
}

_PEPE_ENGINE_END