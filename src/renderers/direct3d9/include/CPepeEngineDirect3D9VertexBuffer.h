/**
 * Project: PepeEngine
 * Tier:    Backend
 * File:    CPepeEngineDirect3D9VertexBuffer.h
 *
 * @brief   Declaration of CPepeEngineDirect3D9VertexBuffer class. This class use Direct3D 9 library.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-03-20
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEDIRECT3D9VERTEXBUFFER_H
#define CPEPEENGINEDIRECT3D9VERTEXBUFFER_H

#include "PepeEngineDirect3D9Init.h"

#include "IPepeEngineVertexBuffer.h"

_PEPE_ENGINE_START

class CPepeEngineDirect3D9VertexBuffer : public IPepeEngineVertexBuffer
{
    LPDIRECT3DVERTEXBUFFER9 m_pBuffer;
    D3DPOOL                 m_pool;
    LPDIRECT3DDEVICE9       m_pDevice;
public:

    CPepeEngineDirect3D9VertexBuffer(
        LPDIRECT3DDEVICE9 pDevice, size_t uiNumVertices, size_t uiVertexSize, Usage usage, size_t uiBufferSize, bool bUseSystemMemory);
    ~CPepeEngineDirect3D9VertexBuffer();

    void* lock(size_t uiOffset, size_t uiLength, LockOptions options);
    void unlock();

    void readData(size_t uiOffset, size_t uiLength, void* pDest);
    void writeData(size_t uiOffset, size_t uiLength, const void* pSource, bool bDiscardWholeBuffer);

    void releaseIfDefaultPool();
    void recreateIfDefaultPool();

    LPDIRECT3DVERTEXBUFFER9 getD3DVertexBuffer() const {
        return m_pBuffer;
    }

};

_PEPE_ENGINE_END

#endif