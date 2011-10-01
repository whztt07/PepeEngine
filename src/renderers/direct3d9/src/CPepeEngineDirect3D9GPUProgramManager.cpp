#include "CPepeEngineCore.h"
#include "CPepeEngineDirect3D9GPUProgramManager.h"
#include "CPepeEngineDirect3D9GPUProgram.h"
#include "CPepeEngineException.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
ResourcePtr CPepeEngineDirect3D9GPUProgramManager::create(const tstring& strName)
{
    ResourcePtr pGPUProgram = getByName(strName);

    if (pGPUProgram.isNull()) {
        ShaderType type = getTypeFromFileExtension(strName);

        if (type == GPU_VERTEX_SHADER) {
            if (CPepeEngineCore::getSingleton().getRenderer()->getRendererCapabilities()->hasCapability(RC_VERTEX_PROGRAM)) {
                pGPUProgram = new CPepeEngineDirect3D9VertexShader(
                    m_pDevice, strName, getVSShaderModel(), getVSEntryPoint()
                );
            } else {
                PEPE_ENGINE_EXCEPT(
                    CPepeEngineException::ERR_RENDERINGAPI_ERROR,
                    _T("This device doesn't support vertex shaders."),
                    _T("CPepeEngineDirect3D9GPUProgramManager::create")
                );
            }
        } else {
            if (CPepeEngineCore::getSingleton().getRenderer()->getRendererCapabilities()->hasCapability(RC_PIXEL_PROGRAM)) {
                pGPUProgram = new CPepeEngineDirect3D9PixelShader(
                    m_pDevice, strName, getPSShaderModel(), getPSEntryPoint()
                );
            } else {
                PEPE_ENGINE_EXCEPT(
                    CPepeEngineException::ERR_RENDERINGAPI_ERROR,
                    _T("This device doesn't support pixel shaders."),
                    _T(" CPepeEngineDirect3D9GPUProgramManager::create")
                );
            }
        }

        add(strName, pGPUProgram);
    }

    return pGPUProgram;
}

// -----------------------------------------------------------------------------------------
GPUProgramPtr CPepeEngineDirect3D9GPUProgramManager::createPixelShader(
    const tstring& strName,
    const tstring& strShaderModel,
    const tstring& strEntryPoint
)
{
    if (CPepeEngineCore::getSingleton().getRenderer()->getRendererCapabilities()->hasCapability(RC_PIXEL_PROGRAM)) {
        ResourcePtr pGPUProgram = getByName(strName);

        if (pGPUProgram.isNull()) {
            pGPUProgram = new CPepeEngineDirect3D9PixelShader(
                m_pDevice, strName, strShaderModel, strEntryPoint
            );

            add(strName, pGPUProgram);
        }

        return pGPUProgram;
    } else {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_RENDERINGAPI_ERROR,
            _T("This device doesn't support pixel shaders."),
            _T(" CPepeEngineDirect3D9GPUProgramManager::createPixelShader")
        );
    }
}

// -----------------------------------------------------------------------------------------
GPUProgramPtr CPepeEngineDirect3D9GPUProgramManager::createVertexShader(
    const tstring& strName,
    const tstring& strShaderModel,
    const tstring& strEntryPoint
)
{
    if (CPepeEngineCore::getSingleton().getRenderer()->getRendererCapabilities()->hasCapability(RC_VERTEX_PROGRAM)) {
        ResourcePtr pGPUProgram = getByName(strName);

        if (pGPUProgram.isNull()) {
            pGPUProgram = new CPepeEngineDirect3D9VertexShader(
                m_pDevice, strName, strShaderModel, strEntryPoint
            );

            add(strName, pGPUProgram);
        }

        return pGPUProgram;
    } else {

        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_RENDERINGAPI_ERROR,
            _T("This device doesn't support vertex shaders."),
            _T(" CPepeEngineDirect3D9GPUProgramManager::createVertexShader")
        );
    }
}

_PEPE_ENGINE_END