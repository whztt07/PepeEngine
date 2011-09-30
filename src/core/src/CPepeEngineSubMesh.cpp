#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineSubMesh.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineSubMesh::CPepeEngineSubMesh(MeshPtr pParentMesh)
{
	if (pParentMesh.isNull())
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_INVALID_PARAMETERS, 
			_T("Creating submesh error: parent mesh doesn't exist."), 
			_T("CPepeEngineSubMesh::CPepeEngineSubMesh")
		);	
	} else
	{
		m_pParentMesh = pParentMesh;		
	}
	
	m_nFaces			= 0;	
	m_pIndexData		= NULL;	

	// submesh always use indexes
	m_renderOperation.m_bUseIndexes		= true;
	m_renderOperation.m_operationType	= CPepeEngineRenderOperation::OT_TRIANGLE_LIST;
	m_renderOperation.m_pVertexData		= m_pParentMesh->getVertexData();
}

// -----------------------------------------------------------------------------------------
CPepeEngineSubMesh::~CPepeEngineSubMesh()
{			
	delete m_pIndexData;		
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSubMesh::setIndexData(CPepeEngineIndexData* pIndexData) 
{
	m_pIndexData					= pIndexData;
	m_renderOperation.m_pIndexData	= pIndexData;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSubMesh::notifyVertexDataChange()
{
	m_renderOperation.m_pVertexData	= m_pParentMesh->getVertexData();
}

_PEPE_ENGINE_END