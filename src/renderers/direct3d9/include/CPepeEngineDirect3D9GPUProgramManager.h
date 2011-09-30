/**
 * Project: PepeEngine
 * Tier:	Backend 
 * File:	CPepeEngineDirect3D9GPUProgramManager.h 
 *
 * @brief	Declaration of CPepeEngineDirect3D9GPUProgramManager class. This class use Direct3D 9 library. 		
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-09-17
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEDIRECT3D9GPUPROGRAMMANAGER_H
#define CPEPEENGINEDIRECT3D9GPUPROGRAMMANAGER_H

#include "PepeEngineDirect3D9Init.h"

#include "IPepeEngineGPUProgramManager.h"

_PEPE_ENGINE_START

class CPepeEngineDirect3D9GPUProgramManager : public IPepeEngineGPUProgramManager
{	
	LPDIRECT3DDEVICE9 m_pDevice;
public:

	CPepeEngineDirect3D9GPUProgramManager(LPDIRECT3DDEVICE9 pDevice) : m_pDevice(pDevice)  {}

	ResourcePtr create(const tstring& strName);
	
	GPUProgramPtr createPixelShader(
		const tstring& strName, 
		const tstring& strShaderModel, 
		const tstring& strEntryPoint
	);
	
	GPUProgramPtr createVertexShader(
		const tstring& strName, 
		const tstring& strShaderModel, 
		const tstring& strEntryPoint
	);
};

#endif

_PEPE_ENGINE_END