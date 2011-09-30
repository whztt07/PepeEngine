/**
 * Project:		PepeEngine
 * Tier:		Frontend 
 * File:		CPepeEngineMaterialManager.h 
 *
 * @brief		Declaration of CPepeEngineMaterialManager class. 
 *			
 * @author		Piotr 'pepe' Picheta
 * @date		2008-02-28
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#pragma once

#ifndef CPEPEENGINEMATERIALMANAGER_H
#define CPEPEENGINEMATERIALMANAGER_H

#include "PepeEngineInit.h"

#include "CPepeEngineMaterial.h"
#include "CPepeEngineSingleton.h"
#include "IPepeEngineResourceManager.h" 

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineMaterialManager 
	: public IPepeEngineResourceManager, public CPepeEngineSingleton<CPepeEngineMaterialManager>
{	
public:	
										CPepeEngineMaterialManager();
	
	ResourcePtr							create(const tstring& strName);

	tstring								generateGUID();	

	static CPepeEngineMaterialManager&	getSingleton(void);
	static CPepeEngineMaterialManager*	getSingletonPtr(void);	
private:
	unsigned int						m_nCounter;	
};

_PEPE_ENGINE_END

#endif