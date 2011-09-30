/**
 * Project: PepeEngine
 * Tier:	Frontend
 * File:	CPepeEngineMesh.h 
 *
 * @brief	Declaration of CPepeEngineMesh class.  
 *			
 * @author		Piotr 'pepe' Picheta
 * @date		2008-03-19
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEMESH_H
#define CPEPEENGINEMESH_H

#include "PepeEngineInit.h"

#include "CPepeEngineAxisAlignedBox.h"
#include "IPepeEngineResource.h"

_PEPE_ENGINE_START

typedef	std::vector<CPepeEngineSubMesh*> SubMeshList;

class _PepeEngineExport CPepeEngineMesh : public IPepeEngineResource
{
	friend								CPepeEngineEntity;		

	SubMeshList							m_subMeshList;				
	CPepeEngineVertexData*				m_pVertexData;
	mutable CPepeEngineAxisAlignedBox	m_AABB;
	mutable bool						m_bNeedAABBCalculated;	
public:				

	unsigned int m_nVertices;
	unsigned int m_nFaces;
	unsigned int m_nNormals;
	unsigned int m_nTexCoords;
	unsigned int m_nMaterials;
	
	CPepeEngineMesh(const tstring& strName);
	~CPepeEngineMesh();

	void load();
	void loadFromFile(const tstring& strName);

	void unload();

	CPepeEngineSubMesh* createSubMesh();
	SubMeshList	getSubMeshes() const {return m_subMeshList;}

	void calculateBoundingBox() const;

	CPepeEngineAxisAlignedBox getAABB() const {calculateBoundingBox(); return m_AABB;}
	void setAABB(const CPepeEngineAxisAlignedBox& aabb) {m_AABB = aabb;}

	void setVertexData(CPepeEngineVertexData* pVertexData);
	CPepeEngineVertexData* getVertexData() const;	
};

struct _PepeEngineExport MeshPtr : public CPepeEngineSharedPtr<CPepeEngineMesh>
{
public:
	inline			MeshPtr();
	inline explicit	MeshPtr(CPepeEngineMesh* r);
	inline			MeshPtr(const MeshPtr& r);
	inline			MeshPtr(const ResourcePtr& r); 
	inline MeshPtr&	operator=(const ResourcePtr& r);
};

_PEPE_ENGINE_END

#endif