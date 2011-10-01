#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineEntity.h"
#include "CPepeEngineMaterialManager.h"
#include "CPepeEngineMeshManager.h"
#include "IPepeEngineGPUProgram.h"
#include "IPepeEngineGPUProgramManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineEntity::CPepeEngineEntity(const tstring& strName)
{
    m_transformMatrix   = CPepeEngineMatrix4::IDENTITY;
    m_strName           = strName;

    setPosition(CPepeEngineVector3::ZERO);
    setScale(CPepeEngineVector3::UNIT_SCALE);
    setRotation(CPepeEngineQuaternion::IDENTITY);
    makeTransforms();
}

// -----------------------------------------------------------------------------------------
CPepeEngineEntity::CPepeEngineEntity(const tstring& strName, const tstring& strMeshName)
{
    m_pMesh = CPepeEngineMeshManager::getSingleton().create(strMeshName);
    m_pMesh->load();

    // wrap submeshes into subentities - important for having different materials for this same meshes
    if (m_pMesh->isLoaded()) {
        std::vector<CPepeEngineSubMesh*>::iterator it = m_pMesh->m_subMeshList.begin();

        while (it != m_pMesh->m_subMeshList.end()) {
            m_subEntityList.push_back(new CPepeEngineSubEntity(this, *it));

            it++;
        }
    }

    m_transformMatrix   = CPepeEngineMatrix4::IDENTITY;
    m_AABB              = m_pMesh->getAABB();
    m_strName           = strName;

    setPosition(CPepeEngineVector3::ZERO);
    setScale(CPepeEngineVector3::UNIT_SCALE);
    setRotation(CPepeEngineQuaternion::IDENTITY);
    makeTransforms();
}

// -----------------------------------------------------------------------------------------
CPepeEngineEntity::CPepeEngineEntity(const tstring& strName, MeshPtr pMesh)
{
    m_pMesh = pMesh;

    // wrap submeshes into subentities - important for having different materials for this same meshes
    if (m_pMesh->isLoaded()) {
        std::vector<CPepeEngineSubMesh*>::iterator it = m_pMesh->m_subMeshList.begin();

        while (it != m_pMesh->m_subMeshList.end()) {
            m_subEntityList.push_back(new CPepeEngineSubEntity(this, *it));

            it++;
        }
    }

    m_transformMatrix   = CPepeEngineMatrix4::IDENTITY;
    m_AABB              = m_pMesh->getAABB();
    m_strName           = strName;

    setPosition(CPepeEngineVector3::ZERO);
    setScale(CPepeEngineVector3::UNIT_SCALE);
    setRotation(CPepeEngineQuaternion::IDENTITY);
    makeTransforms();
}

// -----------------------------------------------------------------------------------------
CPepeEngineEntity::~CPepeEngineEntity()
{
    SubEntityList::iterator it = m_subEntityList.begin();

    while (it != m_subEntityList.end()) {
        delete *it;
        *it = NULL;

        it++;
    }

    m_subEntityList.clear();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::makeTransforms() const
{
    m_transformMatrix.makeTransform(m_translation, m_scale, m_rotation);

    if (m_pMesh) {
        m_AABB = m_pMesh->getAABB();
        m_AABB.transformAffine(m_transformMatrix);

        if (m_AABB.isFinite()) {
            m_position = m_AABB.getCenter();
        }
    }
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVector3& CPepeEngineEntity::getWorldPosition() const
{
    return m_position;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineMatrix4& CPepeEngineEntity::getWorldTransforms() const
{
    return m_transformMatrix;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::render() const
{
    if (m_subEntityList.empty()) {
        return;
    }

    SubEntityList::const_iterator it = m_subEntityList.begin();

    while (it != m_subEntityList.end()) {
        (*it)->render();

        it++;
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setPosition(const CPepeEngineVector3& position) const
{
    setTranslation(position);
    m_position = position;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setPosition(float x, float y, float z) const
{
    setPosition(CPepeEngineVector3(x, y, z));
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setTranslation(const CPepeEngineVector3& translation) const
{
    m_translation = translation;
    makeTransforms();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setTranslation(float x, float y, float z) const
{
    setTranslation(CPepeEngineVector3(x, y, z));
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setScale(const CPepeEngineVector3& scale) const
{
    m_scale = scale;
    makeTransforms();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setScale(float x, float y, float z) const
{
    setScale(CPepeEngineVector3(x, y, z));
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setRotation(const CPepeEngineQuaternion& rotation) const
{
    m_rotation = rotation;
    makeTransforms();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setRotation(float angle, const CPepeEngineVector3& axis) const
{
    m_rotation.fromAngleAxis(Degree(angle), axis);
    makeTransforms();
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3& CPepeEngineEntity::getTranslation() const
{
    return m_translation;
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector3& CPepeEngineEntity::getScale() const
{
    return m_scale;
}

// -----------------------------------------------------------------------------------------
CPepeEngineQuaternion& CPepeEngineEntity::getRotation() const
{
    return m_rotation;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setPixelShader(const tstring &strGPUProgramName)
{
    if (CPepeEngineCore::getSingleton().getRenderer()->getRendererCapabilities()->hasCapability(RC_PIXEL_PROGRAM)) {
        if (m_pMesh->isLoaded()) {
            SubEntityList::iterator it = m_subEntityList.begin();

            while (it != m_subEntityList.end()) {
                MaterialPtr pCurrentMaterial = CPepeEngineMaterialManager::getSingleton().getByName(
                                                   (*it)->getMaterialName()
                                               );
                pCurrentMaterial->setPixelShader(strGPUProgramName);

                it++;
            }
        }
    } else {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_RENDERINGAPI_ERROR,
            _T("This device doesn't support pixel shaders."),
            _T("CPepeEngineEntity::setPixelShader")
        );
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setVertexShader(const tstring &strGPUProgramName)
{
    if (CPepeEngineCore::getSingleton().getRenderer()->getRendererCapabilities()->hasCapability(RC_VERTEX_PROGRAM)) {
        if (m_pMesh->isLoaded()) {
            SubEntityList::iterator it = m_subEntityList.begin();

            while (it != m_subEntityList.end()) {
                MaterialPtr pCurrentMaterial = CPepeEngineMaterialManager::getSingleton().getByName(
                                                   (*it)->getMaterialName()
                                               );

                pCurrentMaterial->setVertexShader(strGPUProgramName);

                it++;
            }
        }
    } else {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_RENDERINGAPI_ERROR,
            _T("This device doesn't support vertex shaders."),
            _T("CPepeEngineEntity::setVertexShader")
        );
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setLightingEnabled(bool bEnabled)
{
    if (m_pMesh->isLoaded()) {
        SubEntityList::iterator it = m_subEntityList.begin();

        while (it != m_subEntityList.end()) {
            MaterialPtr pCurrentMaterial = CPepeEngineMaterialManager::getSingleton().getByName(
                                               (*it)->getMaterialName()
                                           );

            pCurrentMaterial->setLightingEnabled(bEnabled);

            it++;
        }
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setCullingMode(const CullingMode& mode)
{
    if (m_pMesh->isLoaded()) {
        SubEntityList::iterator it = m_subEntityList.begin();

        while (it != m_subEntityList.end()) {
            MaterialPtr pCurrentMaterial = CPepeEngineMaterialManager::getSingleton().getByName(
                                               (*it)->getMaterialName()
                                           );

            pCurrentMaterial->setCullingMode(mode);

            it++;
        }
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setShadeMode(const ShadeMode& mode)
{
    if (m_pMesh->isLoaded()) {
        SubEntityList::iterator it = m_subEntityList.begin();

        while (it != m_subEntityList.end()) {
            MaterialPtr pCurrentMaterial = CPepeEngineMaterialManager::getSingleton().getByName(
                                               (*it)->getMaterialName()
                                           );

            pCurrentMaterial->setShadeMode(mode);

            it++;
        }
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setSpecularEnabled(bool bEnabled)
{
    if (m_pMesh->isLoaded()) {
        SubEntityList::iterator it = m_subEntityList.begin();

        while (it != m_subEntityList.end()) {
            MaterialPtr pCurrentMaterial = CPepeEngineMaterialManager::getSingleton().getByName(
                                               (*it)->getMaterialName()
                                           );

            pCurrentMaterial->setSpecularEnabled(bEnabled);

            it++;
        }
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setSceneBlending(const SceneBlendType& blendType)
{
    if (m_pMesh->isLoaded()) {
        SubEntityList::iterator it = m_subEntityList.begin();

        while (it != m_subEntityList.end()) {
            MaterialPtr pCurrentMaterial = CPepeEngineMaterialManager::getSingleton().getByName(
                                               (*it)->getMaterialName()
                                           );

            pCurrentMaterial->setSceneBlending(blendType);

            it++;
        }
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setSceneBlending(const SceneBlendFactor& srcFactor, const SceneBlendFactor& destFactor)
{
    if (m_pMesh->isLoaded()) {
        SubEntityList::iterator it = m_subEntityList.begin();

        while (it != m_subEntityList.end()) {
            MaterialPtr pCurrentMaterial = CPepeEngineMaterialManager::getSingleton().getByName(
                                               (*it)->getMaterialName()
                                           );

            pCurrentMaterial->setSceneBlending(srcFactor, destFactor);

            it++;
        }
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::setAlpha(float fAlpha)
{
    if (m_pMesh->isLoaded()) {
        SubEntityList::iterator it = m_subEntityList.begin();

        while (it != m_subEntityList.end()) {
            MaterialPtr pCurrentMaterial = CPepeEngineMaterialManager::getSingleton().getByName(
                                               (*it)->getMaterialName()
                                           );

            pCurrentMaterial->setAlpha(fAlpha);

            it++;
        }
    }
}

// -----------------------------------------------------------------------------------------
MeshPtr CPepeEngineEntity::getMesh() const
{
    return m_pMesh;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineEntity::addSubEntity(CPepeEngineSubMesh *pSubMesh)
{
    m_subEntityList.push_back(new CPepeEngineSubEntity(this, pSubMesh));
    notifyListeners(CPepeEngineEvent(EVENT_OBJECT_CHANGED));
}

// -----------------------------------------------------------------------------------------
const tstring& CPepeEngineEntity::getName() const
{
    return m_strName;
}

_PEPE_ENGINE_END