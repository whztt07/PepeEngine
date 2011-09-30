#include "CPepeEngineDirect3D9TextureManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
ResourcePtr CPepeEngineDirect3D9TextureManager::create(const tstring& strName)
{
	ResourcePtr pTexture = getByName(strName);
	
	if (pTexture.isNull())
	{
		pTexture = new CPepeEngineDirect3D9Texture(m_pDevice, strName);

		add(strName, pTexture);	
	}
	
	return pTexture;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9TextureManager::releaseDefaultPoolResources()
{
	ResourceMap::iterator it, rend = m_resourceMap.end();

	for (it = m_resourceMap.begin(); it != rend; it++)
	{
		IPepeEngineTexture* tex			= (TexturePtr)it->second;
		CPepeEngineDirect3D9Texture* t	= static_cast<CPepeEngineDirect3D9Texture*>(tex);
		t->releaseIfDefaultPool();
	}

}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9TextureManager::recreateDefaultPoolResources()
{
	ResourceMap::iterator it, rend = m_resourceMap.end();

	for (it = m_resourceMap.begin(); it != rend; it++)
	{
		IPepeEngineTexture* tex			= (TexturePtr)it->second;
		CPepeEngineDirect3D9Texture* t	= static_cast<CPepeEngineDirect3D9Texture*>(tex);
		t->recreateIfDefaultPool();
	}
}

_PEPE_ENGINE_END