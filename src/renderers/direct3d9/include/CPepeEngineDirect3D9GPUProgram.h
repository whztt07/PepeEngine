/**
 * Project: PepeEngine
 * Tier:    Backend
 * File:    CPepeEngineDirect3D9GPUProgram.h
 *
 * @brief   Declaration of CPepeEngineDirect3D9GPUProgram class. This class use Direct3D 9 library.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-09-16
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEDIRECT3D9GPUPROGRAM_H
#define CPEPEENGINEDIRECT3D9GPUPROGRAM_H

#include "PepeEngineDirect3D9Init.h"

#include "IPepeEngineGPUProgram.h"
#include "IPepeEngineGPUProgramManager.h"

_PEPE_ENGINE_START

class CPepeEngineDirect3D9GPUProgram : public IPepeEngineGPUProgram
{
protected:
    LPD3DXCONSTANTTABLE m_pConstantTable;
    LPDIRECT3DDEVICE9   m_pDevice;
public:

    CPepeEngineDirect3D9GPUProgram(
        LPDIRECT3DDEVICE9 pDevice,
        const ShaderType& shaderType,
        const tstring &strName,
        const tstring& strShaderModel,
        const tstring& strEntryPoint
    ): IPepeEngineGPUProgram(shaderType, strName, strShaderModel, strEntryPoint) {
        m_pDevice           = pDevice;
        m_pConstantTable    = NULL;
    }

    virtual ~CPepeEngineDirect3D9GPUProgram() {
        SAFE_RELEASE(m_pConstantTable);
    }

    void setVariable(
        const tstring& strName,
        const float* pfValues,
        unsigned int nCount
    );

    void setTexture(
        const tstring& strName,
        TexturePtr pTexture
    );

    void buildConstantDefinitions();
    void processParamElement(D3DXHANDLE parent, tstring prefix, unsigned int nIndex) const;
    void populateDef(D3DXCONSTANT_DESC& d3dDesc, GpuConstantDefinition& def) const;
};

class CPepeEngineDirect3D9VertexShader : public CPepeEngineDirect3D9GPUProgram
{
    IDirect3DVertexShader9* m_pVSShader;
public:

    CPepeEngineDirect3D9VertexShader(
        LPDIRECT3DDEVICE9 pDevice,
        const tstring &strName,
        const tstring& strShaderModel,
        const tstring& strEntryPoint
    ): CPepeEngineDirect3D9GPUProgram(pDevice, GPU_VERTEX_SHADER, strName, strShaderModel, strEntryPoint) {
        m_pVSShader = NULL;
    }

    ~CPepeEngineDirect3D9VertexShader();

    void load();
    void unload();

    void create(
        const tstring& strShaderProgram,
        const tstring& strEntryPoint,
        const tstring& strShaderVersion
    );

    IDirect3DVertexShader9* getD3D9Shader() const {
        return m_pVSShader;
    }
};

class CPepeEngineDirect3D9PixelShader : public CPepeEngineDirect3D9GPUProgram
{
    IDirect3DPixelShader9* m_pPSShader;
public:

    CPepeEngineDirect3D9PixelShader(
        LPDIRECT3DDEVICE9 pDevice,
        const tstring &strName,
        const tstring& strShaderModel,
        const tstring& strEntryPoint
    ): CPepeEngineDirect3D9GPUProgram(pDevice, GPU_PIXEL_SHADER, strName, strShaderModel, strEntryPoint) {
        m_pPSShader = NULL;
    }

    ~CPepeEngineDirect3D9PixelShader();

    void load();
    void unload();

    void create(
        const tstring& strShaderProgram,
        const tstring& strEntryPoint,
        const tstring& strShaderVersion
    );

    IDirect3DPixelShader9* getD3D9Shader() const {
        return m_pPSShader;
    }

};

struct _PepeEngineExport Direct3D9GPUProgramPtr : public CPepeEngineSharedPtr<CPepeEngineDirect3D9GPUProgram> {
public:
    inline                          Direct3D9GPUProgramPtr();
    inline explicit                 Direct3D9GPUProgramPtr(CPepeEngineDirect3D9GPUProgram* r);
    inline                          Direct3D9GPUProgramPtr(const Direct3D9GPUProgramPtr& r);
    inline                          Direct3D9GPUProgramPtr(const ResourcePtr& r);
    inline Direct3D9GPUProgramPtr&  operator=(const ResourcePtr& r);
};

_PEPE_ENGINE_END

#endif