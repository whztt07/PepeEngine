#include "CDemoCastleApplication.h"
#include "CDemoCastleFrameListener.h"

// -----------------------------------------------------------------------------------------
CDemoCastleApplication::CDemoCastleApplication() : CDefApplication()
{
}

// -----------------------------------------------------------------------------------------
CDemoCastleApplication::~CDemoCastleApplication()
{
}

// -----------------------------------------------------------------------------------------
void CDemoCastleApplication::createCamera()
{
    CPepeEngineCamera* m_pCamera = new CPepeEngineCamera();
    m_pCamera->setPosition(40.0f, 35.0f, -150.0f);
    m_pCamera->setNearClipDistance(0.1f);
    m_pCamera->setFarClipDistance(1500.0f);
    m_pCamera->setDirection(-0.7f, 0.0f, 1.0f);

    m_pSceneManager->attachCamera(m_pCamera);
}

// -----------------------------------------------------------------------------------------
void CDemoCastleApplication::createScene()
{
    m_pSceneManager->createSkyBox(_T("early_morning.jpg"), 500);

    CPepeEngineEntity*      pCastle     = new CPepeEngineEntity(_T("Castle"), _T("Neuschwanstein.x"));
    CPepeEngineSceneNode*   pRootNode   = m_pSceneManager->getRootSceneNode();
    CPepeEngineSceneNode*   pCastleNode = m_pSceneManager->createSceneNode(_T("Castle"));
    pCastleNode->attachObject(pCastle);
    pRootNode->addChildSceneNode(pCastleNode);

    pCastleNode->setScale(0.25f, 0.25f, 0.25f);
    pCastleNode->setPosition(0.0f, 0.0f, -60.0f);

    pCastle->setCullingMode(CULL_CLOCKWISE);
    pCastle->setSpecularEnabled(false);
    pCastle->setLightingEnabled(true);
    pCastle->setShadeMode(SM_GOURAUD);

    CPepeEngineSceneNode*   pLightRootNode  = m_pSceneManager->createSceneNode(_T("LightRoot"));
    pLightRootNode->setPosition(-100.0f, 200.0f, 100.0f);

    CPepeEngineLight* pLight1 = m_pSceneManager->createLight(_T("light1"));
    pLight1->setLightType(LT_POINT);
    pLight1->setPosition(0.0f, 200.0f, -200.0f);
    pLight1->setDiffuse(0.3f, 0.2f, 0.2f);
    pLight1->setRange(1000.0f);

    pLightRootNode->addLight(pLight1);
    pCastleNode->addChildSceneNode(pLightRootNode);
}

// -----------------------------------------------------------------------------------------
bool CDemoCastleApplication::setup()
{
    m_pCore = new CPepeEngineCore();

    if (m_pCore->showConfigDialog()) {
        m_pCore->create(_T("Pepe Engine Castle Demo"));

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
        m_pCore->getRenderer()->setSpecularEnabled(false);
        m_pCore->getRenderer()->setLightingEnabled(true);
        m_pCore->getRenderer()->setShadeMode(SM_GOURAUD);

        createScene();
        createCamera();
        m_pFrameListener = new CDemoCastleFrameListener();
        CDefApplication::createFrameListener();

        return true;
    } else {
        return false;
    }
}