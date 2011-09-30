#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngine3DSFileMeshLoader.h"
#include "CPepeEngineColor.h"
#include "CPepeEngineMaterial.h"
#include "CPepeEngineMaterialManager.h"
#include "CPepeEngineMesh.h"
#include "CPepeEngineSubMesh.h"
#include "IPepeEngineBufferManager.h"
#include "IPepeEngineTextureManager.h"

#include "3DSFileStructs.cpp"

_PEPE_ENGINE_START
	
// -----------------------------------------------------------------------------------------
CPepeEngine3DSFileMeshLoader::CPepeEngine3DSFileMeshLoader()
{
	fin.clear();
	m_meshes.clear();	
	m_materials.clear();
	m_materialGroups.clear();
	m_pLoadedMesh = NULL;
}

// -----------------------------------------------------------------------------------------
CPepeEngine3DSFileMeshLoader::~CPepeEngine3DSFileMeshLoader()
{
	reset();
}

// -----------------------------------------------------------------------------------------
void CPepeEngine3DSFileMeshLoader::load(const tstring& strFileName, MeshPtr pMesh)
{
	fin.clear();
	fin.open(strFileName.c_str(), std::ios::binary);

	if (pMesh == NULL)
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_INVALID_PARAMETERS, 
			_T("Mesh: null pointer"), 
			_T("CPepeEngine3DSFileMeshLoader::load")
		);
	}

	if (fin.bad())
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_FILE_LOAD_ERROR, 
			_T("File load error"), 
			_T("CPepeEngine3DSFileMeshLoader::load")
		);
	}

	ChunkData data;

	readChunkData(data);

	if (data.header.id != C3DS_MAIN3DS)
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_FILE_LOAD_ERROR, 
			_T("File load error - bad file header"), 
			_T("CPepeEngine3DSFileMeshLoader::load")
		);
	}

	if (readChunk(&data))
	{
		concatenateMeshes(pMesh);
	}

}

// -----------------------------------------------------------------------------------------
bool CPepeEngine3DSFileMeshLoader::isAvailableFileExtension(const tstring& strFileName) const
{
	if (strFileName.rfind(_T(".3ds")) != -1)
	{
		return true;
	} else
	{
		return false;
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngine3DSFileMeshLoader::reset()
{
	for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); it++)
	{
		if (*it != NULL)
		{
			delete[] (*it)->pFaces;			
			delete[] (*it)->pVertices;			
			delete[] (*it)->pTexCoords;			
			delete[] (*it)->pSmoothingGroups;			

			delete *it;
		}
	}

	for (std::vector<Material*>::iterator itMaterial = m_materials.begin(); itMaterial != m_materials.end(); itMaterial++)
	{
		delete *itMaterial;
	}	
	for (
		std::vector<SMaterialGroup>::iterator itMaterialG = m_materialGroups.begin();
		itMaterialG != m_materialGroups.end(); itMaterialG++)
	{
		itMaterialG->clear();
	}	

	m_meshes.clear();
	m_materials.clear();
	m_materialGroups.clear();

	fin.close();
}

// -----------------------------------------------------------------------------------------
void CPepeEngine3DSFileMeshLoader::readChunkData(ChunkData& chunk)
{
	fin.read((char* )&(chunk.header), sizeof(ChunkHeader));
	chunk.read += sizeof(ChunkHeader);
}

// -----------------------------------------------------------------------------------------
bool CPepeEngine3DSFileMeshLoader::readChunk(ChunkData* parent)
{
	while (parent->read < parent->header.length)
	{
		ChunkData data;
		readChunkData(data);

		switch(data.header.id)
		{
		case C3DS_VERSION:
			{
				unsigned short version;
				fin.read((char *)&version, sizeof(unsigned short));

				fin.seekg(data.header.length - data.read - 2, ios::cur);

				data.read += data.header.length - data.read;
				
				if (version != 0x03)
				{
					PEPE_ENGINE_EXCEPT(
						CPepeEngineException::ERR_FILE_LOAD_ERROR, 
						_T("3ds file version is other than 3."), 
						_T("CPepeEngine3DSFileMeshLoader::readChunk")
					);					
				}
			}
			break;
		case C3DS_EDIT_MATERIAL:
			readMaterialChunk(&data);
			break;
	/*	case C3DS_KEYF3DS:
			//readFrameChunk(&data);
			break;*/
		case C3DS_EDIT3DS:
			break;
		case C3DS_MESHVERSION:
		case 0x01:
			{
				unsigned int version;
				fin.read((char *)&version, sizeof(unsigned int));

				data.read += sizeof(unsigned int);
			}
			break;
		case C3DS_EDIT_OBJECT:
			{
				tstring strName;
				readString(data, strName);

				// ---------- loaded mesh initialization ----------
				m_pLoadedMesh = new Mesh;
				m_pLoadedMesh->nFaces				= 0;				
				m_pLoadedMesh->nTexCoords			= 0;
				m_pLoadedMesh->nVertices			= 0;				
				m_pLoadedMesh->nFirstVertex			= 0;	
				m_pLoadedMesh->nFirstFace			= 0;            
				m_pLoadedMesh->nFirstTextureCoord	= 0;				
				m_pLoadedMesh->pFaces				= NULL;				
				m_pLoadedMesh->pTexCoords			= NULL;
				m_pLoadedMesh->pVertices			= NULL;				

				if (!m_meshes.empty())
				{
					Mesh *pLastMesh = m_meshes.back();

					m_pLoadedMesh->nFirstVertex			= pLastMesh->nFirstVertex + pLastMesh->nVertices;
					m_pLoadedMesh->nFirstFace			= pLastMesh->nFirstFace + pLastMesh->nFaces;					
					m_pLoadedMesh->nFirstTextureCoord	= pLastMesh->nFirstTextureCoord + pLastMesh->nTexCoords;
					if (m_pLoadedMesh->nFirstTextureCoord < m_pLoadedMesh->nFirstVertex)
					{
						m_pLoadedMesh->nFirstTextureCoord = m_pLoadedMesh->nFirstVertex;
					}					
				}

				//end ---------- loaded mesh initialization -------

				readObjectChunk(&data);				
				m_meshes.push_back(m_pLoadedMesh);
			}
			break;

		default:
			// ignore chunk
			fin.seekg(data.header.length - data.read, ios::cur);
			data.read += data.header.length - data.read;
		}

		parent->read += data.read;
	}

	return true;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngine3DSFileMeshLoader::readMaterialChunk(ChunkData* parent)
{
	unsigned short matSection = 0;
	
	Material* newMaterial = new Material;

	while (parent->read < parent->header.length)
	{
		ChunkData data;
		readChunkData(data);

		switch(data.header.id)
		{
		case C3DS_MATNAME:
			{
				char* c = new char[data.header.length - data.read];
				fin.read(c, data.header.length - data.read);

				if (strlen(c))
				{
					newMaterial->strMaterialName = _TSTR(c);
				}

				data.read += data.header.length - data.read;
				delete[] c;
			}
			break;
		case C3DS_MATAMBIENT:
			readColorChunk(&data, newMaterial->ambient);
			break;
		case C3DS_MATDIFFUSE:
			readColorChunk(&data, newMaterial->diffuse);
			break;
		case C3DS_MATSPECULAR:
			readColorChunk(&data, newMaterial->specular);
			break;
		case C3DS_MATSHININESS:
			readPercentageChunk(&data, newMaterial->fPower);
			break;
			/*
		case C3DS_TRANSPARENCY:
			{
				float percentage;
				readPercentageChunk(&data, percentage);
				if (percentage > 0.0f)
				{
					CurrentMaterial.Material.MaterialTypeParam=percentage;
					CurrentMaterial.Material.MaterialType=video::EMT_TRANSPARENT_VERTEX_ALPHA;
				}
				else
				{
					CurrentMaterial.Material.MaterialType=video::EMT_SOLID;
				}
			}
			break;
			
		case C3DS_WIRE:
			CurrentMaterial.Material.Wireframe=true;
			break;
		case C3DS_TWO_SIDE:
			CurrentMaterial.Material.BackfaceCulling=false;
			break;
		case C3DS_SHADING:
			{
				s16 flags;
				file->read(&flags, 2);

				switch (flags)
				{
				case 0:
					CurrentMaterial.Material.Wireframe=true;
					break;
				case 1:
					CurrentMaterial.Material.Wireframe=false;
					CurrentMaterial.Material.GouraudShading=false;
					break;
				case 2:
					CurrentMaterial.Material.Wireframe=false;
					CurrentMaterial.Material.GouraudShading=true;
					break;
				default:
					// phong and metal missing
					break;
				}
				data.read += data.header.length - data.read;
			}
			break;*/
		case C3DS_MATTEXMAP:
		case C3DS_MATSPECMAP:
		case C3DS_MATOPACMAP:
		case C3DS_MATREFLMAP:
		case C3DS_MATBUMPMAP:
			{
				matSection = data.header.id;
			}
			break;
		case C3DS_MATMAPFILE:
			{
				// read texture file name
				char* c = new char[data.header.length - data.read];
				fin.read(c, data.header.length - data.read);
				if (matSection == C3DS_MATTEXMAP)
					newMaterial->strTextureName = _TSTR(c);
				/*
				else if (matSection == C3DS_MATSPECMAP)
					CurrentMaterial.Filename[1] = c;
				else if (matSection == C3DS_MATOPACMAP)
					CurrentMaterial.Filename[2] = c;
				else if (matSection == C3DS_MATREFLMAP)
					CurrentMaterial.Filename[3] = c;
				else if (matSection == C3DS_MATBUMPMAP)
					CurrentMaterial.Filename[4] = c;*/
				data.read += data.header.length - data.read;
				delete[] c;
			}
			break;
		case C3DS_MAT_TEXTILING:
			{
				short flags;
				fin.read((char *)&flags, 2);

				data.read += 2;
			}
			break;
		case C3DS_MAT_USCALE:
		case C3DS_MAT_VSCALE:
		case C3DS_MAT_UOFFSET:
		case C3DS_MAT_VOFFSET:
			{
				float value;
				fin.read((char *)&value, 4);
				data.read += 4;
			}
			break;
		default:
			// ignore chunk
			fin.seekg(data.header.length - data.read, ios::cur);
			data.read += data.header.length - data.read;
		}

		parent->read += data.read;
	}

	m_materials.push_back(newMaterial);
	

	return true;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngine3DSFileMeshLoader::readColorChunk(ChunkData* chunk,	float* out)
{
	ChunkData data;
	readChunkData(data);

	unsigned char	c[3];
	float			cf[3];

	switch(data.header.id)
	{
	case C3DS_COL_TRU:
	case C3DS_COL_LIN_24:
		{
			// read 8 bit data
			fin.read((char *)c, sizeof(c));			
			data.read += sizeof(c);
			out[0] = c[0] / 255.0f;
			out[1] = c[1] / 255.0f;
			out[2] = c[2] / 255.0f;
			out[3] = 1.0f;
		}
		break;
	case C3DS_COL_RGB:
	case C3DS_COL_LIN_F:
		{
			// read float data
			fin.read((char *)cf, sizeof(cf));		
			data.read += sizeof(cf);

			out[0] = cf[0];
			out[1] = cf[1];
			out[2] = cf[2];
			out[3] = 1.0f;
		}
		break;
	default:
		{
			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_FILE_LOAD_ERROR, 
				_T("Unknown size of color chunk in 3DS file."), 
				_T("CPepeEngine3DSFileMeshLoader::readColorChunk")
			);				
		}
	}
	
	chunk->read += data.read;

	return true;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngine3DSFileMeshLoader::readPercentageChunk(ChunkData* chunk, float& percentage)
{
	ChunkData data;
	readChunkData(data);

	short intpercentage;
	float fpercentage;

	switch(data.header.id)
	{
	case C3DS_PERCENTAGE_I:
		{
			// read short
			fin.read((char *)&intpercentage, 2);

			percentage	= intpercentage / 100.0f;
			data.read	+= 2;
		}
		break;
	case C3DS_PERCENTAGE_F:
		{
			// read float
			fin.read((char *)&fpercentage, sizeof(float));
			data.read	+= sizeof(float);
			percentage	= (float)fpercentage;
		}
		break;
	default:
		{
			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_FILE_LOAD_ERROR, 
				_T("Unknown percentage chunk in 3DS file."), 
				_T("CPepeEngine3DSFileMeshLoader::readPercentageChunk")
			);	
		}
	}

	chunk->read += data.read;

	return true;
}

// -----------------------------------------------------------------------------------------
void CPepeEngine3DSFileMeshLoader::readString(ChunkData& data, tstring& out)
{
	char	c = 1;
	string	s;

	while (c)
	{
		fin.read((char *)&c, sizeof(char));
		if (c)
		{
			s.push_back(c);
		}

		++data.read;
	}
	out = _TSTR(s);
}

// -----------------------------------------------------------------------------------------
bool CPepeEngine3DSFileMeshLoader::readObjectChunk(ChunkData* parent)
{
	
	while (parent->read < parent->header.length)
	{
		ChunkData data;
		readChunkData(data);

		switch (data.header.id)
		{
		case C3DS_OBJTRIMESH:
			readObjectChunk(&data);
			break;

		case C3DS_TRIVERT: 
			readVertices(data);
			break;

		case C3DS_POINTFLAGARRAY:
			{
				unsigned short numVertex, flags;
				fin.read((char* )&numVertex, sizeof(unsigned short));

				for (unsigned short i = 0; i < numVertex; ++i)
				{
					fin.read((char *)&flags, sizeof(unsigned short));
				}
				data.read += (numVertex + 1) * sizeof(unsigned short);
			}
			break;

		case C3DS_TRIFACE: 
			readIndices(data);
			readObjectChunk(&data); // read smooth and material groups
			break;

		case C3DS_TRIFACEMAT: 
			readMaterialGroup(data);
			break;

		case C3DS_TRIUV: // getting texture coordinates
			readTextureCoords(data);
			break;
/*
		case C3DS_TRIMATRIX:
			{
				f32 mat[4][3];
				file->read(&mat, 12*sizeof(f32));
				TransformationMatrix.makeIdentity();
				for (int i=0; i<4; ++i)
				{
					for (int j=0; j<3; ++j)
					{
						TransformationMatrix(i,j)=mat[i][j];
					}
				}
				data.read += 12*sizeof(f32);
			}
			break;*/
		case C3DS_MESHCOLOR:
			{
				unsigned char flag;
				fin.read((char *)&flag, sizeof(unsigned char));
				++data.read;
			}
			break;
/*
		case C3DS_TRISMOOTH:
			{
				m_pLoadedMesh->pSmoothingGroups = new unsigned int[m_pLoadedMesh->nFaces];
				fin.read((char *)m_pLoadedMesh->pSmoothingGroups, m_pLoadedMesh->nFaces * sizeof(unsigned int));

				data.read += m_pLoadedMesh->nFaces * sizeof(unsigned int);
			}
			break;*/

		default:
			// ignore chunk
			fin.seekg(data.header.length - data.read, ios::cur);
			data.read += data.header.length - data.read;
		}

		parent->read += data.read;
	}

	return true;
}

// -----------------------------------------------------------------------------------------
void CPepeEngine3DSFileMeshLoader::readVertices(ChunkData& data)
{
	fin.read((char *)&(m_pLoadedMesh->nVertices), sizeof(m_pLoadedMesh->nVertices));

	data.read += sizeof(m_pLoadedMesh->nVertices);

	int vertexBufferByteSize = m_pLoadedMesh->nVertices * sizeof(float) * 3;

	if ( (data.header.length - data.read) != vertexBufferByteSize)
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_FILE_LOAD_ERROR, 
			_T("Invalid size of vertices found in 3ds file."), 
			_T("CPepeEngine3DSFileMeshLoader::readVertices")
		);			
	}

	m_pLoadedMesh->pVertices = new Vertex[m_pLoadedMesh->nVertices];
	fin.read((char *)m_pLoadedMesh->pVertices, vertexBufferByteSize);

	data.read += vertexBufferByteSize;
}

// -----------------------------------------------------------------------------------------
void CPepeEngine3DSFileMeshLoader::readIndices(ChunkData& data)
{
	fin.read((char *)&m_pLoadedMesh->nFaces, sizeof(m_pLoadedMesh->nFaces));

	data.read += sizeof(m_pLoadedMesh->nFaces);

	int indexBufferByteSize = m_pLoadedMesh->nFaces * sizeof(unsigned short) * 4;
	
	unsigned short* buff = new unsigned short[m_pLoadedMesh->nFaces * 4];
	fin.read((char *)buff, indexBufferByteSize);

	m_pLoadedMesh->pFaces = new Face[m_pLoadedMesh->nFaces];
	for (unsigned int i = 0; i < m_pLoadedMesh->nFaces; i++)
	{
		m_pLoadedMesh->pFaces[i].a	= buff[i * 4]; 
		m_pLoadedMesh->pFaces[i].b	= buff[i * 4 + 1];
		m_pLoadedMesh->pFaces[i].c	= buff[i * 4 + 2];
	}
	delete[] buff;
	
	data.read += indexBufferByteSize;
}

// -----------------------------------------------------------------------------------------
void CPepeEngine3DSFileMeshLoader::readMaterialGroup(ChunkData& data)
{		
	SMaterialGroup group;
	readString(data, group.strMaterialName);

	fin.read((char *)&group.faceCount, sizeof(group.faceCount));

	data.read += sizeof(group.faceCount);

	// read faces
	group.pFaces = new unsigned short[group.faceCount];
	fin.read((char *)group.pFaces, sizeof(unsigned short) * group.faceCount);
	data.read += sizeof(unsigned short) * group.faceCount;

	m_materialGroups.push_back(group);
}

// -----------------------------------------------------------------------------------------
void CPepeEngine3DSFileMeshLoader::readTextureCoords(ChunkData& data)
{
	fin.read((char *)&m_pLoadedMesh->nTexCoords, sizeof(m_pLoadedMesh->nTexCoords));

	data.read += sizeof(m_pLoadedMesh->nTexCoords);

	int tcoordsBufferByteSize = m_pLoadedMesh->nTexCoords * sizeof(float) * 2;

	if (data.header.length - data.read != tcoordsBufferByteSize)
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_FILE_LOAD_ERROR, 
			_T("Invalid size of tcoords found in 3ds file."), 
			_T("CPepeEngine3DSFileMeshLoader::readTextureCoords")
		);				
	}

	m_pLoadedMesh->pTexCoords = new TexCoords[m_pLoadedMesh->nTexCoords];
	fin.read((char *)m_pLoadedMesh->pTexCoords, tcoordsBufferByteSize);

	data.read += tcoordsBufferByteSize;
}

// -----------------------------------------------------------------------------------------
void CPepeEngine3DSFileMeshLoader::concatenateMeshes(MeshPtr pMesh)
{
	Mesh *pConcatMesh = NULL;

	if (m_meshes.size() >= 2)
	{
		pConcatMesh		= new Mesh;
		Mesh* pLastMesh	= m_meshes.back();

		/** 
		 * We create the new mesh. We get the dimensions of the new mesh
		 */
		pConcatMesh->nVertices		= pLastMesh->nFirstVertex + pLastMesh->nVertices;
		pConcatMesh->nFaces			= pLastMesh->nFirstFace + pLastMesh->nFaces;
		pConcatMesh->nTexCoords		= pLastMesh->nFirstTextureCoord + pLastMesh->nTexCoords;	

	   /**
	    * Here we have a control:
		* - Texture coordinates must be as numerous as Vertices or there must be none
	    * - Normal vectors must be as numerous as Vertices or there must be none
		*/
		if ((pConcatMesh->nTexCoords < pConcatMesh->nVertices) && (pConcatMesh->nTexCoords != 0))
		{
			pConcatMesh->nTexCoords = pConcatMesh->nVertices;
		}		

		/**
		 * We create all the arrays:
		 * - Vertices and Faces
		 * - Material indices per face
		 * - Texture Coords
		 * - Normal vectors and Face Allocation of Normal vectors
		 */
		pConcatMesh->pVertices = new Vertex[pConcatMesh->nVertices];
		memset(pConcatMesh->pVertices, 0, pConcatMesh->nVertices * sizeof(Vertex));

		pConcatMesh->pFaces = new Face[pConcatMesh->nFaces];
		memset(pConcatMesh->pFaces, 0, pConcatMesh->nFaces * sizeof(Face));

		if (pConcatMesh->nTexCoords != 0)
		{
			pConcatMesh->pTexCoords = new TexCoords[pConcatMesh->nTexCoords];
			memset(pConcatMesh->pTexCoords, 0, pConcatMesh->nTexCoords * sizeof(TexCoords));
		}
		
		/** We fill up the arrays with each array from the m_meshes container */
		for (std::vector<Mesh*>::iterator i = m_meshes.begin(); i != m_meshes.end(); i++)
		{
			// ------------ update indices ----------------------------------
			for (unsigned long int j = 0; j < (*i)->nFaces; j++)
			{
				(*i)->pFaces[j].a += (*i)->nFirstVertex;
				(*i)->pFaces[j].b += (*i)->nFirstVertex;
				(*i)->pFaces[j].c += (*i)->nFirstVertex;
			}
			
			//end ------------ update indices -------------------------------

			memcpy(&(pConcatMesh->pVertices[(*i)->nFirstVertex]), (*i)->pVertices, (*i)->nVertices * sizeof(Vertex));
			memcpy(&(pConcatMesh->pFaces[(*i)->nFirstFace]), (*i)->pFaces, (*i)->nFaces * sizeof(Face));			

			if ((*i)->nTexCoords != 0)
			{
				memcpy(&(pConcatMesh->pTexCoords[(*i)->nFirstTextureCoord]), (*i)->pTexCoords, (*i)->nTexCoords * sizeof(TexCoords));
			}			
		}

		m_meshes.push_back(pConcatMesh);
	} else
	{
		pConcatMesh = m_meshes[0];		
	}

	// ---------------------- calculate normals ---------------------------

	size_t				nNormals		= 0;
	Face				*pFaceNormals	= new Face[pConcatMesh->nFaces];
	CPepeEngineVector3* pNormals		= new CPepeEngineVector3[pConcatMesh->nFaces * 3];

	unsigned int count = pConcatMesh->nFaces * 3;
	for (unsigned int i = 0; i < count; i += 3)
	{
		CPepeEnginePlane plane(
			CPepeEngineVector3(
				pConcatMesh->pVertices[pConcatMesh->pIndices[i]].x, 
				pConcatMesh->pVertices[pConcatMesh->pIndices[i]].y, 
				pConcatMesh->pVertices[pConcatMesh->pIndices[i]].z 
			),

			CPepeEngineVector3(
				pConcatMesh->pVertices[pConcatMesh->pIndices[i + 1]].x, 
				pConcatMesh->pVertices[pConcatMesh->pIndices[i + 1]].y, 
				pConcatMesh->pVertices[pConcatMesh->pIndices[i + 1]].z 
			),

			CPepeEngineVector3(
				pConcatMesh->pVertices[pConcatMesh->pIndices[i + 2]].x, 
				pConcatMesh->pVertices[pConcatMesh->pIndices[i + 2]].y, 
				pConcatMesh->pVertices[pConcatMesh->pIndices[i + 2]].z 
			)
		);
		pNormals[pConcatMesh->pIndices[i + 0]] += plane.normal;
		pNormals[pConcatMesh->pIndices[i + 1]] += plane.normal;
		pNormals[pConcatMesh->pIndices[i + 2]] += plane.normal;
		nNormals++;
	}		

	for (unsigned int i = 0; i < count; i++)
	{	
		pNormals[i].normalise();		
	}		

	//end ------------------- calculate normals ---------------------------


	// ---------------------- move data to pepe engine mesh ---------------	
	CPepeEngineVertexData* pVertexData		= new CPepeEngineVertexData();	
	pVertexData->m_nVertexCount				= pConcatMesh->nVertices;
	pVertexData->m_nVertexStart				= 0;	
	pVertexData->m_pVertexDeclaration		= IPepeEngineBufferManager::getSingleton().createVertexDeclaration();

	size_t			offset	= 0;
	unsigned int	vSize	= 0;	
	if (pConcatMesh->nVertices > 0)
	{
		pVertexData->m_pVertexDeclaration->addElement(0, offset, VET_FLOAT3, VES_POSITION);
		offset	+= CPepeEngineVertexElement::getTypeSize(VET_FLOAT3);
		vSize	+= 3;
	}

	if (nNormals > 0)
	{
		pVertexData->m_pVertexDeclaration->addElement(0, offset, VET_FLOAT3, VES_NORMAL);
		offset	+= CPepeEngineVertexElement::getTypeSize(VET_FLOAT3);
		vSize	+= 3;
	}

	if (pConcatMesh->nTexCoords > 0)
	{
		pVertexData->m_pVertexDeclaration->addElement(0, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES);
		vSize += 2;
	}
	
	float* vertexData	= new float[pConcatMesh->nVertices * vSize];

	for (unsigned int i = 0; i < pConcatMesh->nFaces; i++)
	{
		offset = 0;
		if (pConcatMesh->nVertices > 0)
		{
			vertexData[pConcatMesh->pFaces[i].a * vSize + offset]		= pConcatMesh->pVertices[pConcatMesh->pFaces[i].a].x;
			vertexData[pConcatMesh->pFaces[i].a * vSize + offset + 1]	= pConcatMesh->pVertices[pConcatMesh->pFaces[i].a].y;
			vertexData[pConcatMesh->pFaces[i].a * vSize + offset + 2]	= pConcatMesh->pVertices[pConcatMesh->pFaces[i].a].z;

			vertexData[pConcatMesh->pFaces[i].b * vSize + offset]		= pConcatMesh->pVertices[pConcatMesh->pFaces[i].b].x;
			vertexData[pConcatMesh->pFaces[i].b * vSize + offset + 1]	= pConcatMesh->pVertices[pConcatMesh->pFaces[i].b].y;
			vertexData[pConcatMesh->pFaces[i].b * vSize + offset + 2]	= pConcatMesh->pVertices[pConcatMesh->pFaces[i].b].z;

			vertexData[pConcatMesh->pFaces[i].c * vSize + offset]		= pConcatMesh->pVertices[pConcatMesh->pFaces[i].c].x;
			vertexData[pConcatMesh->pFaces[i].c * vSize + offset + 1]	= pConcatMesh->pVertices[pConcatMesh->pFaces[i].c].y;
			vertexData[pConcatMesh->pFaces[i].c * vSize + offset + 2]	= pConcatMesh->pVertices[pConcatMesh->pFaces[i].c].z;
			offset += 3;
		}

		if (nNormals > 0)
		{
			vertexData[pConcatMesh->pFaces[i].a * vSize + offset]		= pNormals[pConcatMesh->pFaces[i].a].x;
			vertexData[pConcatMesh->pFaces[i].a * vSize + offset + 1]	= pNormals[pConcatMesh->pFaces[i].a].y;
			vertexData[pConcatMesh->pFaces[i].a * vSize + offset + 2]	= pNormals[pConcatMesh->pFaces[i].a].z;

			vertexData[pConcatMesh->pFaces[i].b * vSize + offset]		= pNormals[pConcatMesh->pFaces[i].b].x;
			vertexData[pConcatMesh->pFaces[i].b * vSize + offset + 1]	= pNormals[pConcatMesh->pFaces[i].b].y;
			vertexData[pConcatMesh->pFaces[i].b * vSize + offset + 2]	= pNormals[pConcatMesh->pFaces[i].b].z;

			vertexData[pConcatMesh->pFaces[i].c * vSize + offset]		= pNormals[pConcatMesh->pFaces[i].c].x;
			vertexData[pConcatMesh->pFaces[i].c * vSize + offset + 1]	= pNormals[pConcatMesh->pFaces[i].c].y;
			vertexData[pConcatMesh->pFaces[i].c * vSize + offset + 2]	= pNormals[pConcatMesh->pFaces[i].c].z;
			offset += 3;
		}
		
		
		if (pConcatMesh->nTexCoords > 0)
		{
			vertexData[pConcatMesh->pFaces[i].a * vSize + offset]		= pConcatMesh->pTexCoords[pConcatMesh->pFaces[i].a].u;
			vertexData[pConcatMesh->pFaces[i].a * vSize + offset + 1]	= pConcatMesh->pTexCoords[pConcatMesh->pFaces[i].a].v;			

			vertexData[pConcatMesh->pFaces[i].b * vSize + offset]		= pConcatMesh->pTexCoords[pConcatMesh->pFaces[i].b].u;
			vertexData[pConcatMesh->pFaces[i].b * vSize + offset + 1]	= pConcatMesh->pTexCoords[pConcatMesh->pFaces[i].b].v;

			vertexData[pConcatMesh->pFaces[i].c * vSize + offset]		= pConcatMesh->pTexCoords[pConcatMesh->pFaces[i].c].u;
			vertexData[pConcatMesh->pFaces[i].c * vSize + offset + 1]	= pConcatMesh->pTexCoords[pConcatMesh->pFaces[i].c].v;			
		}
	}
	delete[] pFaceNormals;
	delete[] pNormals;

	pVertexData->m_pVertexBuffer = IPepeEngineBufferManager::getSingleton().createVertexBuffer(
		pConcatMesh->nVertices, vSize * sizeof(float), IPepeEngineBuffer::BU_STATIC_WRITE_ONLY, pConcatMesh->nVertices * vSize * sizeof(float), false
	);
	pVertexData->m_pVertexBuffer->writeData(0, vSize * pConcatMesh->nVertices * sizeof(float), vertexData, false);

	pMesh->m_nFaces		= pConcatMesh->nFaces;	
	pMesh->m_nTexCoords	= pConcatMesh->nTexCoords;
	pMesh->m_nVertices	= pConcatMesh->nVertices;
	pMesh->m_nMaterials	= m_materials.size();

	pMesh->setVertexData(pVertexData);
	delete vertexData;
	//end ---------------------- move data to pepe engine mesh ------------

	// ----------------------- create sub meshes --------------------------	
	CPepeEngineSubMesh* pSubMesh	= NULL;

	//For each material
	for (unsigned int i = 0; i < m_materialGroups.size(); i++)
	{
		
		//We initialise the mesh subset
		pSubMesh = pMesh->createSubMesh();
				
		pSubMesh->m_nFaces= m_materialGroups[i].faceCount / 3;

		Material* currMat = NULL;
		for (unsigned int j = 0; j < m_materials.size(); j++)
		{
			if (m_materials[j]->strMaterialName == m_materialGroups[i].strMaterialName)
			{
				currMat = m_materials[j];
				break;
			}
		}

		if (currMat == NULL)
		{
			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_FILE_LOAD_ERROR, 
				_T("Material not found."), 
				_T("CPepeEngine3DSFileMeshLoader::concatenateMeshes")
			);		
		}

		// ------------- material setting ------------------------------------
		pSubMesh->setMaterialName(currMat->strMaterialName);
		
		MaterialPtr pMaterial = CPepeEngineMaterialManager::getSingleton().create(
			pSubMesh->getMaterialName()
		);

		pMaterial->setDiffuse(CPepeEngineColor(
			currMat->diffuse[0],
			currMat->diffuse[1],
			currMat->diffuse[2],
			currMat->diffuse[3]
		));		
		
		pMaterial->setSpecular(CPepeEngineColor(
			currMat->specular[0],
			currMat->specular[1],
			currMat->specular[2]			
		));

		pMaterial->setEmissive(CPepeEngineColor(
			currMat->emissive[0],
			currMat->emissive[1],
			currMat->emissive[2]
		));

		pMaterial->setAmbient(CPepeEngineColor(
			currMat->ambient[0],
			currMat->ambient[1],
			currMat->ambient[2]
		));

		pMaterial->setPower(currMat->fPower);
		
		//end ------------- material setting ---------------------------------

		// ------------------------ texture setting ----------------------------
		if (!currMat->strTextureName.empty())
		{
			CPepeEngineStringUtil::toLowerCase(currMat->strTextureName);
			pMaterial->setTexture(currMat->strTextureName);			
		}
		//end ------------------------ texture setting -------------------------				

		unsigned short* pFaces = new unsigned short[m_materialGroups[i].faceCount * 3];
	
		for (size_t f = 0; f < m_materialGroups[i].faceCount; f++)
		{
			pFaces[f * 3]		= pConcatMesh->pFaces[m_materialGroups[i].pFaces[f]].a;
			pFaces[f * 3 + 1]	= pConcatMesh->pFaces[m_materialGroups[i].pFaces[f]].b;
			pFaces[f * 3 + 2]	= pConcatMesh->pFaces[m_materialGroups[i].pFaces[f]].c;
		}

		CPepeEngineIndexData *pIndexData = new CPepeEngineIndexData();

		pIndexData->m_nIndexStart	= 0;
		pIndexData->m_nIndexCount	= m_materialGroups[i].faceCount * 3;
		pIndexData->m_pIndexBuffer	= IPepeEngineBufferManager::getSingleton().createIndexBuffer(
			sizeof(Face) * m_materialGroups[i].faceCount , IPepeEngineBuffer::BU_STATIC_WRITE_ONLY, IPepeEngineIndexBuffer::IT_16BIT, 
			false
		);

		pIndexData->m_pIndexBuffer->writeData(0, sizeof(Face) * m_materialGroups[i].faceCount, pFaces, false);

		pSubMesh->setIndexData(pIndexData);	

		delete[] pFaces;
	}
	//end -------------------- create sub meshes --------------------------
}

_PEPE_ENGINE_END