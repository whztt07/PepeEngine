/**
* Project:  PepeEngine
* Tier:     Middleware
* File:     IPepeEngineBuffer.h
*
* @brief    Declaration of IPepeEngineBuffer abstract class. (Inspired by Ogre :) )
*
* @author       Piotr 'pepe' Picheta
* @date         2008-03-20
* @copyright    Copyright (c) 2008 Piotr Picheta
*
* @version 1.0
*/

#ifndef IPEPEENGINEBUFFER_H
#define IPEPEENGINEBUFFER_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

enum BufferType {
    BT_VERTEX_BUFFER    = 0,
    BT_INDEX_BUFFER     = 1
};

class _PepeEngineExport IPepeEngineBuffer
{
public:
    enum Usage {
        BU_STATIC = 1,                          /**< Static buffer which the application rarely modifies once created. Modifying
                                                     the contents of this buffer will involve a performance hit. */
        BU_DYNAMIC = 2,                         /**< Indicates the application would like to modify this buffer with the CPU
                                                     fairly often.  Buffers created with this flag will typically end up in AGP memory rather
                                                     than video memory. */
        BU_WRITE_ONLY = 4,                      /**< Indicates the application will never read the contents of the buffer back,
                                                     it will only ever write data. Locking a buffer with this flag will ALWAYS
                                                     return a pointer to new, blank memory rather than the memory associated
                                                     with the contents of the buffer; this avoids DMA stalls because you can
                                                     write to a new memory area while the previous one is being used. */
        BU_DISCARDABLE = 8,                     /**< Indicates that the application will be refilling the contents
                                                     of the buffer regularly (not just updating, but generating the
                                                     contents from scratch), and therefore does not mind if the contents
                                                     of the buffer are lost somehow and need to be recreated. This
                                                     allows and additional level of optimisation on the buffer.
                                                     This option only really makes sense when combined with
                                                     BU_DYNAMIC_WRITE_ONLY. */
        BU_STATIC_WRITE_ONLY = 5,               /**< Combination of BU_STATIC and BU_WRITE_ONLY */
        BU_DYNAMIC_WRITE_ONLY = 6,              /**< Combination of BU_DYNAMIC and BU_WRITE_ONLY. If you use
                                                     this, strongly consider using BU_DYNAMIC_WRITE_ONLY_DISCARDABLE
                                                     instead if you update the entire contents of the buffer very regularly. */
        BU_DYNAMIC_WRITE_ONLY_DISCARDABLE = 14  /**< Combination of BU_DYNAMIC, BU_WRITE_ONLY and BU_DISCARDABLE */
    };

    enum LockOptions {
        BL_NORMAL,          /**< Normal mode, ie allows read/write and contents are preserved. */
        BL_DISCARD,         /**< Discards the entire buffer while locking; this allows optimisation to be
                                 performed because synchronisation issues are relaxed. Only allowed on buffers
                                 created with the BL_DYNAMIC flag. */
        BL_READ_ONLY,       /**< Lock the buffer for reading only. Not allowed in buffers which are created with BU_WRITE_ONLY.
                                 Mandatory on static buffers, ie those created without the BU_DYNAMIC flag. */
        BL_NO_OVERWRITE     /**< As BL_NORMAL, except the application guarantees not to overwrite any
                                 region of the buffer which has already been used in this frame, can allow
                                 some optimisation on some APIs. */
    };
protected:
    size_t      m_uiBufferSize;
    size_t      m_uiLockStart;
    size_t      m_uiLockSize;
    bool        m_bIsLocked;
    bool        m_bUseSystemMemory;
    Usage       m_usage;
    BufferType  m_bufferType;
public:

    IPepeEngineBuffer(Usage usage, size_t uiBufferSize, bool bUseSystemMemory)
        :   m_bIsLocked(false),
            m_usage(usage),
            m_uiBufferSize(uiBufferSize),
            m_bUseSystemMemory(bUseSystemMemory)
    {}

    virtual ~IPepeEngineBuffer() {}


    virtual void* lock(size_t uiOffset, size_t uiLength, LockOptions options) = 0;
    inline void* lock(LockOptions options);

    virtual void unlock() = 0;
    bool isLocked() const {
        return m_bIsLocked;
    }

    virtual void readData(size_t uiOffset, size_t uiLength, void* pDest) = 0;
    virtual void writeData(size_t uiOffset, size_t uiLength, const void* pSource, bool bDiscardWholeBuffer) = 0;

    size_t getBufferSize() const {
        return m_uiBufferSize;
    }
    Usage getUsage() const {
        return m_usage;
    }
    BufferType getBufferType() const {
        return m_bufferType;
    }
};

_PEPE_ENGINE_END

#endif