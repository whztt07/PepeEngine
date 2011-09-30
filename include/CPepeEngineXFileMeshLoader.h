/**
 * Project: PepeEngine
 * Tier:	Frontend
 * File:	CPepeEngineXFileMeshLoader.h   
 *
 * @brief	Declaration of CPepeEngineXFileMeshLoader class.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-06-05
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEXFILEMESHLOADER_H
#define CPEPEENGINEXFILEMESHLOADER_H

#include "PepeEngineInit.h"

#include "IPepeEngineMeshLoader.h"
#include "XFileStructs.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineXFileMeshLoader : public IPepeEngineMeshLoader
{	
private:

	typedef struct tVertex
	{
		float x, y, z;
	} Vertex;

	typedef struct tFace
	{
		unsigned int a, b, c;
	} Face;
	
	typedef struct tFaceNormals
	{
		unsigned int a, b, c;
	} FaceNormals;

	typedef struct tTexCoords
	{
		float u, v;
	} TexCoords;

	typedef struct tMaterial
	{
		float		diffuse[4];
		float		specular[4];
		float		emissive[4];
		float		ambient[4];
		float		fPower;
		tstring		strTextureName;
		tstring		strMaterialName;
	} Material;

	CPepeEngineMatrix4 m_frameMeshTransform;

	typedef struct tMesh
	{
		unsigned short nVertices;
		unsigned short nFaces;
		unsigned short nNormals;
		unsigned short nTexCoords;
		unsigned short nMaterials;

		/** @{ The nFirstxxx variables are used for mesh concatenation */
		unsigned int nFirstVertex;
		unsigned int nFirstFace;            
		unsigned int nFirstTextureCoord;
		unsigned int nFirstNormal;
		unsigned int nFirstMaterial;
		/** @} */

		Face*						pFaces;
		FaceNormals*				pFaceNormals;
		Vertex*						pVertices;
		TexCoords*					pTexCoords;
		CPepeEngineVector3*			pNormals;
		unsigned short int*			pFaceMaterials;	
		tstring						strName;

		tMesh() 
		{
			nVertices			= 0;
			nFaces				= 0;
			nNormals			= 0;
			nTexCoords			= 0;
			nMaterials			= 0;
			nFirstVertex		= 0;
			nFirstFace			= 0;            
			nFirstTextureCoord	= 0;
			nFirstNormal		= 0;
			nFirstMaterial		= 0;

			pFaces				= NULL;
			pFaceNormals		= NULL;
			pVertices			= NULL;
			pTexCoords			= NULL;
			pNormals			= NULL;
			pFaceMaterials		= NULL;			
		}
	} Mesh;

	std::vector<Mesh*>		m_meshes;	
	std::vector<Material*>	m_materials;
	Mesh*					m_pLoadedMesh;

	void concatenateMeshes(MeshPtr pMesh);

	short int processBlock();
	short int blockID(const std::string& strText);
	void avoidTemplate();
	void processBone();
	void processMesh();
	void processMeshNormals();
	void processMeshTextureCoords(); 
	void processMeshMaterials();
	void processMaterial();
	void processFrameTransformMatrix();
public:
	CPepeEngineXFileMeshLoader();
	~CPepeEngineXFileMeshLoader();

	void load(const tstring& strFileName, MeshPtr pMesh);

	bool isAvailableFileExtension(const tstring& strFileName) const;

	void reset();
};

_PEPE_ENGINE_END

#endif