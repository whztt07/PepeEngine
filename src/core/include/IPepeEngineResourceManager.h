/**
 * Project:		PepeEngine
 * Tier:		Frontend
 * File:		IPepeEngineResourceManager.h   
 *
 * @brief		Declaration of IPepeEngineResourceManager class.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-04-23
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef IPEPEENGINERESOURCEMANAGER_H
#define IPEPEENGINERESOURCEMANAGER_H

#include "PepeEngineInit.h"

#include "IPepeEngineResource.h"

_PEPE_ENGINE_START

class _PepeEngineExport IPepeEngineResourceManager
{
public:

															IPepeEngineResourceManager();
	virtual													~IPepeEngineResourceManager();

	virtual ResourcePtr										create(const tstring& strName) = 0;
	void													add(const tstring& strName, ResourcePtr pResource);
	void													remove(const tstring& strName);

	void													load(const tstring& strName);
	void													unload(const tstring& strName);	

	ResourcePtr												getByName(const tstring& strName) const;

protected:
	typedef stdext::hash_map<tstring, ResourcePtr>			ResourceMap;

	ResourceMap												m_resourceMap;	
};

_PEPE_ENGINE_END

#endif