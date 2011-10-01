/**
 * Project: PepeEngine
 * Tier:    Middleware
 * File:    IPepeEngineVertexBuffer.h
 *
 * @brief   Declaration of IPepeEngineVertexBuffer interface.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-03-16
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef IPEPEENGINEVERTEXBUFFER_H
#define IPEPEENGINEVERTEXBUFFER_H

#include "PepeEngineInit.h"

#include "IPepeEngineBuffer.h"

_PEPE_ENGINE_START

class _PepeEngineExport IPepeEngineVertexBuffer : public IPepeEngineBuffer
{
protected:
    size_t m_uiNumVertices;
    size_t m_uiVertexSize;
public:

    IPepeEngineVertexBuffer(size_t uiNumVertices, size_t uiVertexSize, Usage usage,
                            size_t uiBufferSize, bool bUseSystemMemory)
        :   IPepeEngineBuffer(usage, uiBufferSize, bUseSystemMemory),
            m_uiNumVertices(uiNumVertices), m_uiVertexSize(uiVertexSize) {
        m_bufferType = BT_VERTEX_BUFFER;
    }

    virtual ~IPepeEngineVertexBuffer() {}

    size_t getNumVertices() const {
        return m_uiNumVertices;
    }
    size_t getVertexSize() const {
        return m_uiVertexSize;
    }

};

_PEPE_ENGINE_END

#endif