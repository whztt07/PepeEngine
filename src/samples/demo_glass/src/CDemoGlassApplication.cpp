#include "CDemoGlassApplication.h"

// -----------------------------------------------------------------------------------------
CDemoGlassApplication::CDemoGlassApplication() : CDefApplication()
{
}

// -----------------------------------------------------------------------------------------
CDemoGlassApplication::~CDemoGlassApplication()
{
}

// -----------------------------------------------------------------------------------------
void CDemoGlassApplication::createScene()
{
    m_pSceneManager->createSkyBox(_T("snow.jpg"), 500);

    CPepeEngineEntity*      pVase       = new CPepeEngineEntity(_T("Vase"), _T("Teapot.3ds"));
    CPepeEngineSceneNode*   pRootNode   = m_pSceneManager->getRootSceneNode();

    pRootNode->attachObject(pVase);
    pRootNode->setPosition(0.0f, 0.0f, -15.0f);
    pRootNode->setScale(0.1f, 0.1f, 0.1f);

    pVase->setVertexShader(_T("glass.vs"));
    pVase->setPixelShader(_T("glass.ps"));
    pVase->setCullingMode(CULL_CLOCKWISE);


    GPUProgramPtr   pVasePS         = IPepeEngineGPUProgramManager::getSingleton().getByName(_T("glass.ps"));
    TexturePtr      pRainbowTexture = IPepeEngineTextureManager::getSingleton().create(_T("Rainbow.tga"));
    pRainbowTexture->load();

    pVasePS->getParameters()->bindTexture(_T("Rainbow"), pRainbowTexture);
    pVasePS->getParameters()->setNamedConstant(_T("indexOfRefractionRatio"), 1.14f);
    pVasePS->getParameters()->setNamedConstant(_T("rainbowSpread"), 0.18f);
    pVasePS->getParameters()->setNamedConstant(_T("rainbowScale"), 0.2f);
    pVasePS->getParameters()->setNamedConstant(_T("reflectionScale"), 1.0f);
    pVasePS->getParameters()->setNamedConstant(_T("refractionScale"), 1.0f);
    pVasePS->getParameters()->setNamedConstant(_T("ambient"), 0.2f);
    pVasePS->getParameters()->setNamedConstant(_T("baseColor"), CPepeEngineVector4(0.78f, 0.78f, 0.78f, 1.0f));
}

// -----------------------------------------------------------------------------------------
bool CDemoGlassApplication::setup()
{
    m_pCore = new CPepeEngineCore();

    if (m_pCore->showConfigDialog()) {
        m_pCore->create(_T("Pepe Engine Glass Demo"));

#if _DEBUG
        CPepeEngineMeshManager::getSingleton().setFilePath(_T("../../../../media/meshes/"));
        IPepeEngineTextureManager::getSingleton().setFilePath(_T("../../../../media/textures/"));
        IPepeEngineGPUProgramManager::getSingleton().setFilePath(_T("../../../../media/shaders/"));
#else
        CPepeEngineMeshManager::getSingleton().setFilePath(_T("media/meshes/"));
        IPepeEngineTextureManager::getSingleton().setFilePath(_T("media/textures/"));
        IPepeEngineGPUProgramManager::getSingleton().setFilePath(_T("media/shaders/"));
#endif

        m_pSceneManager = m_pCore->getSceneManager();

        m_pCore->getRenderer()->setNormalizeNormalsEnabled(false);

        createScene();
        CDefApplication::createCamera();
        CDefApplication::createFrameListener();

        return true;
    } else {
        return false;
    }
}