/**
 * Project:		PepeEngine
 * Tier:		Middleware
 * File:		IPepeEngineTextureManager.h 
 *
 * @brief		Declaration of IPepeEngineTextureManager interface.  
 *				Loads and unloads textures and uses reference counting to ensure that textures are only released 
 *				when they are no longer being used. 
 *				If something requests a texture that has already been loaded, a handle to the already 
 *				loaded texture is returned and the reference count incremented. 
 *				Likewise, when a texture is finished with they can either delete the handle function which will decrement the reference count. 
 *				Once the ref count hits zero, it can be safely removed from video memory.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-04-23
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef IPEPEENGINETEXTUREMANAGER_H
#define IPEPEENGINETEXTUREMANAGER_H

#include "PepeEngineInit.h"

#include "CPepeEngineSingleton.h"
#include "IPepeEngineResourceManager.h" 
#include "IPepeEngineTexture.h" 

_PEPE_ENGINE_START

class _PepeEngineExport IPepeEngineTextureManager : public IPepeEngineResourceManager, public CPepeEngineSingleton<IPepeEngineTextureManager>
{
	tstring m_strFilePath;
public:		
	virtual ~IPepeEngineTextureManager() {}

	void setFilePath(const tstring& strFilePath) {m_strFilePath = strFilePath;}
	const tstring& getFilePath() const {return m_strFilePath;}

	static IPepeEngineTextureManager& getSingleton(void);
	static IPepeEngineTextureManager* getSingletonPtr(void);
};

_PEPE_ENGINE_END

#endif