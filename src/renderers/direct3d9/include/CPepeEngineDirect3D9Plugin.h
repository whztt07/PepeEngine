/**
 * Project: PepeEngine
 * Tier:	Backend 
 * File:	CPepeEngineDirect3D9Plugin.h 
 *
 * @brief	Declaration of CPepeEngineDirect3D9Plugin class. This class use Direct3D 9 library. 		
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-05-26
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEDIRECT3D9PLUGIN_H
#define CPEPEENGINEDIRECT3D9PLUGIN_H

#include "PepeEngineDirect3D9Init.h"

#include "IPepeEnginePlugin.h"
#include "CPepeEngineDirect3D9Renderer.h"

_PEPE_ENGINE_START

class CPepeEngineDirect3D9Plugin : public IPepeEnginePlugin
{
protected:
	CPepeEngineDirect3D9Renderer* m_pRenderer;
public:
	
	CPepeEngineDirect3D9Plugin();	
	
	const tstring& getName() const;
	
	void install();
	void initialise();
	void shutdown();
	void uninstall();
};

_PEPE_ENGINE_END

#endif