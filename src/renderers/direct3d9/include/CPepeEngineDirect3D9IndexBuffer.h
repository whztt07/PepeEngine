/**
 * Project: PepeEngine
 * Tier:	Backend 
 * File:	CPepeEngineDirect3D9IndexBuffer.h 
 *
 * @brief	Declaration of CPepeEngineDirect3D9IndexBuffer class. This class use Direct3D 9 library. 		
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-03-20
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEDIRECT3D9INDEXBUFFER_H
#define CPEPEENGINEDIRECT3D9INDEXBUFFER_H

#include "PepeEngineDirect3D9Init.h"

#include "IPepeEngineIndexBuffer.h"

_PEPE_ENGINE_START

class CPepeEngineDirect3D9IndexBuffer : public IPepeEngineIndexBuffer
{
	LPDIRECT3DINDEXBUFFER9	m_pBuffer;
	D3DPOOL					m_pool;
	LPDIRECT3DDEVICE9		m_pDevice;
public:

	/**
	 *
	 * @param usage			[Good practice - match this with create device similar parameter]
	 * @param uiBufferSize	 	 
	 */
	CPepeEngineDirect3D9IndexBuffer(LPDIRECT3DDEVICE9 pDevice, size_t uiBufferSize, Usage usage, IndexType indexType, bool bUseSystemMemory);
	~CPepeEngineDirect3D9IndexBuffer();

	void* lock(size_t uiOffset, size_t uiLength, LockOptions options);
	void unlock();	
	
	void readData(size_t uiOffset, size_t uiLength, void* pDest);
	void writeData(size_t uiOffset, size_t uiLength, const void* pSource, bool bDiscardWholeBuffer);

	void releaseIfDefaultPool();
	void recreateIfDefaultPool();

	LPDIRECT3DINDEXBUFFER9 getD3DIndexBuffer(void) {return m_pBuffer;}
};

_PEPE_ENGINE_END

#endif