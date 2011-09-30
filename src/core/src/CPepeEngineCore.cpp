#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineCore.h"
#include "CPepeEngineLogManager.h"
#include "CPepeEngineMaterialManager.h"
#include "CPepeEngineMeshManager.h"
#include "IPepeEngineGPUProgramManager.h"
#include "IPepeEnginePlugin.h"
#include "IPepeEngineRenderWindow.h"
#include "IPepeEngineVideoModeList.h"

_PEPE_ENGINE_START

typedef void (*DLL_START_PLUGIN)(void);
typedef void (*DLL_STOP_PLUGIN)(void);

// -----------------------------------------------------------------------------------------
float CPepeEngineCore::calculateEventTime(unsigned long now, FrameEventTimeType type)
{
	/*
		Calculate the average time passed between events of the given type
	*/

	std::deque<unsigned long>& times = m_eventTimes[type];
	times.push_back(now);

	if (times.size() == 1)
	{
		return 0;
	}

	// Times up to mFrameSmoothingTime seconds old should be kept
	unsigned long discardThreshold = static_cast<unsigned long>(m_fFrameSmoothingTime * 1000.0f);

	// Find the oldest time to keep
	std::deque<unsigned long>::iterator it = times.begin(),	end = times.end() - 2; // We need at least two times
	while (it != end)
	{
		if (now - *it > discardThreshold)
		{
			++it;
		}
		else
		{
			break;
		}
	}

	// Remove old times
	times.erase(times.begin(), it);

	return float(times.back() - times.front()) / ((times.size() - 1) * 1000);
}

// -----------------------------------------------------------------------------------------
CPepeEngineCore::CPepeEngineCore() : m_fFrameSmoothingTime(0.0f)
{			
	if (CPepeEngineLogManager::getSingletonPtr() == 0)
	{
		m_pLogManager = new CPepeEngineLogManager();	
		CPepeEngineLogManager::getSingleton().createLog(_T("pepe_engine_error.log"), false, true);
		CPepeEngineLogManager::getSingleton().createLog(_T("pepe_engine.log"), true, false);
	}

	LOG("Pepe Engine Started");

	if (CPepeEngineMeshManager::getSingletonPtr() == 0)
	{
		m_pMeshManager = new CPepeEngineMeshManager();
	}	
	
	if (CPepeEngineDynamicLibraryManager::getSingletonPtr() == 0)
	{
		m_pDynamicLibraryManager = new CPepeEngineDynamicLibraryManager();
	}

	if (CPepeEngineImageManager::getSingletonPtr() == 0)
	{
		m_pImageManager = new CPepeEngineImageManager();
	}

	m_pConfig			= new CPepeEngineConfig();
	m_pSceneManager		= NULL;	
	m_pMaterialManager	= NULL;
	m_pWindow			= NULL;	
	m_pTimer			= NULL;
	m_pConfigDialog		= NULL;
	m_pRenderer			= NULL;
	m_pRendererList		= new RendererList;

	m_pRendererList->clear();
	m_plugins.clear();
	m_pluginLibs.clear();

	// Clear event times
	for (int i = 0; i < 3; ++i)
	{
		m_eventTimes[i].clear();
	}
	m_fFrameSmoothingTime	= 0.0f;
	m_fElapsedTime			= 0.0f;

	#if PEPE_ENGINE_PLATFORM == PEPE_ENGINE_PLATFORM_WIN32		
		loadPlugin(_T("OS_Win32"));
	#else
	#	error operating system not supported
	#endif

	// TODO: should load this from config file
	loadPlugin(_T("Renderer_Direct3D9"));
}

// -----------------------------------------------------------------------------------------
CPepeEngineCore::~CPepeEngineCore()
{
	if (m_pWindow)
	{
		m_pWindow->destroy();
	}	
	
	delete m_pConfig;
	delete m_pSceneManager;
	delete m_pMaterialManager;
	delete m_pMeshManager;	
	delete m_pRendererList;
	delete m_pImageManager;

	unloadPlugins();
	delete m_pDynamicLibraryManager;

	// Clear event times
	for (int i = 0; i < 3; ++i)
	{
		m_eventTimes[i].clear();
	}

	m_frameListeners.clear();

	LOG("Pepe Engine Stopped");
	delete m_pLogManager;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCore::create(	
	const tstring& strWindowTitle
)
{
	if (m_pWindow)
	{		
		if (m_pRenderer)
		{
			m_pConfig->log();
			IPepeEngineVideoMode* pVideoMode = m_pRenderer->getVideoModeList()->item(
				m_pConfig->getOption(_T("Video Mode"))->strValue
			);
			m_pWindow->create(
				strWindowTitle,
				pVideoMode->getWidth(),
				pVideoMode->getHeight(),
				CPepeEngineConverter::parseBool(m_pConfig->getOption(_T("FullScreen"))->strValue)
			);

			m_pRenderer->attachWindow(m_pWindow);
			
			// ---------------- setting default parameters -----------------------
			m_pRenderer->setLightingEnabled(true);
			m_pRenderer->setSpecularEnabled(true);
			m_pRenderer->setZBufferEnabled(true);
			m_pRenderer->setNormalizeNormalsEnabled(true);
			m_pRenderer->setCullingMode(CULL_CLOCKWISE);
			m_pRenderer->setShadeMode(SM_GOURAUD);
			m_pRenderer->setPolygonMode(PM_SOLID);	

			IPepeEngineGPUProgramManager::getSingleton().setPSShaderModel(m_pRenderer->getRendererCapabilities()->getMaxPixelProgramVersion());
			IPepeEngineGPUProgramManager::getSingleton().setVSShaderModel(m_pRenderer->getRendererCapabilities()->getMaxVertexProgramVersion());
			//end ------------- setting default parameters -----------------------

			m_pSceneManager = new CPepeEngineSceneManager();	
			
			if (CPepeEngineMaterialManager::getSingletonPtr() == 0)
			{
				m_pMaterialManager = new CPepeEngineMaterialManager();
			}			
		} else
		{
			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_INVALID_PARAMETERS, 
				_T("Renderer not found."), 
				_T("CPepeEngineCore::create")
			);		
		}
	} else
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_INVALID_PARAMETERS, 
			_T("Invalid initialization"), 
			_T("CPepeEngineCore::create")
		);		
	}
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineCore::fireFrameStarted()
{
	unsigned long now = m_pTimer->getMilliseconds();
	FrameEvent frameEvent;        
	frameEvent.fTimeSinceLastFrame = calculateEventTime(now, FETT_STARTED);

	m_fElapsedTime	+= frameEvent.fTimeSinceLastFrame;
	m_fFrameTime	=  frameEvent.fTimeSinceLastFrame;

	FrameListenerList::iterator i;

	bool bRet = true;
	for (i = m_frameListeners.begin(); i != m_frameListeners.end(); ++i)
	{
		if (!(*i)->frameStarted(frameEvent))
		{
			bRet = false;
			break;
		}
	}	
	return bRet;	
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineCore::fireFrameEnded()
{
	unsigned long now = m_pTimer->getMilliseconds();
	FrameEvent frameEvent;        
	frameEvent.fTimeSinceLastFrame = calculateEventTime(now, FETT_ENDED);

	FrameListenerList::iterator i;

	bool bRet = true;
	for (i = m_frameListeners.begin(); i != m_frameListeners.end(); ++i)
	{
		if (!(*i)->frameEnded(frameEvent))
		{
			bRet = false;
			break;
		}
	}	
	return bRet;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCore::addFrameListener(IPepeEngineFrameListener* pFrameListener)
{
	FrameListenerList::iterator i;

	for (i = m_frameListeners.begin(); i != m_frameListeners.end(); ++i)
	{
		if (*i == pFrameListener)
		{			
			return;
		}
	}
	m_frameListeners.push_back(pFrameListener);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCore::removeFrameListener(IPepeEngineFrameListener* pFrameListener)
{
	FrameListenerList::iterator i;

	for (i = m_frameListeners.begin(); i != m_frameListeners.end(); ++i)
	{
		if (*i == pFrameListener)
		{
			delete *i;
			m_frameListeners.erase(i);
			return;
		}
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCore::run()
{	
	// Clear event times
	for (int i = 0; i < 3; ++i)
	{
		m_eventTimes[i].clear();
	}

	while (m_pWindow->run())
	{				
		if (!fireFrameStarted()) break;		
		m_pSceneManager->drawAll();			
		if (!fireFrameEnded()) break;

		m_pRenderer->getRenderWindow()->update();			
	}	
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCore::addRenderer(IPepeEngineRenderer* pRenderer)
{
	m_pRendererList->push_back(pRenderer);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCore::setActiveRenderer(IPepeEngineRenderer* pRenderer)
{
	if (m_pRenderer && pRenderer != pRenderer)
	{
		m_pRenderer->shutdown();
		m_pConfig->detachListener(m_pRenderer);
	}
	m_pRenderer = pRenderer;
	m_pConfig->attachListener(m_pRenderer);
}

// -----------------------------------------------------------------------------------------
IPepeEngineRenderer* CPepeEngineCore::getRendererByName(const tstring& strName) const
{
	if (strName.empty())
	{		
		return NULL;
	}

	RendererList::const_iterator pRend;
	for (pRend = getAvailableRenderers()->begin(); pRend != getAvailableRenderers()->end(); ++pRend)
	{
		IPepeEngineRenderer* r = *pRend;
		if (r->getName() == strName)
		{
			return r;
		}
	}
	
	return NULL;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCore::addWindow(IPepeEngineWindow* pWindow)
{
	m_pWindow = pWindow;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCore::addTimer(IPepeEngineTimer* pTimer)
{
	m_pTimer = pTimer;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCore::addConfigDialog(IPepeEngineConfigDialog* pConfigDialog)
{
	m_pConfigDialog = pConfigDialog;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCore::loadPlugin(const tstring& strPluginName)
{
	// Load plugin library
	DynamicLibraryPtr lib = CPepeEngineDynamicLibraryManager::getSingleton().create(strPluginName);
	lib->load();
	m_pluginLibs.push_back(lib);

	// Call startup function
	DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)lib->getSymbol(_T("dllStartPlugin"));

	if (!pFunc)
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_ITEM_NOT_FOUND_ERROR, 
			_T("Cannot find symbol dllStartPlugin in library ") + strPluginName, 
			_T("CPepeEngineCore::loadPlugin")
		);			
	}

	// This must call installPlugin
	pFunc();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCore::unloadPlugin(const tstring& strPluginName)
{	
	PluginLibList::iterator i;

	for (i = m_pluginLibs.begin(); i != m_pluginLibs.end(); ++i)
	{
		if ((*i)->getName() == strPluginName)
		{
			// Call plugin shutdown
			DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN)(*i)->getSymbol(_T("dllStopPlugin"));
			// this must call uninstallPlugin
			pFunc();
			// Unload library
			CPepeEngineDynamicLibraryManager::getSingleton().unload(strPluginName);
			m_pluginLibs.erase(i);
			return;
		}
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCore::unloadPlugins()
{	
	PluginLibList::iterator i;

	for (i = m_pluginLibs.begin(); i != m_pluginLibs.end(); ++i)
	{		
		// Call plugin shutdown
		DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN)(*i)->getSymbol(_T("dllStopPlugin"));
		// this must call uninstallPlugin
		pFunc();
	}
	m_pluginLibs.clear();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCore::installPlugin(IPepeEnginePlugin* pPlugin)
{
	LOG("Installing plugin: " + pPlugin->getName());
	m_plugins.push_back(pPlugin);
	pPlugin->install();
	
	pPlugin->initialise();	
	LOG("Plugin successfully installed");
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCore::uninstallPlugin(IPepeEnginePlugin* pPlugin)
{
	LOG("Uninstalling plugin: " + pPlugin->getName());
	PluginList::iterator i = std::find(m_plugins.begin(), m_plugins.end(), pPlugin);
	if (i != m_plugins.end())
	{		
		pPlugin->shutdown();
		pPlugin->uninstall();
		m_plugins.erase(i);
	}
	LOG("Plugin successfully uninstalled");
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineCore::showConfigDialog()
{
	return m_pConfigDialog->display();
}

// -----------------------------------------------------------------------------------------
float CPepeEngineCore::getElapsedTime() const
{
	return m_fElapsedTime;
}

// -----------------------------------------------------------------------------------------
float CPepeEngineCore::getFrameTime() const
{
	return m_fFrameTime;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineCore::drawStats()
{
	FrameStats stats = m_pSceneManager->getFrameStats();

	tstring str =	_T("Averange FPS:\t")	+ CPepeEngineConverter::parseString(stats.fAvgFPS)			+ _T("\n") + 
					_T("Best FPS:\t")		+ CPepeEngineConverter::parseString(stats.fBestFPS)			+ _T("\n") +
					_T("Worst FPS:\t")		+ CPepeEngineConverter::parseString(stats.fWorstFPS)		+ _T("\n") +
					_T("Last FPS:\t")		+ CPepeEngineConverter::parseString(stats.fLastFPS)			+ _T("\n") +
					_T("Triangles:\t")		+ CPepeEngineConverter::parseString((float)stats.nTriangleCount);

	m_pRenderer->drawText(str, 10, 10);	
}

// -----------------------------------------------------------------------------------------
template<> CPepeEngineCore* CPepeEngineSingleton<CPepeEngineCore>::ms_singleton = 0;

CPepeEngineCore& CPepeEngineCore::getSingleton(void)
{
	assert(ms_singleton); 
	return *ms_singleton;
}

CPepeEngineCore* CPepeEngineCore::getSingletonPtr(void)
{
	return ms_singleton;
}

_PEPE_ENGINE_END