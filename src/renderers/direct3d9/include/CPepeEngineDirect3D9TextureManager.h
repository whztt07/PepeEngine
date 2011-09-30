/**
 * Project: PepeEngine
 * Tier:	Backend 
 * File:	CPepeEngineDirect3D9TextureManager.h 
 *
 * @brief	Declaration of CPepeEngineDirect3D9TextureManager class. This class use Direct3D 9 library. 		
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-05-08
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEDIRECT3D9TEXTUREMANAGER_H
#define CPEPEENGINEDIRECT3D9TEXTUREMANAGER_H

#include "PepeEngineDirect3D9Init.h"

#include "CPepeEngineDirect3D9Texture.h"
#include "IPepeEngineTextureManager.h"

_PEPE_ENGINE_START

class CPepeEngineDirect3D9TextureManager : public IPepeEngineTextureManager
{	
	LPDIRECT3DDEVICE9 m_pDevice;
public:

	CPepeEngineDirect3D9TextureManager(LPDIRECT3DDEVICE9 pDevice) : m_pDevice(pDevice)  {}

	ResourcePtr create(const tstring& strName);

	void releaseDefaultPoolResources();
	void recreateDefaultPoolResources();
};


_PEPE_ENGINE_END

#endif