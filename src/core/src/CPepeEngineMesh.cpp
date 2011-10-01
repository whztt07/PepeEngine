#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineMesh.h"
#include "CPepeEngineMeshManager.h"
#include "CPepeEngineSubMesh.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineMesh::CPepeEngineMesh(const tstring& strName) : IPepeEngineResource(strName)
{
    m_nVertices             = 0;
    m_nFaces                = 0;
    m_nNormals              = 0;
    m_nTexCoords            = 0;
    m_nMaterials            = 0;
    m_bNeedAABBCalculated   = true;
    m_pVertexData           = NULL;
}

// -----------------------------------------------------------------------------------------
CPepeEngineMesh::~CPepeEngineMesh()
{
    if (isLoaded()) {
        unload();
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMesh::load()
{
    if (!isLoaded()) {
        LOG("Loading mesh: " + getName());
        loadFromFile(getName());
        LOG("Mesh successfully loaded");
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMesh::loadFromFile(const tstring& strName)
{
    tstring strFilePath = CPepeEngineMeshManager::getSingleton().getFilePath() + strName;
    CPepeEngineMeshManager::getSingleton().getMeshLoader(strName)->load(strFilePath, MeshPtr(this));
    m_bIsLoaded = true;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMesh::unload()
{
    LOG("Unloading mesh: " + getName());
    std::vector<CPepeEngineSubMesh*>::iterator it = m_subMeshList.begin();

    while (it != m_subMeshList.end()) {
        delete *it;
        *it = NULL;
        it++;
    }

    m_subMeshList.clear();

    delete m_pVertexData;

    m_bIsLoaded             = false;
    m_bNeedAABBCalculated   = true;
    LOG("Mesh successfully unloaded");
}

// -----------------------------------------------------------------------------------------
CPepeEngineSubMesh* CPepeEngineMesh::createSubMesh()
{
    CPepeEngineSubMesh* pSubMesh = new CPepeEngineSubMesh(MeshPtr(this));
    m_subMeshList.push_back(pSubMesh);

    return pSubMesh;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMesh::calculateBoundingBox() const
{
    if (m_bNeedAABBCalculated && (m_nVertices > 0)) {
        float* vertexData = static_cast<float*>(m_pVertexData->m_pVertexBuffer->lock(
                                                    IPepeEngineBuffer::BL_DISCARD));

        size_t              vSize   = m_pVertexData->m_pVertexBuffer->getVertexSize() / sizeof (float);
        size_t              nOffset = m_pVertexData->m_pVertexDeclaration->getElementBySemantic(VES_POSITION)->getOffset();
        CPepeEngineVector3  tmp;

        for (unsigned int i = 0; i < m_nVertices; i++) {
            tmp[0] = vertexData[i * vSize + nOffset];
            tmp[1] = vertexData[i * vSize + nOffset + 1];
            tmp[2] = vertexData[i * vSize + nOffset + 2];
            m_AABB.merge(tmp);
        }

        m_bNeedAABBCalculated = false;
        m_pVertexData->m_pVertexBuffer->unlock();
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMesh::setVertexData(CPepeEngineVertexData* pVertexData)
{
    delete m_pVertexData;
    m_pVertexData = pVertexData;

    std::vector<CPepeEngineSubMesh*>::iterator it = m_subMeshList.begin();

    while (it != m_subMeshList.end()) {
        (*it)->notifyVertexDataChange();
        it++;
    }
}

// -----------------------------------------------------------------------------------------
CPepeEngineVertexData* CPepeEngineMesh::getVertexData() const
{
    return m_pVertexData;
}

// -----------------------------------------------------------------------------------------
MeshPtr::MeshPtr() : CPepeEngineSharedPtr<CPepeEngineMesh>()
{
}

// -----------------------------------------------------------------------------------------
MeshPtr::MeshPtr(CPepeEngineMesh* r)
    : CPepeEngineSharedPtr<CPepeEngineMesh>(r)
{
}

// -----------------------------------------------------------------------------------------
MeshPtr::MeshPtr(const MeshPtr& r)
    : CPepeEngineSharedPtr<CPepeEngineMesh>(r)
{
}

// -----------------------------------------------------------------------------------------
MeshPtr::MeshPtr(const ResourcePtr& r) : CPepeEngineSharedPtr<CPepeEngineMesh>()
{
    ptr = (CPepeEngineMesh*)r.data();

    if (ptr) {
        ptr->ref();
    }
}

// -----------------------------------------------------------------------------------------
MeshPtr& MeshPtr::operator=(const ResourcePtr& r)
{
    if (ptr == (CPepeEngineMesh*)r.data()) {
        return *this;
    }

    if (ptr) {
        ptr->deref();
    }

    ptr = (CPepeEngineMesh*)r.data();

    if (ptr) {
        ptr->ref();
    }

    return *this;
}

_PEPE_ENGINE_END