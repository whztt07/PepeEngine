#include "PepeEnginePrecompiledHeaders.h"
#include "IPepeEngineBuffer.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
void* IPepeEngineBuffer::lock(LockOptions options)
{
	return this->lock(0, m_uiBufferSize, options);
}

_PEPE_ENGINE_END