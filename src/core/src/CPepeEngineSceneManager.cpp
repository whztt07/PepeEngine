#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineMaterial.h"
#include "CPepeEngineSceneManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::addToNodeList(CPepeEngineSceneNode* pNode)
{
	m_nodesMap.insert(make_pair(pNode->getName(), pNode));
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::removeFromNodeList(const tstring& strName)
{
	SceneNodeMap::iterator it = m_nodesMap.find(strName);

	if (it != m_nodesMap.end())
	{		
		m_nodesMap.erase(it);		
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::removeFromNodeList(CPepeEngineSceneNode* pNode)
{
	SceneNodeMap::iterator it = m_nodesMap.begin(), endit = m_nodesMap.end();

	while (it != endit)
	{				
		if (it->second == pNode)
		{
			it->second = NULL;
			m_nodesMap.erase(it);	
			break;
		}
		it++;
	}
}

// -----------------------------------------------------------------------------------------
CPepeEngineSceneManager::CPepeEngineSceneManager()
{	
	m_pRoot				= new CPepeEngineSceneNode(_T("Root"), this);
	m_pCamera			= new CPepeEngineCamera();
	m_pSkyBox			= NULL;
	m_bDebugMode		= false;
	m_nodesMap.clear();

	m_autoParamDataSource.setCamera(m_pCamera);

	m_bgColor.set(0.0f, 0.0f, 0.0f);

	resetStats();
	addToNodeList(m_pRoot);
}

// -----------------------------------------------------------------------------------------
CPepeEngineSceneManager::~CPepeEngineSceneManager()
{	
	delete m_pRoot;
	delete m_pCamera;
	for (LightList::iterator it = m_lights.begin(); it != m_lights.end(); it++)
	{
		delete *it;
	}
	m_lights.clear();
	m_nodesMap.clear();
	removeSkyBox();
}

// -----------------------------------------------------------------------------------------
CPepeEngineSceneNode* CPepeEngineSceneManager::getRootSceneNode() const
{
	return m_pRoot;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::setRootSceneNode(CPepeEngineSceneNode *pNode)
{
	m_pRoot = pNode;
}

// -----------------------------------------------------------------------------------------
CPepeEngineSceneNode* CPepeEngineSceneManager::createSceneNode(const tstring& strName)
{
	return new CPepeEngineSceneNode(strName, this);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::createSkyBox(const tstring& strSkyBoxName, size_t nSize)
{
	m_pSkyBox = new CPepeEngineSkyBox(strSkyBoxName, nSize);	
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::removeSkyBox()
{
	delete m_pSkyBox;
	m_pSkyBox = NULL;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::setAmbientLight(float r, float g, float b)
{
	CPepeEngineCore::getSingleton().getRenderer()->setAmbientLight(r, g, b);

	m_autoParamDataSource.setAmbientLightColor(CPepeEngineColor(r, g, b));
}

// -----------------------------------------------------------------------------------------
CPepeEngineLight* CPepeEngineSceneManager::createLight(const tstring& strName)
{
	CPepeEngineLight* pLight = getLight(strName);

	if (pLight == NULL)
	{
		pLight = new CPepeEngineLight(strName);
		m_lights.push_back(pLight);
		return pLight;
	} else
	{
		return pLight;
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::addLight(CPepeEngineLight* pLight)
{
	assert(pLight != NULL);

	if (!hasLight(pLight))
	{
		m_lights.push_back(pLight);
	}
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineSceneManager::hasLight(const CPepeEngineLight* pLight)
{
	for (LightList::iterator it = m_lights.begin(); it != m_lights.end(); it++)
	{
		if (*it == pLight)
		{
			return true;
		}
	}
	return false;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineSceneManager::hasLight(const tstring& strName)
{
	for (LightList::iterator it = m_lights.begin(); it != m_lights.end(); it++)
	{
		CPepeEngineLight* l = static_cast<CPepeEngineLight*>(*it);
		if (l->getName() == strName)
		{
			return true;
		}
	}
	return false;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::removeLight(CPepeEngineLight* pLight)
{
	for (LightList::iterator it = m_lights.begin(); it != m_lights.end(); it++)
	{
		if (*it == pLight)
		{
			delete *it;
			m_lights.erase(it);
			return;
		}
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::removeLight(const tstring& strName)
{
	for (LightList::iterator it = m_lights.begin(); it != m_lights.end(); it++)
	{
		CPepeEngineLight* l = static_cast<CPepeEngineLight*>(*it);
		if (l->getName() == strName)
		{
			delete *it;
			m_lights.erase(it);
			return;
		}
	}
}

// -----------------------------------------------------------------------------------------
CPepeEngineLight* CPepeEngineSceneManager::getLight(const tstring& strName)
{
	for (LightList::iterator it = m_lights.begin(); it != m_lights.end(); it++)
	{
		CPepeEngineLight* l = static_cast<CPepeEngineLight*>(*it);
		if (l->getName() == strName)
		{
			return *it;
		}
	}
	return NULL;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::setCurrentMaterial(MaterialPtr pMaterial)
{
	m_pCurrentMaterial = pMaterial;

	m_autoParamDataSource.setMaterialAmbientColor(pMaterial->getAmbient());
	m_autoParamDataSource.setMaterialEmissiveColor(pMaterial->getEmissive());
	m_autoParamDataSource.setMaterialDiffuseColor(pMaterial->getDiffuse());
	m_autoParamDataSource.setMaterialSpecularColor(pMaterial->getSpecular());
	m_autoParamDataSource.setMaterialPower(pMaterial->getPower());
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::drawAll()
{
	IPepeEngineRenderer* renderer = CPepeEngineCore::getSingleton().getRenderer();

	renderer->beginScene();
	renderer->setViewMatrix(m_pCamera->getTransformMatrix());		
	renderer->setProjectionMatrix(m_pCamera->getProjectionMatrixRS());	
	renderer->clearFrameBuffer(FBT_COLOR|FBT_DEPTH, m_bgColor);	

	m_autoParamDataSource.setCamera(m_pCamera);
	
	findLightsAffectingFrustum();

	renderer->useLights(m_lightsAffectingFrustum, MAX_LIGHTS);

	m_autoParamDataSource.setLightList(&m_lightsAffectingFrustum);

	if (m_pSkyBox)
	{		
		m_autoParamDataSource.setCurrentRenderable(m_pSkyBox);
		m_pSkyBox->renderFromCamera();
	}
	m_pRoot->render();

	renderer->endScene();

	updateStats();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::findLightsAffectingFrustum()
{
	m_lightsAffectingFrustum.clear();
	for (LightList::iterator it = m_lights.begin(); it != m_lights.end(); it++)
	{
		CPepeEngineLight* l = static_cast<CPepeEngineLight*>(*it);

		if (l->isVisible())
		{		
			if (l->getLightType() == LT_DIRECTIONAL)
			{				
				m_lightsAffectingFrustum.push_back(l);
			}
			else
			{
				/** 
				 *	Treating spotlight as point for simplicity
				 *	Just see if the lights attenuation range is within the frustum
				 */				
				CPepeEngineSphere sphere(l->getDerivedPosition(), l->getRange());
				if (m_pCamera->isVisible(sphere))
				{
					m_lightsAffectingFrustum.push_back(l);
				}
			}
		}
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::updateStats()
{
	m_nFrameCount++;

	unsigned long thisTime	= CPepeEngineCore::getSingleton().getTimer()->getMilliseconds();
	// check frame time
	unsigned long frameTime = thisTime - m_ulLastTime;

	m_ulLastTime				= thisTime;
	m_stats.ulBestFrameTime		= std::min(m_stats.ulBestFrameTime, frameTime);
	m_stats.ulWorstFrameTime	= std::max(m_stats.ulWorstFrameTime, frameTime);

	// check if new second (update only once per second)
	if (thisTime - m_ulLastSecond > 1000) 
	{ 
		// new second - not 100% precise
		m_stats.fLastFPS = (float)m_nFrameCount / (float)(thisTime - m_ulLastSecond) * 1000.0f;

		if (m_stats.fAvgFPS == 0)
		{
			m_stats.fAvgFPS = m_stats.fLastFPS;
		} else
		{
			m_stats.fAvgFPS = (m_stats.fAvgFPS + m_stats.fLastFPS) / 2;
		}

		m_stats.fBestFPS	= std::max(m_stats.fBestFPS, m_stats.fLastFPS);
		m_stats.fWorstFPS	= std::min(m_stats.fWorstFPS, m_stats.fLastFPS);

		m_ulLastSecond		= thisTime;
		m_nFrameCount		= 0;
	}
	m_stats.nTriangleCount = CPepeEngineCore::getSingleton().getRenderer()->getFacesCount();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::resetStats()
{
	m_stats.fAvgFPS				= 0.0f;
	m_stats.fBestFPS			= 0.0f;
	m_stats.fLastFPS			= 0.0f;
	m_stats.fWorstFPS			= 999.0f;
	m_stats.nTriangleCount		= 0;
	m_stats.nBatchCount			= 0;
	m_stats.ulBestFrameTime		= 999999;
	m_stats.ulWorstFrameTime	= 0;
	m_ulLastTime				= CPepeEngineCore::getSingleton().getTimer()->getMilliseconds();
	m_ulLastSecond				= m_ulLastTime;
	m_nFrameCount				= 0;
}

// -----------------------------------------------------------------------------------------
CPepeEngineSceneNode* CPepeEngineSceneManager::getSceneNode(const tstring& strName) const
{
	SceneNodeMap::const_iterator it = m_nodesMap.find(strName);

	if (it == m_nodesMap.end())
	{
		return NULL;
	} else
	{
		return it->second;
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSceneManager::setDebugMode(bool bDebugMode)
{
	m_bDebugMode = bDebugMode;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineSceneManager::isDebugMode() const
{
	return m_bDebugMode;
}

_PEPE_ENGINE_END