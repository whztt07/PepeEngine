#include "PepeEnginePrecompiledHeaders.h"
#include "IPepeEngineResourceManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
IPepeEngineResourceManager::IPepeEngineResourceManager()
{
    m_resourceMap.clear();
}

// -----------------------------------------------------------------------------------------
IPepeEngineResourceManager::~IPepeEngineResourceManager()
{
    m_resourceMap.clear();
}

// -----------------------------------------------------------------------------------------
void IPepeEngineResourceManager::add(const tstring& strName, ResourcePtr pResource)
{
    m_resourceMap.insert(make_pair(strName, pResource));
}

// -----------------------------------------------------------------------------------------
void IPepeEngineResourceManager::remove(const tstring& strName)
{
    ResourceMap::iterator it = m_resourceMap.find(strName);

    if (it != m_resourceMap.end()) {
        it->second->unload();
        delete it->second;
        it->second = NULL;
        m_resourceMap.erase(it);
    }
}

// -----------------------------------------------------------------------------------------
void IPepeEngineResourceManager::load(const tstring& strName)
{
    ResourcePtr resource = getByName(strName);

    if (resource != NULL) {
        if (!resource->isLoaded()) {
            resource->load();
        }
    } else {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_ITEM_NOT_FOUND_ERROR,
            _T("Couldn't find resource: ") + strName,
            _T("IPepeEngineResourceManager::load")
        );
    }
}

// -----------------------------------------------------------------------------------------
void IPepeEngineResourceManager::unload(const tstring& strName)
{
    ResourcePtr resource = getByName(strName);

    if (resource != NULL) {
        if (resource->isLoaded()) {
            resource->unload();
        }
    } else {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_ITEM_NOT_FOUND_ERROR,
            _T("Couldn't find resource: ") + strName,
            _T("IPepeEngineResourceManager::unload")
        );
    }
}

// -----------------------------------------------------------------------------------------
ResourcePtr IPepeEngineResourceManager::getByName(const tstring& strName) const
{
    ResourceMap::const_iterator it = m_resourceMap.find(strName);

    if (it == m_resourceMap.end()) {
        return NULL;
    } else {
        return it->second;
    }
}

_PEPE_ENGINE_END