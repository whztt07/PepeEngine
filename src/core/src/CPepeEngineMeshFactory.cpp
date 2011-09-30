#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineImageManager.h"
#include "CPepeEngineMaterialManager.h"
#include "CPepeEngineMeshFactory.h"
#include "CPepeEngineMeshManager.h"
#include "CPepeEnginePlane.h"
#include "IPepeEngineBufferManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
MeshPtr CPepeEngineMeshFactory::createPlane(size_t nSize)
{
	static size_t			nPlaneCounter	= 0;
	const tstring			strName			= _T("Plane ") + CPepeEngineConverter::parseString((float)nPlaneCounter++);
	MeshPtr					pMesh			= CPepeEngineMeshManager::getSingleton().create(strName);	
	CPepeEngineVertexData*	vertexData		= new CPepeEngineVertexData();
	pMesh->setVertexData(vertexData);
	CPepeEngineSubMesh* pSubMesh		= pMesh->createSubMesh();
		
	CPepeEngineIndexData* indexData		= new CPepeEngineIndexData();

	// define the vertex format
	vertexData->m_pVertexDeclaration	= IPepeEngineBufferManager::getSingleton().createVertexDeclaration();	

	size_t currOffset = 0;
	
	vertexData->m_pVertexDeclaration->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
	currOffset += CPepeEngineVertexElement::getTypeSize(VET_FLOAT3);

	vertexData->m_pVertexDeclaration->addElement(0, currOffset, VET_FLOAT3, VES_NORMAL);
	currOffset += CPepeEngineVertexElement::getTypeSize(VET_FLOAT3);
	
	vertexData->m_pVertexDeclaration->addElement(0, currOffset, VET_FLOAT2, VES_TEXTURE_COORDINATES);
	currOffset += CPepeEngineVertexElement::getTypeSize(VET_FLOAT2);
		
	float fHalfSize = nSize / 2.0f;
	float vertices[32] = {
		-fHalfSize, 0,			-fHalfSize, // pos
		0,			1,			0,			// normal
		0,			1,						// texcoord
		fHalfSize, 	0,			-fHalfSize,
		0,			1,			0,
		1,			1,
		fHalfSize,  0,			fHalfSize,
		0,			1,			0,
		1,			0,
		-fHalfSize, 0,			fHalfSize,
		0,			1,			0,
		0,			0 
	};

	// allocate the vertex buffer
	vertexData->m_nVertexCount		= 4;
	vertexData->m_pVertexBuffer		= IPepeEngineBufferManager::getSingleton().createVertexBuffer(	
		vertexData->m_nVertexCount, vertexData->m_pVertexDeclaration->getVertexSize(0), 	
		IPepeEngineBuffer::BU_STATIC_WRITE_ONLY, 
		vertexData->m_pVertexDeclaration->getVertexSize(0) * vertexData->m_nVertexCount,
		false
	);
	
	vertexData->m_pVertexBuffer->writeData(
		0, vertexData->m_pVertexBuffer->getBufferSize(), vertices, true
	);

	// allocate index buffer
	indexData->m_nIndexCount	= 6;
	indexData->m_pIndexBuffer	= IPepeEngineBufferManager::getSingleton().createIndexBuffer(
		2 * indexData->m_nIndexCount,
		IPepeEngineBuffer::BU_STATIC_WRITE_ONLY,
		IPepeEngineIndexBuffer::IT_16BIT, 		
		false
	);
	
	unsigned short* pIndices = static_cast<unsigned short*>(indexData->m_pIndexBuffer->lock(
		0, 2 * indexData->m_nIndexCount, IPepeEngineBuffer::BL_DISCARD)
	);
	unsigned short faces[6] = {0, 1, 2, 0, 2, 3};
	indexData->m_pIndexBuffer->writeData(0, indexData->m_pIndexBuffer->getBufferSize(), faces, true);

	pSubMesh->setIndexData(indexData);
	

	// the original code was missing this line:
	
	pMesh->setAABB(CPepeEngineAxisAlignedBox(-(float)fHalfSize, 0.0f, -(float)fHalfSize, (float)fHalfSize, 0.0f, (float)fHalfSize));

	pMesh->setLoaded(true);
	MaterialPtr pMaterial = CPepeEngineMaterialManager::getSingleton().create(strName);	
	pSubMesh->setMaterialName(strName);
	
	return pMesh;
}

// -----------------------------------------------------------------------------------------
MeshPtr CPepeEngineMeshFactory::createSphere(size_t nSize)
{	
	const int			NUM_SEGMENTS	= 16;
	const int			NUM_RINGS		= 16;	
	static size_t		nSphereCounter	= 0;
	const tstring		strName			= _T("Sphere ") + CPepeEngineConverter::parseString((float)nSphereCounter++);
	MeshPtr				pMesh			= CPepeEngineMeshManager::getSingleton().create(strName);	
	CPepeEngineVertexData* vertexData	= new CPepeEngineVertexData();
	pMesh->setVertexData(vertexData);
	CPepeEngineSubMesh* pSubMesh		= pMesh->createSubMesh();
		
	CPepeEngineIndexData* indexData		= new CPepeEngineIndexData();

	// define the vertex format
	vertexData->m_pVertexDeclaration	= IPepeEngineBufferManager::getSingleton().createVertexDeclaration();	

	size_t currOffset = 0;
	
	vertexData->m_pVertexDeclaration->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
	currOffset += CPepeEngineVertexElement::getTypeSize(VET_FLOAT3);

	vertexData->m_pVertexDeclaration->addElement(0, currOffset, VET_FLOAT3, VES_NORMAL);
	currOffset += CPepeEngineVertexElement::getTypeSize(VET_FLOAT3);
	
	vertexData->m_pVertexDeclaration->addElement(0, currOffset, VET_FLOAT2, VES_TEXTURE_COORDINATES);
	currOffset += CPepeEngineVertexElement::getTypeSize(VET_FLOAT2);
			
	// allocate the vertex buffer
	vertexData->m_nVertexCount		= (NUM_RINGS + 1) * (NUM_SEGMENTS + 1);
	vertexData->m_pVertexBuffer		= IPepeEngineBufferManager::getSingleton().createVertexBuffer(	
		vertexData->m_nVertexCount,
		vertexData->m_pVertexDeclaration->getVertexSize(0), 	
		IPepeEngineBuffer::BU_STATIC_WRITE_ONLY, 
		vertexData->m_pVertexDeclaration->getVertexSize(0) * vertexData->m_nVertexCount,
		false
	);
	
	float* pVertex = static_cast<float*>(vertexData->m_pVertexBuffer->lock(
		0, 
		vertexData->m_pVertexDeclaration->getVertexSize(0) * vertexData->m_nVertexCount,
		IPepeEngineBuffer::BL_DISCARD)
	);

	// allocate index buffer
	indexData->m_nIndexCount	= 6 * NUM_RINGS * (NUM_SEGMENTS + 1);
	indexData->m_pIndexBuffer	= IPepeEngineBufferManager::getSingleton().createIndexBuffer(
		2 * indexData->m_nIndexCount,
		IPepeEngineBuffer::BU_STATIC_WRITE_ONLY,
		IPepeEngineIndexBuffer::IT_16BIT, 		
		false
	);
	
	unsigned short* pIndices = static_cast<unsigned short*>(indexData->m_pIndexBuffer->lock(
		0, 
		2 * indexData->m_nIndexCount,
		IPepeEngineBuffer::BL_DISCARD));

	float fDeltaRingAngle			= (CPepeEngineMath::PI / NUM_RINGS);
	float fDeltaSegAngle			= (2 * CPepeEngineMath::PI / NUM_SEGMENTS);
	unsigned short wVerticeIndex	= 0;

	// Generate the group of rings for the sphere
	for (int ring = 0; ring <= NUM_RINGS; ring++) 
	{
		float r0 = nSize * sinf(ring * fDeltaRingAngle);
		float y0 = nSize * cosf(ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for (int seg = 0; seg <= NUM_SEGMENTS; seg++)
		{
			float x0 = r0 * sinf(seg * fDeltaSegAngle);
			float z0 = r0 * cosf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			*pVertex++					= x0;
			*pVertex++					= y0;
			*pVertex++					= z0;
			CPepeEngineVector3 vNormal	= CPepeEngineVector3(x0, y0, z0).normalisedCopy();
			*pVertex++					= vNormal.x;
			*pVertex++					= vNormal.y;
			*pVertex++					= vNormal.z;			
			*pVertex++					= (float) seg / (float) NUM_SEGMENTS;
			*pVertex++					= (float) ring / (float) NUM_RINGS;

			if (ring != NUM_RINGS) 
			{
				// each vertex (except the last) has six indicies pointing to it
				*pIndices++ = wVerticeIndex + NUM_SEGMENTS + 1;
				*pIndices++ = wVerticeIndex;               
				*pIndices++ = wVerticeIndex + NUM_SEGMENTS;
				*pIndices++ = wVerticeIndex + NUM_SEGMENTS + 1;
				*pIndices++ = wVerticeIndex + 1;
				*pIndices++ = wVerticeIndex;
				wVerticeIndex++;
			}
		}; // end for seg
	} // end for ring

	// Unlock
	indexData->m_pIndexBuffer->unlock();
	vertexData->m_pVertexBuffer->unlock();
		
	pSubMesh->setIndexData(indexData);	
	
	pMesh->setAABB(CPepeEngineAxisAlignedBox(
		CPepeEngineVector3(-(float)nSize, -(float)nSize, -(float)nSize),
		CPepeEngineVector3((float)nSize, (float)nSize, (float)nSize))
	);	

	pMesh->setLoaded(true);

	MaterialPtr pMaterial = CPepeEngineMaterialManager::getSingleton().create(strName);
	pSubMesh->setMaterialName(strName);
	
	return pMesh;
}

_PEPE_ENGINE_END