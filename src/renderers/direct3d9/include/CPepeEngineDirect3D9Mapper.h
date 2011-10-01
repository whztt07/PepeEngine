/**
 * Project:     PepeEngine
 * Tier:        Backend
 * File:        CPepeEngineDirect3D9Mapper.h
 *
 * @brief       Declaration of CPepeEngineDirect3D9Mapper class. This class use Direct3D 9 library. Inspired by Ogre :)
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-03-25
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version     1.0
 */

#ifndef CPEPEENGINEDIRECT3D9MAPPER_H
#define CPEPEENGINEDIRECT3D9MAPPER_H

#include "PepeEngineDirect3D9Init.h"

#include "CPepeEngineColor.h"
#include "CPepeEngineImage.h"
#include "CPepeEngineLight.h"
#include "CPepeEngineMaterial.h"
#include "CPepeEngineMatrix4.h"
#include "CPepeEngineVector3.h"
#include "CPepeEngineVertexDeclaration.h"
#include "IPepeEngineRenderer.h"
#include "IPepeEngineIndexBuffer.h"

_PEPE_ENGINE_START

class CPepeEngineDirect3D9Mapper
{
public:
    static unsigned long        get(IPepeEngineBuffer::Usage usage);
    static unsigned long        get(IPepeEngineBuffer::LockOptions options, IPepeEngineBuffer::Usage usage);
    static D3DFORMAT            get(IPepeEngineIndexBuffer::IndexType indexType);
    static D3DCOLORVALUE        get(CPepeEngineColor color);
    static D3DMATERIAL9         get(MaterialPtr material);
    static D3DDECLTYPE          get(VertexElementType vType);
    static D3DDECLUSAGE         get(VertexElementSemantic sem);
    static D3DMATRIX            get(const CPepeEngineMatrix4& matrix);
    static unsigned long        get(const CullingMode& mode);
    static unsigned long        get(const ShadeMode& mode);
    static D3DFILLMODE          get(const PolygonMode& mode);
    static D3DLIGHTTYPE         get(const LightType& type);
    static D3DXVECTOR3          get(const CPepeEngineVector3& vec);
    static D3DFORMAT            get(PixelFormat pf);
    static PixelFormat          get(D3DFORMAT d3dPF);
    static PixelFormat          getClosestSupportedPF(PixelFormat pf);
    static D3DCUBEMAP_FACES     get(const tstring& suffix);
    static D3DTEXTUREADDRESS    get(const TextureAddressingMode& tam, const D3DCAPS9& devCaps);
    static D3DSAMPLERSTATETYPE  get(const FilterType& filterType);
    static DWORD                get(
        const FilterType& filterType,
        const FilterOptions& filterOpt,
        const D3DCAPS9& devCaps,
        const TextureType& texType
    );
    static D3DFOGMODE           get(FogMode fm);
    static D3DBLEND             get(SceneBlendFactor sbf);
};

#endif

_PEPE_ENGINE_END