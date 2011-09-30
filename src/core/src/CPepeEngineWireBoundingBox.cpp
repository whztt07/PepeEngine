#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineWireBoundingBox.h"
#include "IPepeEngineBufferManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineWireBoundingBox::CPepeEngineWireBoundingBox()
{
	m_renderOperation.m_pVertexData = new CPepeEngineVertexData();

	m_renderOperation.m_pIndexData							= 0;
	m_renderOperation.m_pVertexData->m_nVertexCount			= 24; 
	m_renderOperation.m_pVertexData->m_nVertexStart			= 0; 
	m_renderOperation.m_operationType						= CPepeEngineRenderOperation::OT_LINE_LIST; 
	m_renderOperation.m_bUseIndexes							= false; 
	m_renderOperation.m_pVertexData->m_pVertexDeclaration	= IPepeEngineBufferManager::getSingleton().createVertexDeclaration();

	m_renderOperation.m_pVertexData->m_pVertexDeclaration->addElement(
		0, 0, VET_FLOAT3, VES_POSITION);

	m_renderOperation.m_pVertexData->m_pVertexBuffer = IPepeEngineBufferManager::getSingleton().createVertexBuffer(
		m_renderOperation.m_pVertexData->m_nVertexCount,
		m_renderOperation.m_pVertexData->m_pVertexDeclaration->getVertexSize(0),
		IPepeEngineBuffer::BU_STATIC_WRITE_ONLY,
		m_renderOperation.m_pVertexData->m_pVertexDeclaration->getVertexSize(0) * 
		m_renderOperation.m_pVertexData->m_nVertexCount,
		false
	);
}

// -----------------------------------------------------------------------------------------
CPepeEngineWireBoundingBox::CPepeEngineWireBoundingBox(const CPepeEngineAxisAlignedBox& aabb)
{
	CPepeEngineWireBoundingBox();

	loadDataFromAABB(aabb);
}

// -----------------------------------------------------------------------------------------
CPepeEngineWireBoundingBox::~CPepeEngineWireBoundingBox()
{	
	delete m_renderOperation.m_pVertexData;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWireBoundingBox::loadDataFromAABB(const CPepeEngineAxisAlignedBox& aabb)
{
	CPepeEngineVector3 vmax = aabb.getMaximum();
	CPepeEngineVector3 vmin = aabb.getMinimum();
	
	float maxx = vmax.x;
	float maxy = vmax.y;
	float maxz = vmax.z;

	float minx = vmin.x;
	float miny = vmin.y;
	float minz = vmin.z;

	// fill in the Vertex buffer: 12 lines with 2 endpoints each make up a box
	IPepeEngineVertexBuffer* vbuf =	m_renderOperation.m_pVertexData->m_pVertexBuffer;

	float* pPos = static_cast<float*>(vbuf->lock(IPepeEngineBuffer::BL_DISCARD));

	// line 0
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = minz;
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = minz;
	// line 1
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = minz;
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = maxz;
	// line 2
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = minz;
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = minz;
	// line 3
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = minz;
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	// line 4
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = minz;
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = minz;
	// line 5
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = minz;
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = maxz;
	// line 6
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = minz;
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = minz;
	// line 7
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	// line 8
	*pPos++ = minx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = maxz;
	// line 9
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = minz;
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	// line 10
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = maxz;
	*pPos++ = maxx;
	*pPos++ = maxy;
	*pPos++ = maxz;
	// line 11
	*pPos++ = minx;
	*pPos++ = miny;
	*pPos++ = maxz;
	*pPos++ = maxx;
	*pPos++ = miny;
	*pPos++ = maxz;
	vbuf->unlock();
}

_PEPE_ENGINE_END