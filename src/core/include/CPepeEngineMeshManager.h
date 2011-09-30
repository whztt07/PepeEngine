/**
 * Project:		PepeEngine
 * Tier:		Frontend 
 * File:		CPepeEngineMeshManager.h 
 *
 * @brief		Declaration of CPepeEngineMeshManager class.  
 *			
 * @author		Piotr 'pepe' Picheta
 * @date		2008-06-18
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef CPEPEENGINEMESHMANAGER_H
#define CPEPEENGINEMESHMANAGER_H

#include "PepeEngineInit.h"

#include "CPepeEngine3DSFileMeshLoader.h"
#include "CPepeEngineMesh.h"
#include "CPepeEngineSingleton.h"
#include "CPepeEngineXFileMeshLoader.h"
#include "IPepeEngineMeshLoader.h"
#include "IPepeEngineResourceManager.h" 

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineMeshManager 
	: public IPepeEngineResourceManager, public CPepeEngineSingleton<CPepeEngineMeshManager>
{
	mutable std::vector<IPepeEngineMeshLoader* >	m_meshLoadersList;
	tstring											m_strFilePath;
public:

	CPepeEngineMeshManager();
	~CPepeEngineMeshManager();

	ResourcePtr create(const tstring& strName);

	void setFilePath(const tstring& strFilePath) {m_strFilePath = strFilePath;}
	const tstring& getFilePath() const {return m_strFilePath;}

	void addMeshLoader(IPepeEngineMeshLoader* pMeshLoader);
	IPepeEngineMeshLoader* getMeshLoader(const tstring& strName) const;

	static CPepeEngineMeshManager& getSingleton(void);
	static CPepeEngineMeshManager* getSingletonPtr(void);
};

_PEPE_ENGINE_END

#endif