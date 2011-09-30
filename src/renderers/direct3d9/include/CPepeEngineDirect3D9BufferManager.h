/**
 * Project: PepeEngine
 * Tier:	Backend
 * File:	CPepeEngineDirect3D9BufferManager.h 
 *
 * @brief	Declaration of CPepeEngineDirect3D9BufferManager class. This class use Direct3D 9 library. 		
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-06-04
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEDIRECT3D9BUFFERMANAGER_H
#define CPEPEENGINEDIRECT3D9BUFFERMANAGER_H

#include "PepeEngineDirect3D9Init.h"

#include "CPepeEngineDirect3D9IndexBuffer.h"
#include "CPepeEngineDirect3D9VertexBuffer.h"
#include "CPepeEngineDirect3D9VertexDeclaration.h"
#include "IPepeEngineBufferManager.h"

_PEPE_ENGINE_START

class CPepeEngineDirect3D9BufferManager : public IPepeEngineBufferManager
{	
	LPDIRECT3DDEVICE9 m_pDevice;
public:

	CPepeEngineDirect3D9BufferManager(LPDIRECT3DDEVICE9 pDevice) : m_pDevice(pDevice)  {}

	IPepeEngineIndexBuffer* createIndexBuffer(
		size_t uiBufferSize, IPepeEngineBuffer::Usage usage, IPepeEngineIndexBuffer::IndexType indexType, 
		bool bUseSystemMemory
	);

	IPepeEngineVertexBuffer* createVertexBuffer(
		size_t uiNumVertices, size_t uiVertexSize, IPepeEngineBuffer::Usage usage, 
		size_t uiBufferSize, bool bUseSystemMemory
	);

	CPepeEngineVertexDeclaration* createVertexDeclaration();

	void releaseDefaultPoolResources();
	void recreateDefaultPoolResources();
};


_PEPE_ENGINE_END

#endif