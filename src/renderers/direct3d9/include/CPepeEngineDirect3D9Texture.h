/**
 * Project: PepeEngine
 * Tier:    Backend
 * File:    CPepeEngineDirect3D9Texture.h
 *
 * @brief   Declaration of CPepeEngineDirect3D9Texture class. This class use Direct3D 9 library.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-02-28
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEDIRECT3D9TEXTURE_H
#define CPEPEENGINEDIRECT3D9TEXTURE_H

#include "PepeEngineDirect3D9Init.h"

#include "IPepeEngineTexture.h"

_PEPE_ENGINE_START

class CPepeEngineDirect3D9Texture : public IPepeEngineTexture
{
    LPDIRECT3DTEXTURE9              m_pNormTexture;
    LPDIRECT3DCUBETEXTURE9          m_pCubeTexture;
    LPDIRECT3DVOLUMETEXTURE9        m_pVolumeTexture;

    LPDIRECT3DDEVICE9               m_pDevice;
    LPDIRECT3D9                     m_pD3D;
    D3DDEVICE_CREATION_PARAMETERS   m_devCreationParams;
    D3DFORMAT                       m_BBPixelFormat;
    D3DPOOL                         m_D3DPool;
    D3DCAPS9                        m_caps;

    void createInternalResources();
    void createNormTexture();
    void createVolumeTexture();
    void createCubeMapTexture();

    void loadNormTexture();
    void loadVolumeTexture();
    void loadCubeMapTexture();

    D3DTEXTUREFILTERTYPE getBestFilterMethod();
    bool canUseDynamicTextures(DWORD srcUsage, D3DRESOURCETYPE srcType, D3DFORMAT srcFormat);
    bool canAutoGenMipmaps(unsigned int srcUsage, D3DRESOURCETYPE srcType, D3DFORMAT srcFormat);

    D3DFORMAT chooseD3DFormat();

public:

    CPepeEngineDirect3D9Texture(LPDIRECT3DDEVICE9 pDevice, const tstring &strName);

    ~CPepeEngineDirect3D9Texture();

    void load();
    void unload();

    bool useDefaultPool() const;
    void releaseIfDefaultPool();
    void recreateIfDefaultPool();

    LPDIRECT3DTEXTURE9 getD3D9Texture() const {
        return m_pNormTexture;
    }
    LPDIRECT3DCUBETEXTURE9 getD3D9CubeTexture() const {
        return m_pCubeTexture;
    }
    LPDIRECT3DVOLUMETEXTURE9 getD3D9VolumeTexture() const {
        return m_pVolumeTexture;
    }
};

struct _PepeEngineExport Direct3D9TexturePtr : public CPepeEngineSharedPtr<CPepeEngineDirect3D9Texture> {
public:
    inline                          Direct3D9TexturePtr();
    inline explicit                 Direct3D9TexturePtr(CPepeEngineDirect3D9Texture* r);
    inline                          Direct3D9TexturePtr(const Direct3D9TexturePtr& r);
    inline                          Direct3D9TexturePtr(const ResourcePtr& r);
    inline Direct3D9TexturePtr&     operator=(const ResourcePtr& r);
};


_PEPE_ENGINE_END

#endif