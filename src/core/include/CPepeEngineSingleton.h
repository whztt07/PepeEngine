/**
 * Project: PepeEngine
 * Tier:	Frontend
 * File:	CPepeEngineSingleton.h   
 *
 * @brief	Declaration of CPepeEngineSingleton class.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-04-06
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINESINGLETON_H
#define CPEPEENGINESINGLETON_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

template <typename T>
class _PepeEngineExport CPepeEngineSingleton
{
protected:
	static T* ms_singleton;
public:

	CPepeEngineSingleton()
	{
		assert(!ms_singleton);
		ms_singleton = static_cast<T*>(this);
	}

	virtual ~CPepeEngineSingleton()
	{
		assert(ms_singleton);
		ms_singleton = 0;
	}
		 
	static T& getSingleton()
	{
		assert(ms_singleton);  
		return *ms_singleton; 
	}

	static T* getSingletonPtr()
	{
		return ms_singleton; 
	}

};

_PEPE_ENGINE_END

#endif