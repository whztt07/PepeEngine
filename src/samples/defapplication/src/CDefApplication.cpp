#include "CDefApplication.h"

// -----------------------------------------------------------------------------------------
CDefApplication::CDefApplication()
{
	m_pFrameListener	= NULL;
	m_pCore				= NULL;
	m_pCamera			= NULL;	
	m_pSceneManager		= NULL;		
}

// -----------------------------------------------------------------------------------------
CDefApplication::~CDefApplication()
{
	delete m_pFrameListener;
	delete m_pCore;		
	m_pCamera			= NULL;	
	m_pSceneManager		= NULL;		
}

// -----------------------------------------------------------------------------------------
void CDefApplication::createCamera()
{
	CPepeEngineCamera* m_pCamera = new CPepeEngineCamera();
	m_pCamera->setPosition(0.0f, 3.0f, 5.0f);		
	m_pCamera->setNearClipDistance(0.1f);
	m_pCamera->setFarClipDistance(1500.0f);
	m_pCamera->lookAt(0.0f, 2.0f, 0.0f);

	m_pSceneManager->attachCamera(m_pCamera);
}

// -----------------------------------------------------------------------------------------
void CDefApplication::createFrameListener()
{
	if (m_pFrameListener == NULL)
	{
		m_pFrameListener = new CDefApplicationFrameListener();
	}
	m_pCore->addFrameListener(m_pFrameListener);
}

// -----------------------------------------------------------------------------------------
void CDefApplication::createScene()
{	
}

// -----------------------------------------------------------------------------------------
void CDefApplication::run()
{
	if (!setup()) return;

	m_pCore->run();
}

// -----------------------------------------------------------------------------------------
bool CDefApplication::setup()
{
	return true;
}