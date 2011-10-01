/**
 * Project: PepeEngine
 * Tier:    Middleware
 * File:    IPepeEngineBufferManager.h
 *
 * @brief   Declaration of IPepeEngineBufferManager interface.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-06-04
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef IPEPEENGINEBUFFERMANAGER_H
#define IPEPEENGINEBUFFERMANAGER_H

#include "PepeEngineInit.h"

#include "CPepeEngineSingleton.h"
#include "CPepeEngineVertexDeclaration.h"
#include "IPepeEngineBuffer.h"
#include "IPepeEngineIndexBuffer.h"

_PEPE_ENGINE_START

class _PepeEngineExport IPepeEngineBufferManager : public CPepeEngineSingleton<IPepeEngineBufferManager>
{
protected:
    typedef std::vector<IPepeEngineBuffer*> BufferList;
    typedef std::vector<CPepeEngineVertexDeclaration*> VertexDeclarationList;

    BufferList              m_bufferList;
    VertexDeclarationList   m_vertexDeclarationList;
public:

    IPepeEngineBufferManager();
    ~IPepeEngineBufferManager();

    virtual IPepeEngineIndexBuffer* createIndexBuffer(
        size_t uiBufferSize, IPepeEngineBuffer::Usage usage, IPepeEngineIndexBuffer::IndexType indexType,
        bool bUseSystemMemory
    ) = 0;

    virtual IPepeEngineVertexBuffer* createVertexBuffer(
        size_t uiNumVertices, size_t uiVertexSize, IPepeEngineBuffer::Usage usage,
        size_t uiBufferSize, bool bUseSystemMemory
    ) = 0;

    virtual CPepeEngineVertexDeclaration* createVertexDeclaration() = 0;

    static IPepeEngineBufferManager& getSingleton(void);
    static IPepeEngineBufferManager* getSingletonPtr(void);
};

_PEPE_ENGINE_END

#endif