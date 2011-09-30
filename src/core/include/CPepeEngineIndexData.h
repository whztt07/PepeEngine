/**
* Project:	PepeEngine
* Tier:		Frontend
* File:		CPepeEngineIndexData.h   
*
* @brief	Declaration of CPepeEngineIndexData class.
*
* @author		Piotr 'pepe' Picheta
* @date			2008-07-09
* @copyright	Copyright (c) 2008 Piotr Picheta
*
* @version 1.0
*/

#ifndef CPEPEENGINEINDEXDATA_H
#define CPEPEENGINEINDEXDATA_H

#include "PepeEngineInit.h"

#include "IPepeEngineIndexBuffer.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineIndexData
{	
public:
	CPepeEngineIndexData();
	~CPepeEngineIndexData();

	size_t	m_nIndexStart;
	size_t	m_nIndexCount;

	IPepeEngineIndexBuffer* m_pIndexBuffer;		
};

_PEPE_ENGINE_END

#endif