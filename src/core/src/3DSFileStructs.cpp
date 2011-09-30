#include "PepeEnginePrecompiledHeaders.h"

_PEPE_ENGINE_START

// byte-align structures
#if PEPE_ENGINE_COMPILER == PEPE_ENGINE_COMPILER_MSVC || PEPE_ENGINE_COMPILER == PEPE_ENGINE_COMPILER_BORL
#	pragma pack(push, packing)
#	pragma pack(1)
#	define PACK_STRUCT
#elif PEPE_ENGINE_COMPILER == PEPE_ENGINE_COMPILER_GNUC
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error compiler not supported
#endif

struct ChunkHeader
{
	unsigned short	id;
	unsigned int	length;
} PACK_STRUCT;

// Default alignment
#if PEPE_ENGINE_COMPILER == PEPE_ENGINE_COMPILER_MSVC || PEPE_ENGINE_COMPILER == PEPE_ENGINE_COMPILER_BORL
#	pragma pack(pop, packing)
#endif

#undef PACK_STRUCT

struct ChunkData
{
	ChunkData() : read(0) {header.length = 0; header.id = 0;}

	ChunkHeader		header;
	unsigned int	read;
};

typedef struct tVertex
{
	float x, y, z;
} Vertex;

// byte-align structures
#if PEPE_ENGINE_COMPILER == PEPE_ENGINE_COMPILER_MSVC || PEPE_ENGINE_COMPILER == PEPE_ENGINE_COMPILER_BORL
#	pragma pack(push, packing)
#	pragma pack(1)
#	define PACK_STRUCT
#elif PEPE_ENGINE_COMPILER == PEPE_ENGINE_COMPILER_GNUC
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error compiler not supported
#endif

struct Face
{
	unsigned short a, b, c;		
} PACK_STRUCT;

// Default alignment
#if PEPE_ENGINE_COMPILER == PEPE_ENGINE_COMPILER_MSVC || PEPE_ENGINE_COMPILER == PEPE_ENGINE_COMPILER_BORL
#	pragma pack(pop, packing)
#endif

#undef PACK_STRUCT


struct TexCoords
{
	float u, v;
};

struct Material
{
	float		diffuse[4];
	float		specular[4];
	float		emissive[4];
	float		ambient[4];
	float		fPower;
	tstring		strTextureName;
	tstring		strMaterialName;
};

struct SMaterialGroup
{
	SMaterialGroup() : faceCount(0), pFaces(NULL) {};

	SMaterialGroup(const SMaterialGroup& o)
	{
		*this = o;
	}

	~SMaterialGroup() 
	{ 
		clear();
	}

	void clear() 
	{ 
		delete[] pFaces;
		pFaces		= 0;
		faceCount	= 0;
	}

	void operator =(const SMaterialGroup& o)
	{
		strMaterialName = o.strMaterialName;
		faceCount		= o.faceCount;
		pFaces			= new unsigned short[faceCount];
		for (unsigned int i = 0; i < faceCount; ++i)
		{
			pFaces[i] = o.pFaces[i];
		}
	}

	tstring			strMaterialName;
	unsigned short	faceCount;
	unsigned short* pFaces;
};

struct Mesh
{
	unsigned short nVertices;
	unsigned short nFaces;	
	unsigned short nTexCoords;

	/** @{ The nFirstxxx variables are used for mesh concatenation */
	unsigned int nFirstVertex;
	unsigned int nFirstFace;            
	unsigned int nFirstTextureCoord;	
	/** @} */

	union
	{
		Face*			pFaces;		
		unsigned short* pIndices;
	};
	Vertex*						pVertices;
	TexCoords*					pTexCoords;		
	tstring						strName;
	unsigned int*				pSmoothingGroups;

	Mesh() 
	{
		nVertices			= 0;
		nFaces				= 0;
		nTexCoords			= 0;			
		nFirstVertex		= 0;
		nFirstFace			= 0;            
		nFirstTextureCoord	= 0;						

		pFaces				= NULL;			
		pVertices			= NULL;
		pTexCoords			= NULL;	
		pSmoothingGroups	= NULL;
	}
};

// Primary chunk
const unsigned short C3DS_MAIN3DS		= 0x4D4D;

// Main Chunks
const unsigned short C3DS_EDIT3DS		= 0x3D3D;
const unsigned short C3DS_KEYF3DS		= 0xB000;
const unsigned short C3DS_VERSION		= 0x0002;
const unsigned short C3DS_MESHVERSION	= 0x3D3E;

// sub chunks of C3DS_EDIT3DS
const unsigned short C3DS_EDIT_MATERIAL = 0xAFFF;
const unsigned short C3DS_EDIT_OBJECT   = 0x4000;

// sub chunks of C3DS_EDIT_MATERIAL
const unsigned short C3DS_MATNAME       = 0xA000; 
const unsigned short C3DS_MATAMBIENT    = 0xA010;
const unsigned short C3DS_MATDIFFUSE    = 0xA020;
const unsigned short C3DS_MATSPECULAR   = 0xA030;
const unsigned short C3DS_MATSHININESS  = 0xA040;
const unsigned short C3DS_MATSHIN2PCT   = 0xA041;
const unsigned short C3DS_TRANSPARENCY  = 0xA050;
const unsigned short C3DS_TWO_SIDE      = 0xA081;
const unsigned short C3DS_WIRE          = 0xA085;
const unsigned short C3DS_SHADING       = 0xA100;
const unsigned short C3DS_MATTEXMAP     = 0xA200;
const unsigned short C3DS_MATSPECMAP    = 0xA204;
const unsigned short C3DS_MATOPACMAP    = 0xA210;
const unsigned short C3DS_MATREFLMAP    = 0xA220;
const unsigned short C3DS_MATBUMPMAP    = 0xA230;
const unsigned short C3DS_MATMAPFILE    = 0xA300;
const unsigned short C3DS_MAT_TEXTILING = 0xA351;
const unsigned short C3DS_MAT_USCALE    = 0xA354;
const unsigned short C3DS_MAT_VSCALE    = 0xA356;
const unsigned short C3DS_MAT_UOFFSET   = 0xA358;
const unsigned short C3DS_MAT_VOFFSET   = 0xA35A;

// subs of C3DS_EDIT_OBJECT
const unsigned short C3DS_OBJTRIMESH    = 0x4100;

// subs of C3DS_OBJTRIMESH
const unsigned short C3DS_TRIVERT       = 0x4110;
const unsigned short C3DS_POINTFLAGARRAY= 0x4111;
const unsigned short C3DS_TRIFACE       = 0x4120;
const unsigned short C3DS_TRIFACEMAT    = 0x4130;
const unsigned short C3DS_TRIUV         = 0x4140;
const unsigned short C3DS_TRISMOOTH     = 0x4150;
const unsigned short C3DS_TRIMATRIX     = 0x4160;
const unsigned short C3DS_MESHCOLOR     = 0x4165;
const unsigned short C3DS_DIRECT_LIGHT  = 0x4600;
const unsigned short C3DS_DL_INNER_RANGE= 0x4659;
const unsigned short C3DS_DL_OUTER_RANGE= 0x465A;
const unsigned short C3DS_DL_MULTIPLIER = 0x465B;
const unsigned short C3DS_CAMERA        = 0x4700;
const unsigned short C3DS_CAM_SEE_CONE  = 0x4710;
const unsigned short C3DS_CAM_RANGES    = 0x4720;

// subs of C3DS_KEYF3DS
const unsigned short C3DS_KF_HDR        = 0xB00A;
const unsigned short C3DS_AMBIENT_TAG   = 0xB001;
const unsigned short C3DS_OBJECT_TAG    = 0xB002;
const unsigned short C3DS_CAMERA_TAG    = 0xB003;
const unsigned short C3DS_TARGET_TAG    = 0xB004;
const unsigned short C3DS_LIGHTNODE_TAG = 0xB005;
const unsigned short C3DS_KF_SEG        = 0xB008;
const unsigned short C3DS_KF_CURTIME    = 0xB009;
const unsigned short C3DS_KF_NODE_HDR   = 0xB010;
const unsigned short C3DS_PIVOTPOINT    = 0xB013;
const unsigned short C3DS_BOUNDBOX      = 0xB014;
const unsigned short C3DS_POS_TRACK_TAG = 0xB020;
const unsigned short C3DS_ROT_TRACK_TAG = 0xB021;
const unsigned short C3DS_SCL_TRACK_TAG = 0xB022;
const unsigned short C3DS_NODE_ID       = 0xB030;

// different color chunk types
const unsigned short C3DS_COL_RGB		= 0x0010;
const unsigned short C3DS_COL_TRU		= 0x0011;
const unsigned short C3DS_COL_LIN_24	= 0x0012;
const unsigned short C3DS_COL_LIN_F		= 0x0013;

// percentage chunk types
const unsigned short C3DS_PERCENTAGE_I	= 0x0030;
const unsigned short C3DS_PERCENTAGE_F	= 0x0031;

_PEPE_ENGINE_END