#include "CPepeEngineDirect3D9IndexBuffer.h"
#include "CPepeEngineDirect3D9Mapper.h"
#include "CPepeEngineException.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineDirect3D9IndexBuffer::CPepeEngineDirect3D9IndexBuffer(
    LPDIRECT3DDEVICE9 pDevice, size_t uiBufferSize, Usage usage, IndexType indexType, bool bUseSystemMemory)
    : m_pDevice(pDevice), IPepeEngineIndexBuffer(usage, uiBufferSize, indexType, bUseSystemMemory)

{

#if PEPE_ENGINE_D3D_MANAGE_BUFFERS
    m_pool = bUseSystemMemory ? D3DPOOL_SYSTEMMEM :
             /**
              * If not system mem, use managed pool UNLESS buffer is discardable
              * if discardable, keeping the software backing is expensive
              */
             (usage & IPepeEngineBuffer::BU_DISCARDABLE) ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;
#else
    m_pool = bUseSystemMemory ? D3DPOOL_SYSTEMMEM : D3DPOOL_DEFAULT;
#endif

    HRESULT hr = m_pDevice->CreateIndexBuffer(
                     static_cast<unsigned int>(uiBufferSize),
                     CPepeEngineDirect3D9Mapper::get(usage),
                     CPepeEngineDirect3D9Mapper::get(indexType),
                     m_pool,
                     &m_pBuffer,
                     NULL
                 );

    if (FAILED(hr)) {
        tstring msg = DXGetErrorDescription9(hr);

        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_RENDERINGAPI_ERROR,
            _T("Cannot create D3D9 index buffer: ") + msg,
            _T("CPepeEngineDirect3D9IndexBuffer::CPepeEngineDirect3D9IndexBuffer")
        );
    }

}

// -----------------------------------------------------------------------------------------
CPepeEngineDirect3D9IndexBuffer::~CPepeEngineDirect3D9IndexBuffer()
{
    SAFE_RELEASE(m_pBuffer);
}

// -----------------------------------------------------------------------------------------
void* CPepeEngineDirect3D9IndexBuffer::lock(size_t uiOffset, size_t uiLength, LockOptions options)
{
    void* pBuf;

    HRESULT hr = m_pBuffer->Lock(
                     static_cast<unsigned int>(uiOffset),
                     static_cast<unsigned int>(uiLength),
                     &pBuf,
                     CPepeEngineDirect3D9Mapper::get(options, m_usage)
                 );

    if (FAILED(hr)) {
        tstring msg = DXGetErrorDescription9(hr);

        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_RENDERINGAPI_ERROR,
            _T("Cannot lock D3D9 index buffer: ") + msg,
            _T("CPepeEngineDirect3D9IndexBuffer::lock")
        );
    }

    return pBuf;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9IndexBuffer::unlock()
{
    HRESULT hr = m_pBuffer->Unlock();

    if (FAILED(hr)) {
        tstring msg = DXGetErrorDescription9(hr);

        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_RENDERINGAPI_ERROR,
            _T("Cannot unlock D3D9 index buffer: ") + msg,
            _T("CPepeEngineDirect3D9IndexBuffer::unlock")
        );
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9IndexBuffer::readData(size_t uiOffset, size_t uiLength, void* pDest)
{
    void* pSrc = this->lock(uiOffset, uiLength, IPepeEngineBuffer::BL_READ_ONLY);
    memcpy(pDest, pSrc, uiLength);
    this->unlock();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9IndexBuffer::writeData(size_t uiOffset, size_t uiLength, const void* pSource, bool bDiscardWholeBuffer)
{
    void* pDst = this->lock(uiOffset, uiLength, bDiscardWholeBuffer ? IPepeEngineBuffer::BL_DISCARD : IPepeEngineBuffer::BL_NORMAL);
    memcpy(pDst, pSource, uiLength);
    this->unlock();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9IndexBuffer::releaseIfDefaultPool()
{
    if (m_pool == D3DPOOL_DEFAULT) {
        SAFE_RELEASE(m_pBuffer);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9IndexBuffer::recreateIfDefaultPool()
{
    if (m_pool == D3DPOOL_DEFAULT) {
        HRESULT hr = m_pDevice->CreateIndexBuffer(
                         static_cast<unsigned int>(m_uiBufferSize),
                         CPepeEngineDirect3D9Mapper::get(m_usage),
                         CPepeEngineDirect3D9Mapper::get(m_indexType),
                         m_pool,
                         &m_pBuffer,
                         NULL
                     );

        if (FAILED(hr)) {
            tstring msg = DXGetErrorDescription9(hr);

            PEPE_ENGINE_EXCEPT(
                CPepeEngineException::ERR_RENDERINGAPI_ERROR,
                _T("Cannot recreate D3D9 index buffer: ") + msg,
                _T("CPepeEngineDirect3D9IndexBuffer::recreateIfDefaultPool")
            );
        }
    }
}

_PEPE_ENGINE_END