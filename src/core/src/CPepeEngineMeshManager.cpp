#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineMeshManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineMeshManager::CPepeEngineMeshManager()
{
    m_meshLoadersList.push_back(new CPepeEngineXFileMeshLoader());
    m_meshLoadersList.push_back(new CPepeEngine3DSFileMeshLoader());
}

// -----------------------------------------------------------------------------------------
CPepeEngineMeshManager::~CPepeEngineMeshManager()
{
    std::vector<IPepeEngineMeshLoader* >::iterator it = m_meshLoadersList.begin();

    while (it != m_meshLoadersList.end()) {
        delete *it;
        *it = NULL;
        it++;
    }

    m_meshLoadersList.clear();
}

// -----------------------------------------------------------------------------------------
ResourcePtr CPepeEngineMeshManager::create(const tstring& strName)
{
    ResourcePtr pMesh = getByName(strName);

    if (pMesh.isNull()) {
        pMesh = new CPepeEngineMesh(strName);

        add(strName, pMesh);
    }

    return pMesh;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMeshManager::addMeshLoader(IPepeEngineMeshLoader* pMeshLoader)
{
    m_meshLoadersList.push_back(pMeshLoader);
}

// -----------------------------------------------------------------------------------------
IPepeEngineMeshLoader* CPepeEngineMeshManager::getMeshLoader(const tstring& strName) const
{
    std::vector<IPepeEngineMeshLoader* >::iterator it = m_meshLoadersList.begin();

    while (it != m_meshLoadersList.end()) {
        if ((*it)->isAvailableFileExtension(strName)) {
            return *it;
        }

        it++;
    }

    PEPE_ENGINE_EXCEPT(
        CPepeEngineException::ERR_INVALID_PARAMETERS,
        _T("File format not supported."),
        _T("CPepeEngineMeshManager::getMeshLoader")
    );
}

// -----------------------------------------------------------------------------------------
template<> CPepeEngineMeshManager* CPepeEngineSingleton<CPepeEngineMeshManager>::ms_singleton = 0;

CPepeEngineMeshManager& CPepeEngineMeshManager::getSingleton(void)
{
    assert(ms_singleton);
    return *ms_singleton;
}

CPepeEngineMeshManager* CPepeEngineMeshManager::getSingletonPtr(void)
{
    return ms_singleton;
}

_PEPE_ENGINE_END