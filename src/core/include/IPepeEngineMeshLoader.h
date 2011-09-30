/**
 * Project: PepeEngine
 * Tier:	Frontend
 * File:	IPepeEngineMeshLoader.h   
 *
 * @brief	Declaration of IPepeEngineMeshLoader interface.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-06-05
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef IPEPEENGINEMESHLOADER_H
#define IPEPEENGINEMESHLOADER_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

class _PepeEngineExport IPepeEngineMeshLoader
{	
protected:
	std::ifstream fin;
public:
		
	virtual ~IPepeEngineMeshLoader() {}

	virtual void load(const tstring& strFileName, MeshPtr pMesh) = 0;
	virtual bool isAvailableFileExtension(const tstring& strFileName) const = 0;
	virtual void reset() = 0;	
};

_PEPE_ENGINE_END

#endif