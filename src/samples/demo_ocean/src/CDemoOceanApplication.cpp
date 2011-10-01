#include "CDemoOceanApplication.h"

// -----------------------------------------------------------------------------------------
CDemoOceanApplication::CDemoOceanApplication() : CDefApplication()
{
}

// -----------------------------------------------------------------------------------------
CDemoOceanApplication::~CDemoOceanApplication()
{
}

// -----------------------------------------------------------------------------------------
void CDemoOceanApplication::createCamera()
{
    CPepeEngineCamera* m_pCamera = new CPepeEngineCamera();
    m_pCamera->setPosition(0.0f, 5.0f, 0.0f);
    m_pCamera->setNearClipDistance(0.1f);
    m_pCamera->setFarClipDistance(1500.0f);

    m_pCamera->pitch(Degree(-15.0f));

    m_pSceneManager->attachCamera(m_pCamera);
}

// -----------------------------------------------------------------------------------------
void CDemoOceanApplication::createScene()
{
    m_pSceneManager->createSkyBox(_T("morning.jpg"), 500);

    CPepeEngineEntity*      pOcean      = new CPepeEngineEntity(_T("OceanSurface"), _T("OceanSurface2.3ds"));
    CPepeEngineSceneNode*   pRootNode   = m_pSceneManager->getRootSceneNode();

    pOcean->setVertexShader(_T("ocean.vs"));
    pOcean->setPixelShader(_T("ocean.ps"));
    pOcean->setPosition(0.0f, 0.0f, 0.0f);

    GPUProgramPtr   pOceanPS            = IPepeEngineGPUProgramManager::getSingleton().getByName(_T("ocean.ps"));
    GPUProgramPtr   pOceanVS            = IPepeEngineGPUProgramManager::getSingleton().getByName(_T("ocean.vs"));
    TexturePtr      pOceanBumpTexture   = IPepeEngineTextureManager::getSingleton().create(_T("waves2.dds"));
    pOceanBumpTexture->load();
    pOceanPS->getParameters()->bindTexture(_T("bump"), pOceanBumpTexture);
    pOceanPS->getParameters()->setNamedConstant(_T("fBias"), 0.328f);
    pOceanPS->getParameters()->setNamedConstant(_T("fPower"), 5.0f);
    pOceanPS->getParameters()->setNamedConstant(_T("fHdrMultiplier"), 0.371f);
    pOceanPS->getParameters()->setNamedConstant(_T("fReflectionAmount"), 0.7f);
    pOceanPS->getParameters()->setNamedConstant(_T("fReflectionBlur"), 0.0f);
    pOceanPS->getParameters()->setNamedConstant(_T("fWaterAmount"), 0.4f);
    pOceanPS->getParameters()->setNamedConstant(_T("f4ShallowColor"), CPepeEngineVector4(0.0f, 1.0f, 1.0f, 1.0f));
    pOceanPS->getParameters()->setNamedConstant(_T("f4DeepColor"), CPepeEngineVector4(0.0f, 0.3f, 0.5f, 1.0f));
    pOceanPS->getParameters()->setNamedConstant(_T("f4ReflectionColor"), CPepeEngineVector4(0.95f, 1.0f, 1.0f, 1.0f));

    float texScale[2] = {25.0f, 26.0f};
    pOceanVS->getParameters()->setNamedConstant(_T("f2TextureScale"), texScale, 2, 1);

    float bumpSpeed[2] = {0.015f, 0.005f};
    pOceanVS->getParameters()->setNamedConstant(_T("f2BumpSpeed"), bumpSpeed, 2, 1);
    pOceanVS->getParameters()->setNamedConstant(_T("fBumpScale"), 0.2f);
    pOceanVS->getParameters()->setNamedConstant(_T("waveFreq"), 0.028f);
    pOceanVS->getParameters()->setNamedConstant(_T("waveAmp"), 1.8f);

    pRootNode->attachObject(pOcean);

    pOcean->setCullingMode(CULL_NONE);
    pOcean->setSpecularEnabled(false);
    pOcean->setLightingEnabled(false);
}

// -----------------------------------------------------------------------------------------
bool CDemoOceanApplication::setup()
{
    m_pCore = new CPepeEngineCore();

    if (m_pCore->showConfigDialog()) {
        m_pCore->create(_T("Pepe Engine Ocean Demo"));

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
        createCamera();
        CDefApplication::createFrameListener();

        return true;
    } else {
        return false;
    }
}