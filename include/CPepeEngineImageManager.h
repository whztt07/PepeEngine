/**
 * Project:		PepeEngine
 * Tier:		Frontend
 * File:		CPepeEngineImageManager.h   
 *
 * @brief		Declaration of CPepeEngineImageManager class.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-11-12
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef CPEPEENGINEIMAGEMANAGER_H
#define CPEPEENGINEIMAGEMANAGER_H

#include "PepeEngineInit.h"

#include "CPepeEngineImage.h"
#include "CPepeEngineSingleton.h"
#include "IPepeEngineResourceManager.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineImageManager 
	: public IPepeEngineResourceManager, public CPepeEngineSingleton<CPepeEngineImageManager>
{	
public:
									CPepeEngineImageManager();
									~CPepeEngineImageManager();

	ResourcePtr						create(const tstring& strName);

	static CPepeEngineImageManager& getSingleton(void);
	static CPepeEngineImageManager* getSingletonPtr(void);	
};

_PEPE_ENGINE_END

#endif