#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineMaterialManager.h"
#include "CPepeEngineMeshManager.h"
#include "CPepeEngineSkyBox.h"
#include "IPepeEngineBufferManager.h"
#include "IPepeEngineGPUProgramManager.h"
#include "IPepeEngineTextureManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineSkyBox::CPepeEngineSkyBox(const tstring& strCubeTexture, int nSize) : CPepeEngineEntity(_T("SkyBox"))
{
	if (CPepeEngineCore::getSingleton().getRenderer()->getRendererCapabilities()->hasCapability(RC_CUBEMAPPING))
	{
		m_pMesh = 0;
		m_nSize = nSize;
		createSphereMesh();

		MaterialPtr pMaterial = CPepeEngineMaterialManager::getSingleton().create(
			_T("SkyBox")
		);

		m_pSkyBoxTexture = IPepeEngineTextureManager::getSingleton().create(
			strCubeTexture
		);	
		m_pSkyBoxTexture->setTextureType(TEX_TYPE_CUBE_MAP);
		m_pSkyBoxTexture->load();
		pMaterial->setTexture(strCubeTexture);	
		pMaterial->getTextureUnitState().setTextureAddressingMode(TAM_CLAMP);
		pMaterial->getTextureUnitState().setTextureFilterOptions(TFO_TRILINEAR);
		pMaterial->setLightingEnabled(false);
		pMaterial->setSpecularEnabled(false);
		pMaterial->setCullingMode(CULL_NONE);		

		CPepeEngineSubMesh* pSubMesh = m_pMesh->getSubMeshes()[0];
		pSubMesh->setMaterialName(_T("SkyBox"));		
			
		addSubEntity(pSubMesh);		
		makeTransforms();
	} else
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("This device doesn't support cube mapping."), 
			_T("CPepeEngineSkyBox::CPepeEngineSkyBox")
		);	
	}
}

// -----------------------------------------------------------------------------------------
CPepeEngineSkyBox::~CPepeEngineSkyBox()
{	
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSkyBox::renderFromCamera()
{
	setTranslation(CPepeEngineCore::getSingleton().getSceneManager()->getCamera()->getPosition());
	CPepeEngineCore::getSingleton().getRenderer()->setWorldMatrix(getWorldTransforms());
	render();
}

// -----------------------------------------------------------------------------------------
TexturePtr CPepeEngineSkyBox::getTexture() const
{
	return m_pSkyBoxTexture;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSkyBox::createSphereMesh()
{	
	m_pMesh								= CPepeEngineMeshManager::getSingleton().create(_T("Sky Box Sphere"));	
	CPepeEngineVertexData* vertexData	= new CPepeEngineVertexData();
	m_pMesh->setVertexData(vertexData);
	CPepeEngineSubMesh* pSubMesh		= m_pMesh->createSubMesh();

	const int	NUM_SEGMENTS	= 64;
	const int	NUM_RINGS		= 64;	
	

	CPepeEngineIndexData* indexData		= new CPepeEngineIndexData();

	// define the vertex format
	vertexData->m_pVertexDeclaration	= IPepeEngineBufferManager::getSingleton().createVertexDeclaration();	

	size_t currOffset = 0;
	
	vertexData->m_pVertexDeclaration->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
	currOffset += CPepeEngineVertexElement::getTypeSize(VET_FLOAT3);
	
	vertexData->m_pVertexDeclaration->addElement(0, currOffset, VET_FLOAT3, VES_TEXTURE_COORDINATES);
	currOffset += CPepeEngineVertexElement::getTypeSize(VET_FLOAT3);
		
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
		float r0 = m_nSize * sinf(ring * fDeltaRingAngle);
		float y0 = m_nSize * cosf(ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for (int seg = 0; seg <= NUM_SEGMENTS; seg++)
		{
			float x0 = r0 * sinf(seg * fDeltaSegAngle);
			float z0 = r0 * cosf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			*pVertex++ = x0;
			*pVertex++ = y0;
			*pVertex++ = z0;

			*pVertex++ = x0;
			*pVertex++ = y0;
			*pVertex++ = z0;

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
	
	m_pMesh->setAABB(CPepeEngineAxisAlignedBox(
		CPepeEngineVector3((float)-m_nSize, (float)-m_nSize, (float)-m_nSize),
		CPepeEngineVector3((float)m_nSize, (float)m_nSize, (float)m_nSize))
	);	

	m_pMesh->setLoaded(true);
}

_PEPE_ENGINE_END