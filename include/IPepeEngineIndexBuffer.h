/**
 * Project: PepeEngine
 * Tier:    Middleware
 * File:    IPepeEngineIndexBuffer.h
 *
 * @brief   Declaration of IPepeEngineIndexBuffer interface.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-03-20
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef IPEPEENGINEINDEXBUFFER_H
#define IPEPEENGINEINDEXBUFFER_H

#include "PepeEngineInit.h"

#include "IPepeEngineBuffer.h"

_PEPE_ENGINE_START

class _PepeEngineExport IPepeEngineIndexBuffer : public IPepeEngineBuffer
{
public:

    enum IndexType {
        IT_16BIT,
        IT_32BIT
    };

    IPepeEngineIndexBuffer(Usage usage, size_t uiBufferSize, IndexType indexType, bool bUseSystemMemory)
        : IPepeEngineBuffer(usage, uiBufferSize, bUseSystemMemory) {
        m_indexType     = indexType;
        m_bufferType    = BT_INDEX_BUFFER;
    }

    virtual ~IPepeEngineIndexBuffer() {}

protected:
    IndexType m_indexType;
};

_PEPE_ENGINE_END

#endif