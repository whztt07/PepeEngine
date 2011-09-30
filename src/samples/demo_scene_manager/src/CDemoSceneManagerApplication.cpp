#include "CDemoSceneManagerApplication.h"
#include "CDemoSceneManagerFrameListener.h"

// -----------------------------------------------------------------------------------------
CDemoSceneManagerApplication::CDemoSceneManagerApplication() : CDefApplication()
{	
}

// -----------------------------------------------------------------------------------------
CDemoSceneManagerApplication::~CDemoSceneManagerApplication()
{	
}

// -----------------------------------------------------------------------------------------
void CDemoSceneManagerApplication::createCamera()
{
	CPepeEngineCamera* m_pCamera = new CPepeEngineCamera();
	m_pCamera->setPosition(0.0f, 0.0f, 30.0f);		
	m_pCamera->setNearClipDistance(0.1f);
	m_pCamera->setFarClipDistance(1500.0f);
	m_pCamera->setDirection(0.0f, 0.0f, -1.0f);

	m_pSceneManager->attachCamera(m_pCamera);
}

// -----------------------------------------------------------------------------------------
void CDemoSceneManagerApplication::createScene()
{	
	m_pSceneManager->createSkyBox(_T("cloudy_noon.jpg"), 500);
	
	std::vector<CPepeEngineSceneNode*> nodes;
	CPepeEngineSceneNode*	pRootNode		= m_pSceneManager->getRootSceneNode();		
	CPepeEngineEntity*		pSphere			= NULL;				
	MeshPtr					pSphereMesh		= CPepeEngineMeshManager::getSingleton().create(_T("face.3ds"));						
	pSphereMesh->load();
	
	for (unsigned int i = 0; i < 9; i++)
	{
		const tstring strName	= _T("Sphere") + CPepeEngineConverter::parseString((float)i);		
		pSphere					= new CPepeEngineEntity(strName, pSphereMesh);								
		pSphere->setVertexShader(_T("env_map.vs"));
		pSphere->setPixelShader(_T("env_map.ps"));
		pSphere->setLightingEnabled(false);
		pSphere->setSpecularEnabled(false);
		pSphere->setCullingMode(CULL_NONE);				
		nodes.push_back(m_pSceneManager->createSceneNode(strName));	
		nodes[i]->attachObject(pSphere);		
	}
	
	pRootNode->addChildSceneNode(nodes[0]);
	nodes[0]->setPosition(0.0f, 0.0f, 0.0f);	
	nodes[0]->addChildSceneNode(nodes[3]);
	nodes[0]->addChildSceneNode(nodes[4]);	
	nodes[3]->addChildSceneNode(nodes[1]);
	nodes[4]->addChildSceneNode(nodes[2]);	
	nodes[1]->setPosition(6.0f, 6.0f, 6.0f);
	nodes[2]->setPosition(-6.0f, -6.0f, -6.0f);
	nodes[3]->setPosition(-4.0f, 4.0f, 4.0f);
	nodes[3]->setScale(0.5f, 0.5f, 0.3f);
	nodes[4]->setPosition(4.0f, -4.0f, -4.0f);
	
	nodes[1]->addChildSceneNode(nodes[5]);
	nodes[1]->addChildSceneNode(nodes[6]);	
	nodes[5]->setPosition(-4.0f, 4.0f, 4.0f);
	nodes[5]->setScale(0.7f, 0.5f, 0.5f);
	nodes[6]->setPosition(4.0f, -4.0f, -4.0f);
	
	nodes[2]->addChildSceneNode(nodes[7]);
	nodes[2]->addChildSceneNode(nodes[8]);	
	nodes[7]->setPosition(-4.0f, 4.0f, 4.0f);
	nodes[7]->setScale(0.5f, 0.5f, 0.5f);
	nodes[8]->setPosition(4.0f, -4.0f, -4.0f);
	nodes[8]->setScale(0.5f, 0.2f, 0.6f);		
}

// -----------------------------------------------------------------------------------------
bool CDemoSceneManagerApplication::setup()
{
	m_pCore = new CPepeEngineCore();
	if (m_pCore->showConfigDialog())
	{
		m_pCore->create(_T("Pepe Engine Scene Manager Demo"));		

		#if defined(_DEBUG) || defined (DEBUG)
			IPepeEngineTextureManager::getSingleton().setFilePath(_T("../../../../media/textures/"));
			IPepeEngineGPUProgramManager::getSingleton().setFilePath(_T("../../../../media/shaders/"));
			CPepeEngineMeshManager::getSingleton().setFilePath(_T("../../../../media/meshes/"));
		#else
			IPepeEngineTextureManager::getSingleton().setFilePath(_T("media/textures/"));
			IPepeEngineGPUProgramManager::getSingleton().setFilePath(_T("media/shaders/"));
			CPepeEngineMeshManager::getSingleton().setFilePath(_T("media/meshes/"));
		#endif

		m_pSceneManager	= m_pCore->getSceneManager();					

		m_pCore->getRenderer()->setNormalizeNormalsEnabled(false);		
		m_pCore->getRenderer()->setLightingEnabled(false);
		m_pCore->getRenderer()->setSpecularEnabled(false);				

		createScene();
		m_pFrameListener = new CDemoSceneManagerFrameListener();
		createCamera();
		CDefApplication::createFrameListener();

		return true;
	} else
	{
		return false;
	}
}