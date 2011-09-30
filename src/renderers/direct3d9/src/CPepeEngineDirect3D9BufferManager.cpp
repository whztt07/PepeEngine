#include "CPepeEngineDirect3D9BufferManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
IPepeEngineIndexBuffer* CPepeEngineDirect3D9BufferManager::createIndexBuffer(
	size_t uiBufferSize, IPepeEngineBuffer::Usage usage, IPepeEngineIndexBuffer::IndexType indexType, 
	bool bUseSystemMemory
)
{
	assert(uiBufferSize > 0);
	CPepeEngineDirect3D9IndexBuffer* buffer = new CPepeEngineDirect3D9IndexBuffer(
		m_pDevice, uiBufferSize, usage, indexType, bUseSystemMemory
	);
	m_bufferList.push_back(buffer);
	return buffer;
}

// -----------------------------------------------------------------------------------------
IPepeEngineVertexBuffer* CPepeEngineDirect3D9BufferManager::createVertexBuffer(
	size_t uiNumVertices, size_t uiVertexSize, IPepeEngineBuffer::Usage usage, 
	size_t uiBufferSize, bool bUseSystemMemory
)
{
	assert(uiNumVertices > 0);

	CPepeEngineDirect3D9VertexBuffer* buffer = new CPepeEngineDirect3D9VertexBuffer(
		m_pDevice, uiNumVertices, uiVertexSize, usage, uiBufferSize, bUseSystemMemory
	);
	m_bufferList.push_back(buffer);
	return buffer;
}

// -----------------------------------------------------------------------------------------
CPepeEngineVertexDeclaration* CPepeEngineDirect3D9BufferManager::createVertexDeclaration()
{
	CPepeEngineDirect3D9VertexDeclaration* vertexDeclaration = new CPepeEngineDirect3D9VertexDeclaration(m_pDevice);
	m_vertexDeclarationList.push_back(vertexDeclaration);
	return vertexDeclaration;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9BufferManager::releaseDefaultPoolResources()
{
	BufferList::iterator it = m_bufferList.begin();
	while (it != m_bufferList.end())
	{			
		if ((*it)->getBufferType() == BT_VERTEX_BUFFER)
		{
			CPepeEngineDirect3D9VertexBuffer* buffer = static_cast<CPepeEngineDirect3D9VertexBuffer*>(*it);
			buffer->releaseIfDefaultPool();
		} else if ((*it)->getBufferType() == BT_INDEX_BUFFER)
		{
			CPepeEngineDirect3D9IndexBuffer* buffer = static_cast<CPepeEngineDirect3D9IndexBuffer*>(*it);
			buffer->releaseIfDefaultPool();
		}

		it++;
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9BufferManager::recreateDefaultPoolResources()
{
	BufferList::iterator it = m_bufferList.begin();
	while (it != m_bufferList.end())
	{			
		if ((*it)->getBufferType() == BT_VERTEX_BUFFER)
		{
			CPepeEngineDirect3D9VertexBuffer* buffer = static_cast<CPepeEngineDirect3D9VertexBuffer*>(*it);
			buffer->recreateIfDefaultPool();
		} else if ((*it)->getBufferType() == BT_INDEX_BUFFER)
		{
			CPepeEngineDirect3D9IndexBuffer* buffer = static_cast<CPepeEngineDirect3D9IndexBuffer*>(*it);
			buffer->recreateIfDefaultPool();
		}

		it++;
	}
}

_PEPE_ENGINE_END