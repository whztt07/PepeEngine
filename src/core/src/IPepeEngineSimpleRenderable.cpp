#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineMaterialManager.h"
#include "IPepeEngineSimpleRenderable.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
IPepeEngineSimpleRenderable::IPepeEngineSimpleRenderable()
{
    m_position  = CPepeEngineVector3::ZERO;
    m_transform = CPepeEngineMatrix4::IDENTITY;
    m_pMaterial = (MaterialPtr )CPepeEngineMaterialManager::getSingleton().getByName(_T("default material"));
}

// -----------------------------------------------------------------------------------------
IPepeEngineSimpleRenderable::~IPepeEngineSimpleRenderable()
{
    m_pMaterial = NULL;
}

// -----------------------------------------------------------------------------------------
void IPepeEngineSimpleRenderable::render() const
{
    CPepeEngineCore::getSingleton().getRenderer()->unbindGPUPrograms();
    CPepeEngineCore::getSingleton().getRenderer()->setTexture(TexturePtr(NULL));
    CPepeEngineCore::getSingleton().getRenderer()->setMaterial(m_pMaterial);
    CPepeEngineCore::getSingleton().getRenderer()->render(m_renderOperation);
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVector3& IPepeEngineSimpleRenderable::getWorldPosition() const
{
    return m_position;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineMatrix4& IPepeEngineSimpleRenderable::getWorldTransforms() const
{
    return m_transform;
}

_PEPE_ENGINE_END