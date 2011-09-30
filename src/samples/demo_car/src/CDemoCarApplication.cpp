#include "CDemoCarApplication.h"

// -----------------------------------------------------------------------------------------
CDemoCarApplication::CDemoCarApplication() : CDefApplication()
{	
}

// -----------------------------------------------------------------------------------------
CDemoCarApplication::~CDemoCarApplication()
{	
}

// -----------------------------------------------------------------------------------------
void CDemoCarApplication::createCamera()
{
	CPepeEngineCamera* m_pCamera = new CPepeEngineCamera();
	m_pCamera->setPosition(8.0f, 9.0f, 20.0f);		
	m_pCamera->setNearClipDistance(0.1f);
	m_pCamera->setFarClipDistance(1500.0f);
	m_pCamera->lookAt(*m_pSceneManager->getSceneNode(_T("Root")));
	
	m_pSceneManager->attachCamera(m_pCamera);
}

// -----------------------------------------------------------------------------------------
void CDemoCarApplication::createScene()
{
	CPepeEngineEntity*		pCar		= new CPepeEngineEntity(_T("Car"), _T("car.3ds"));									
	CPepeEngineSceneNode*	pRootNode	= m_pSceneManager->getRootSceneNode();

	pRootNode->attachObject(pCar);
	pRootNode->setPosition(0.0f, 0.0f, 0.0f);	
	pRootNode->setScale(0.2f, 0.2f, 0.2f);
	pRootNode->pitch(Degree(-80.0f));

	pCar->setVertexShader(_T("car.vs"));
	pCar->setPixelShader(_T("car.ps"));

	GPUProgramPtr	pCarPS			= IPepeEngineGPUProgramManager::getSingleton().getByName(_T("car.ps"));		
	TexturePtr		pEnvTexture		= IPepeEngineTextureManager::getSingleton().create(_T("Showroom.dds"));		
	TexturePtr		pNormalTexture	= IPepeEngineTextureManager::getSingleton().create(_T("Car  Normal Map.tga"));		
	TexturePtr		pSparkleTexture	= IPepeEngineTextureManager::getSingleton().create(_T("SparkleNoiseMap.tga"));		

	pEnvTexture->load();
	pNormalTexture->load();
	pSparkleTexture->load();	
	
	pCarPS->getParameters()->bindTexture(_T("sparkleMap"), pSparkleTexture);	
	pCarPS->getParameters()->bindTexture(_T("envMap"), pEnvTexture);	
	pCarPS->getParameters()->bindTexture(_T("dot3Map"), pNormalTexture);			
	pCarPS->getParameters()->setNamedConstant(_T("brightnessFactor"), 8.0f);
	pCarPS->getParameters()->setNamedConstant(_T("normalPerturbation"), 1.0f);
	pCarPS->getParameters()->setNamedConstant(_T("microflakePerturbationA"), 0.1f);
	pCarPS->getParameters()->setNamedConstant(_T("microflakePerturbation"), 1.0f);
	pCarPS->getParameters()->setNamedConstant(_T("glossLevel"), 0.0f);
	pCarPS->getParameters()->setNamedConstant(_T("paintColor0"), CPepeEngineVector4(0.4f, 0.0f, 0.4f, 0.0f));
	pCarPS->getParameters()->setNamedConstant(_T("paintColor2"), CPepeEngineVector4(0.0f, 0.35f, -0.35f, 0.0f));
	pCarPS->getParameters()->setNamedConstant(_T("paintColorMid"), CPepeEngineVector4(0.6f, 0.0f, 0.0f, 0.0f));
	pCarPS->getParameters()->setNamedConstant(_T("flakeLayerColor"), CPepeEngineVector4(0.5f, 0.5f, 0.0f, 0.0f));
}

// -----------------------------------------------------------------------------------------
bool CDemoCarApplication::setup()
{
	m_pCore = new CPepeEngineCore();
	if (m_pCore->showConfigDialog())
	{
		m_pCore->create(_T("Pepe Engine Car Demo"));		

		#if _DEBUG
		CPepeEngineMeshManager::getSingleton().setFilePath(_T("../../../../media/meshes/"));
		IPepeEngineTextureManager::getSingleton().setFilePath(_T("../../../../media/textures/"));
		IPepeEngineGPUProgramManager::getSingleton().setFilePath(_T("../../../../media/shaders/"));
		#else
		CPepeEngineMeshManager::getSingleton().setFilePath(_T("media/meshes/"));
		IPepeEngineTextureManager::getSingleton().setFilePath(_T("media/textures/"));
		IPepeEngineGPUProgramManager::getSingleton().setFilePath(_T("media/shaders/"));
		#endif

		m_pSceneManager	= m_pCore->getSceneManager();			

		m_pCore->getRenderer()->setNormalizeNormalsEnabled(false);				

		createScene();
		createCamera();
		CDefApplication::createFrameListener();

		return true;
	} else
	{
		return false;
	}
}