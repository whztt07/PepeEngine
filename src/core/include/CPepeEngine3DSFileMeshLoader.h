/**
 * Project:		PepeEngine
 * Tier:		Frontend
 * File:		CPepeEngine3DSFileMeshLoader.h   
 *
 * @brief		Declaration of CPepeEngine3DSFileMeshLoader class.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-10-07
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef CPEPEENGINE3DSFILEMESHLOADER_H
#define CPEPEENGINE3DSFILEMESHLOADER_H

#include "PepeEngineInit.h"

#include "IPepeEngineMeshLoader.h"

_PEPE_ENGINE_START

struct ChunkData;
struct Material;
struct Mesh;
struct SMaterialGroup;

class _PepeEngineExport CPepeEngine3DSFileMeshLoader : public IPepeEngineMeshLoader
{	
public:
									CPepeEngine3DSFileMeshLoader();
									~CPepeEngine3DSFileMeshLoader();

	void							load(const tstring& strFileName, MeshPtr pMesh);
	bool							isAvailableFileExtension(const tstring& strFileName) const;
	void							reset();
private:
	

	std::vector<Mesh*>				m_meshes;	
	std::vector<SMaterialGroup>		m_materialGroups;	
	std::vector<Material*>			m_materials;
	Mesh*							m_pLoadedMesh;
	
	void							readChunkData(ChunkData& chunk);
	bool							readChunk(ChunkData* parent);
	bool							readMaterialChunk(ChunkData* parent);
	bool							readColorChunk(ChunkData* chunk, float* out);
	bool							readPercentageChunk(ChunkData* chunk, float& percentage);
	void							readString(ChunkData& data, tstring& out);
	bool							readObjectChunk(ChunkData* parent);
	void							readVertices(ChunkData& data);
	void							readIndices(ChunkData& data);
	void							readMaterialGroup(ChunkData& data);	
	void							readTextureCoords(ChunkData& data);

	void							concatenateMeshes(MeshPtr pMesh);
};

_PEPE_ENGINE_END

#endif