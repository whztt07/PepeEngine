#include "CDemoMorphApplication.h"

// -----------------------------------------------------------------------------------------
CDemoMorphApplication::CDemoMorphApplication() : CDefApplication()
{	
}

// -----------------------------------------------------------------------------------------
CDemoMorphApplication::~CDemoMorphApplication()
{	
}

// -----------------------------------------------------------------------------------------
void CDemoMorphApplication::createScene()
{	
	CPepeEngineEntity*		pCube		= new CPepeEngineEntity(_T("Cube"), _T("TesselatedCube.3ds"));										
	CPepeEngineSceneNode*	pCubeNode	= m_pSceneManager->createSceneNode(_T("Cube"));				

	pCubeNode->attachObject(pCube);
	pCubeNode->setScale(10.0f, 10.0f, 10.0f);
	pCubeNode->setPosition(0.0f, -5.0f, -50.0f);
	pCube->setCullingMode(CULL_NONE);
	pCube->setVertexShader(_T("morph.vs"));
	pCube->setPixelShader(_T("morph.ps"));

	GPUProgramPtr pMorphVS = IPepeEngineGPUProgramManager::getSingleton().getByName(_T("morph.vs"));
	pMorphVS->getParameters()->setNamedConstant(_T("speed"), 0.2f);
	pMorphVS->getParameters()->setNamedConstant(_T("lerpMin"), -2.0f);
	pMorphVS->getParameters()->setNamedConstant(_T("lerpMax"), 2.0f);

	CPepeEngineSceneNode* pRootNode	= m_pSceneManager->getRootSceneNode();
	pRootNode->addChildSceneNode(pCubeNode);	
}

// -----------------------------------------------------------------------------------------
bool CDemoMorphApplication::setup()
{
	m_pCore = new CPepeEngineCore();
	if (m_pCore->showConfigDialog())
	{
		m_pCore->create(_T("Pepe Engine Morph Demo"));		

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
		CDefApplication::createCamera();
		CDefApplication::createFrameListener();

		return true;
	} else
	{
		return false;
	}
}