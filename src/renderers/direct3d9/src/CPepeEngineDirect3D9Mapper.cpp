#include "CPepeEngineDirect3D9Mapper.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
unsigned long CPepeEngineDirect3D9Mapper::get(IPepeEngineBuffer::Usage usage)
{
    unsigned long ret = 0;

    if (usage & IPepeEngineBuffer::BU_DYNAMIC) {
#if PEPE_ENGINE_D3D_MANAGE_BUFFERS

        /**
         * Only add the dynamic flag for default pool, and
         * we use default pool when buffer is discardable
         */
        if (usage & IPepeEngineBuffer::BU_DISCARDABLE) {
            ret |= D3DUSAGE_DYNAMIC;
        }

#else
        ret |= D3DUSAGE_DYNAMIC;
#endif
    }

    if (usage & IPepeEngineBuffer::BU_WRITE_ONLY) {
        ret |= D3DUSAGE_WRITEONLY;
    }

    return ret;
}

// -----------------------------------------------------------------------------------------
unsigned long CPepeEngineDirect3D9Mapper::get(IPepeEngineBuffer::LockOptions options, IPepeEngineBuffer::Usage usage)
{
    unsigned long ret = 0;

    if (options == IPepeEngineBuffer::BL_DISCARD) {
#if PEPE_ENGINE_D3D_MANAGE_BUFFERS

        /**
         * Only add the discard flag for dynamic usgage and default pool
         */
        if ((usage & IPepeEngineBuffer::BU_DYNAMIC) && (usage & IPepeEngineBuffer::BU_DISCARDABLE)) {
            ret |= D3DLOCK_DISCARD;
        }

#else

        /**
         * D3D doesn't like discard or no_overwrite on non-dynamic buffers
         */
        if (usage & IPepeEngineBuffer::BU_DYNAMIC) {
            ret |= D3DLOCK_DISCARD;
        }

#endif
    }

    if (options == IPepeEngineBuffer::BL_READ_ONLY) {
        /**
         * D3D debug runtime doesn't like you locking managed buffers readonly
         * when they were created with write-only (even though you CAN read
         * from the software backed version)
         */
        if (!(usage & IPepeEngineBuffer::BU_WRITE_ONLY)) {
            ret |= D3DLOCK_READONLY;
        }

    }

    if (options == IPepeEngineBuffer::BL_NO_OVERWRITE) {
#if PEPE_ENGINE_D3D_MANAGE_BUFFERS

        /**
         * Only add the nooverwrite flag for dynamic usgage and default pool
         */
        if ((usage & IPepeEngineBuffer::BU_DYNAMIC) && (usage & IPepeEngineBuffer::BU_DISCARDABLE)) {
            ret |= D3DLOCK_NOOVERWRITE;
        }

#else

        /**
         * D3D doesn't like discard or no_overwrite on non-dynamic buffers
         */
        if (usage & IPepeEngineBuffer::BU_DYNAMIC) {
            ret |= D3DLOCK_NOOVERWRITE;
        }

#endif
    }

    return ret;
}

// -----------------------------------------------------------------------------------------
D3DFORMAT CPepeEngineDirect3D9Mapper::get(IPepeEngineIndexBuffer::IndexType indexType)
{
    if (indexType == IPepeEngineIndexBuffer::IT_32BIT) {
        return D3DFMT_INDEX32;
    } else {
        return D3DFMT_INDEX16;
    }
}

// -----------------------------------------------------------------------------------------
D3DCOLORVALUE CPepeEngineDirect3D9Mapper::get(CPepeEngineColor color)
{
    D3DCOLORVALUE c;

    c.r = color.r;
    c.g = color.g;
    c.b = color.b;
    c.a = color.a;

    return c;
}

// -----------------------------------------------------------------------------------------
D3DMATERIAL9 CPepeEngineDirect3D9Mapper::get(MaterialPtr material)
{
    D3DMATERIAL9 mat;
    ZeroMemory(&mat, sizeof(mat));

    mat.Ambient     = get(material->getAmbient());
    mat.Diffuse     = get(material->getDiffuse());
    mat.Specular    = get(material->getSpecular());
    mat.Emissive    = get(material->getEmissive());
    mat.Power       = material->getPower();

    return mat;
}

// -----------------------------------------------------------------------------------------
D3DDECLTYPE CPepeEngineDirect3D9Mapper::get(VertexElementType vType)
{
    switch (vType) {
    case VET_COLOUR:
    case VET_COLOUR_ABGR:
    case VET_COLOUR_ARGB:
        return D3DDECLTYPE_D3DCOLOR;
        break;

    case VET_FLOAT1:
        return D3DDECLTYPE_FLOAT1;
        break;

    case VET_FLOAT2:
        return D3DDECLTYPE_FLOAT2;
        break;

    case VET_FLOAT3:
        return D3DDECLTYPE_FLOAT3;
        break;

    case VET_FLOAT4:
        return D3DDECLTYPE_FLOAT4;
        break;

    case VET_SHORT2:
        return D3DDECLTYPE_SHORT2;
        break;

    case VET_SHORT4:
        return D3DDECLTYPE_SHORT4;
        break;

    case VET_UBYTE4:
        return D3DDECLTYPE_UBYTE4;
        break;
    }

    // to keep compiler happy
    return D3DDECLTYPE_FLOAT3;
}

// -----------------------------------------------------------------------------------------
D3DDECLUSAGE CPepeEngineDirect3D9Mapper::get(VertexElementSemantic sem)
{
    switch (sem) {
    case VES_BLEND_INDICES:
        return D3DDECLUSAGE_BLENDINDICES;
        break;

    case VES_BLEND_WEIGHTS:
        return D3DDECLUSAGE_BLENDWEIGHT;
        break;

    case VES_DIFFUSE:
        return D3DDECLUSAGE_COLOR; // NB index will differentiate
        break;

    case VES_SPECULAR:
        return D3DDECLUSAGE_COLOR; // NB index will differentiate
        break;

    case VES_NORMAL:
        return D3DDECLUSAGE_NORMAL;
        break;

    case VES_POSITION:
        return D3DDECLUSAGE_POSITION;
        break;

    case VES_TEXTURE_COORDINATES:
        return D3DDECLUSAGE_TEXCOORD;
        break;

    case VES_BINORMAL:
        return D3DDECLUSAGE_BINORMAL;
        break;

    case VES_TANGENT:
        return D3DDECLUSAGE_TANGENT;
        break;
    }

    // to keep compiler happy
    return D3DDECLUSAGE_POSITION;
}

// -----------------------------------------------------------------------------------------
D3DMATRIX CPepeEngineDirect3D9Mapper::get(const CPepeEngineMatrix4& matrix)
{
    /*
        Transpose matrix
        D3D9 uses row vectors i.e. V*M
        OpenGL and everything else uses column vectors i.e. M*V
    */
    return D3DXMATRIX(
               matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0],
               matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1],
               matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
               matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]
           );
}

// -----------------------------------------------------------------------------------------
unsigned long CPepeEngineDirect3D9Mapper::get(const CullingMode& mode)
{
    switch (mode) {
    case CULL_NONE:
        return D3DCULL_NONE;

    case CULL_CLOCKWISE:
        return D3DCULL_CW;

    case CULL_ANTICLOCKWISE:
        return D3DCULL_CCW;
    }

    return 0;
}

// -----------------------------------------------------------------------------------------
unsigned long CPepeEngineDirect3D9Mapper::get(const ShadeMode& mode)
{
    switch (mode) {
    case SM_FLAT:
        return D3DSHADE_FLAT;

    case SM_GOURAUD:
        return D3DSHADE_GOURAUD;

    case SM_PHONG:
        return D3DSHADE_PHONG;
    }

    return 0;
}

// -----------------------------------------------------------------------------------------
D3DFILLMODE CPepeEngineDirect3D9Mapper::get(const PolygonMode& mode)
{
    switch (mode) {
    case PM_POINTS:
        return D3DFILL_POINT;

    case PM_WIREFRAME:
        return D3DFILL_WIREFRAME;

    case PM_SOLID:
        return D3DFILL_SOLID;
    }

    return D3DFILL_FORCE_DWORD;
}

// -----------------------------------------------------------------------------------------
D3DLIGHTTYPE CPepeEngineDirect3D9Mapper::get(const LightType& type)
{
    switch (type) {
    case LT_POINT:
        return D3DLIGHT_POINT;

    case LT_DIRECTIONAL:
        return D3DLIGHT_DIRECTIONAL;

    case LT_SPOTLIGHT:
        return D3DLIGHT_SPOT;
    }

    return D3DLIGHT_DIRECTIONAL;
}

// -----------------------------------------------------------------------------------------
D3DXVECTOR3 CPepeEngineDirect3D9Mapper::get(const CPepeEngineVector3& vec)
{
    return D3DXVECTOR3(vec.x, vec.y, vec.z);
}

// -----------------------------------------------------------------------------------------
D3DFORMAT CPepeEngineDirect3D9Mapper::get(PixelFormat pf)
{
    switch(pf) {
    case PF_L8:
        return D3DFMT_L8;

    case PF_L16:
        return D3DFMT_L16;

    case PF_A8:
        return D3DFMT_A8;

    case PF_A4L4:
        return D3DFMT_A4L4;

    case PF_BYTE_LA:
        return D3DFMT_A8L8; // Assume little endian here

    case PF_R3G3B2:
        return D3DFMT_R3G3B2;

    case PF_A1R5G5B5:
        return D3DFMT_A1R5G5B5;

    case PF_R5G6B5:
        return D3DFMT_R5G6B5;

    case PF_A4R4G4B4:
        return D3DFMT_A4R4G4B4;

    case PF_R8G8B8:
        return D3DFMT_R8G8B8;

    case PF_A8R8G8B8:
        return D3DFMT_A8R8G8B8;

    case PF_A8B8G8R8:
        return D3DFMT_A8B8G8R8;

    case PF_X8R8G8B8:
        return D3DFMT_X8R8G8B8;

    case PF_X8B8G8R8:
        return D3DFMT_X8B8G8R8;

    case PF_A2B10G10R10:
        return D3DFMT_A2B10G10R10;

    case PF_A2R10G10B10:
        return D3DFMT_A2R10G10B10;

    case PF_FLOAT16_R:
        return D3DFMT_R16F;

    case PF_FLOAT16_GR:
        return D3DFMT_G16R16F;

    case PF_FLOAT16_RGBA:
        return D3DFMT_A16B16G16R16F;

    case PF_FLOAT32_R:
        return D3DFMT_R32F;

    case PF_FLOAT32_GR:
        return D3DFMT_G32R32F;

    case PF_FLOAT32_RGBA:
        return D3DFMT_A32B32G32R32F;

    case PF_SHORT_RGBA:
        return D3DFMT_A16B16G16R16;

    case PF_SHORT_GR:
        return D3DFMT_G16R16;

    case PF_DXT1:
        return D3DFMT_DXT1;

    case PF_DXT2:
        return D3DFMT_DXT2;

    case PF_DXT3:
        return D3DFMT_DXT3;

    case PF_DXT4:
        return D3DFMT_DXT4;

    case PF_DXT5:
        return D3DFMT_DXT5;

    case PF_UNKNOWN:
    default:
        return D3DFMT_UNKNOWN;
    }
}

// -----------------------------------------------------------------------------------------
PixelFormat CPepeEngineDirect3D9Mapper::get(D3DFORMAT d3dPF)
{
    switch(d3dPF) {
    case D3DFMT_A8:
        return PF_A8;

    case D3DFMT_L8:
        return PF_L8;

    case D3DFMT_L16:
        return PF_L16;

    case D3DFMT_A4L4:
        return PF_A4L4;

    case D3DFMT_A8L8:
        return PF_BYTE_LA;  // Assume little endian here

    case D3DFMT_R3G3B2:
        return PF_R3G3B2;

    case D3DFMT_A1R5G5B5:
        return PF_A1R5G5B5;

    case D3DFMT_A4R4G4B4:
        return PF_A4R4G4B4;

    case D3DFMT_R5G6B5:
        return PF_R5G6B5;

    case D3DFMT_R8G8B8:
        return PF_R8G8B8;

    case D3DFMT_X8R8G8B8:
        return PF_X8R8G8B8;

    case D3DFMT_A8R8G8B8:
        return PF_A8R8G8B8;

    case D3DFMT_X8B8G8R8:
        return PF_X8B8G8R8;

    case D3DFMT_A8B8G8R8:
        return PF_A8B8G8R8;

    case D3DFMT_A2R10G10B10:
        return PF_A2R10G10B10;

    case D3DFMT_A2B10G10R10:
        return PF_A2B10G10R10;

    case D3DFMT_R16F:
        return PF_FLOAT16_R;

    case D3DFMT_A16B16G16R16F:
        return PF_FLOAT16_RGBA;

    case D3DFMT_R32F:
        return PF_FLOAT32_R;

    case D3DFMT_G32R32F:
        return PF_FLOAT32_GR;

    case D3DFMT_A32B32G32R32F:
        return PF_FLOAT32_RGBA;

    case D3DFMT_G16R16F:
        return PF_FLOAT16_GR;

    case D3DFMT_A16B16G16R16:
        return PF_SHORT_RGBA;

    case D3DFMT_G16R16:
        return PF_SHORT_GR;

    case D3DFMT_DXT1:
        return PF_DXT1;

    case D3DFMT_DXT2:
        return PF_DXT2;

    case D3DFMT_DXT3:
        return PF_DXT3;

    case D3DFMT_DXT4:
        return PF_DXT4;

    case D3DFMT_DXT5:
        return PF_DXT5;

    default:
        return PF_UNKNOWN;
    }
}

// -----------------------------------------------------------------------------------------
PixelFormat CPepeEngineDirect3D9Mapper::getClosestSupportedPF(PixelFormat pf)
{
    if (get(pf) != D3DFMT_UNKNOWN) {
        return pf;
    }

    switch(pf) {
    case PF_B5G6R5:
        return PF_R5G6B5;

    case PF_B8G8R8:
        return PF_R8G8B8;

    case PF_B8G8R8A8:
        return PF_A8R8G8B8;

    case PF_SHORT_RGB:
        return PF_SHORT_RGBA;

    case PF_FLOAT16_RGB:
        return PF_FLOAT16_RGBA;

    case PF_FLOAT32_RGB:
        return PF_FLOAT32_RGBA;

    case PF_UNKNOWN:
    default:
        return PF_A8R8G8B8;
    }
}

// -----------------------------------------------------------------------------------------
D3DCUBEMAP_FACES CPepeEngineDirect3D9Mapper::get(const tstring& suffix)
{
    if (suffix == _T("_UP")) {
        return D3DCUBEMAP_FACE_POSITIVE_Y;
    }

    if (suffix == _T("_BK")) {
        return D3DCUBEMAP_FACE_NEGATIVE_Z;
    }

    if (suffix == _T("_DN")) {
        return D3DCUBEMAP_FACE_NEGATIVE_Y;
    }

    if (suffix == _T("_FR")) {
        return D3DCUBEMAP_FACE_POSITIVE_Z;
    }

    if (suffix == _T("_LF")) {
        return D3DCUBEMAP_FACE_NEGATIVE_X;
    }

    if (suffix == _T("_RT")) {
        return D3DCUBEMAP_FACE_POSITIVE_X;
    }

    return (D3DCUBEMAP_FACES)0;
}

// -----------------------------------------------------------------------------------------
D3DTEXTUREADDRESS CPepeEngineDirect3D9Mapper::get(const TextureAddressingMode& tam, const D3DCAPS9& devCaps)
{
    switch (tam) {
    case TAM_WRAP:
        return D3DTADDRESS_WRAP;

    case TAM_MIRROR:
        return D3DTADDRESS_MIRROR;

    case TAM_CLAMP:
        return D3DTADDRESS_CLAMP;

    case TAM_BORDER:
        if (devCaps.TextureAddressCaps & D3DPTADDRESSCAPS_BORDER) {
            return D3DTADDRESS_BORDER;
        } else {
            return D3DTADDRESS_CLAMP;
        }
    }

    return D3DTADDRESS_FORCE_DWORD;
}

// -----------------------------------------------------------------------------------------
D3DSAMPLERSTATETYPE CPepeEngineDirect3D9Mapper::get(const FilterType& filterType)
{
    switch (filterType) {
    case FT_MIN:
        return D3DSAMP_MINFILTER;

    case FT_MAG:
        return D3DSAMP_MAGFILTER;

    case FT_MIP:
        return D3DSAMP_MIPFILTER;
    }

    return D3DSAMP_MINFILTER;
}

// -----------------------------------------------------------------------------------------
DWORD CPepeEngineDirect3D9Mapper::get(
    const FilterType& filterType,
    const FilterOptions& filterOpt,
    const D3DCAPS9& devCaps,
    const TextureType& texType
)
{
    // Assume normal
    DWORD capsType = devCaps.TextureFilterCaps;

    switch (texType) {
    case TEX_TYPE_2D:
        capsType = devCaps.TextureFilterCaps;
        break;

    case TEX_TYPE_CUBE_MAP:
        capsType = devCaps.CubeTextureFilterCaps;
        break;

    case TEX_TYPE_3D:
        capsType = devCaps.VolumeTextureFilterCaps;
        break;
    }

    switch (filterType) {
    case FT_MIN:
        switch (filterOpt) {
        case FO_ANISOTROPIC:
            if (capsType & D3DPTFILTERCAPS_MINFANISOTROPIC) {
                return D3DTEXF_ANISOTROPIC;
            }

        case FO_LINEAR:
            if (capsType & D3DPTFILTERCAPS_MINFLINEAR) {
                return D3DTEXF_LINEAR;
            }

        case FO_POINT:
        case FO_NONE:
            return D3DTEXF_POINT;
        }

        break;

    case FT_MAG:
        switch (filterOpt) {
        case FO_ANISOTROPIC:
            if (capsType & D3DPTFILTERCAPS_MAGFANISOTROPIC) {
                return D3DTEXF_ANISOTROPIC;
            }

        case FO_LINEAR:
            if (capsType & D3DPTFILTERCAPS_MAGFLINEAR) {
                return D3DTEXF_LINEAR;
            }

        case FO_POINT:
        case FO_NONE:
            return D3DTEXF_POINT;
        }

        break;

    case FT_MIP:
        switch (filterOpt) {
        case FO_ANISOTROPIC:
        case FO_LINEAR:
            if (capsType & D3DPTFILTERCAPS_MIPFLINEAR) {
                return D3DTEXF_LINEAR;
            }

        case FO_POINT:
            if (capsType & D3DPTFILTERCAPS_MIPFPOINT) {
                return D3DTEXF_POINT;
            }

        case FO_NONE:
            return D3DTEXF_NONE;
        }

        break;
    }

    return 0;
}

// -----------------------------------------------------------------------------------------
D3DFOGMODE CPepeEngineDirect3D9Mapper::get(FogMode fm)
{
    switch (fm) {
    case FOG_EXP:
        return D3DFOG_EXP;

    case FOG_EXP2:
        return D3DFOG_EXP2;

    case FOG_LINEAR:
        return D3DFOG_LINEAR;
    }

    return D3DFOG_FORCE_DWORD;
}

// -----------------------------------------------------------------------------------------
D3DBLEND CPepeEngineDirect3D9Mapper::get(SceneBlendFactor sbf)
{
    switch( sbf ) {
    case SBF_ONE:
        return D3DBLEND_ONE;

    case SBF_ZERO:
        return D3DBLEND_ZERO;

    case SBF_DEST_COLOUR:
        return D3DBLEND_DESTCOLOR;

    case SBF_SOURCE_COLOUR:
        return D3DBLEND_SRCCOLOR;

    case SBF_ONE_MINUS_DEST_COLOUR:
        return D3DBLEND_INVDESTCOLOR;

    case SBF_ONE_MINUS_SOURCE_COLOUR:
        return D3DBLEND_INVSRCCOLOR;

    case SBF_DEST_ALPHA:
        return D3DBLEND_DESTALPHA;

    case SBF_SOURCE_ALPHA:
        return D3DBLEND_SRCALPHA;

    case SBF_ONE_MINUS_DEST_ALPHA:
        return D3DBLEND_INVDESTALPHA;

    case SBF_ONE_MINUS_SOURCE_ALPHA:
        return D3DBLEND_INVSRCALPHA;
    }

    return D3DBLEND_FORCE_DWORD;
}

_PEPE_ENGINE_END