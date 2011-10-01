#include "PepeEnginePrecompiledHeaders.h"
#include "IPepeEngineBufferManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
IPepeEngineBufferManager::IPepeEngineBufferManager()
{
    m_bufferList.clear();
}

// -----------------------------------------------------------------------------------------
IPepeEngineBufferManager::~IPepeEngineBufferManager()
{
    BufferList::iterator it = m_bufferList.begin();

    while (it != m_bufferList.end()) {
        delete *it;
        *it = NULL;
        it++;
    }

    m_bufferList.clear();

    VertexDeclarationList::iterator vit = m_vertexDeclarationList.begin();

    while (vit != m_vertexDeclarationList.end()) {
        delete *vit;
        *vit = NULL;
        vit++;
    }

    m_vertexDeclarationList.clear();
}

// -----------------------------------------------------------------------------------------
template<> IPepeEngineBufferManager* CPepeEngineSingleton<IPepeEngineBufferManager>::ms_singleton = 0;

IPepeEngineBufferManager& IPepeEngineBufferManager::getSingleton(void)
{
    assert(ms_singleton);
    return *ms_singleton;
}

IPepeEngineBufferManager* IPepeEngineBufferManager::getSingletonPtr(void)
{
    return ms_singleton;
}

_PEPE_ENGINE_END