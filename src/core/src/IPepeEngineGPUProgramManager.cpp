#include "PepeEnginePrecompiledHeaders.h"
#include "IPepeEngineGPUProgramManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
IPepeEngineGPUProgramManager::IPepeEngineGPUProgramManager()
{
    m_strDefVSEntryPoint    = _T("vs_main");
    m_strDefPSEntryPoint    = _T("ps_main");
}

// -----------------------------------------------------------------------------------------
IPepeEngineGPUProgramManager::~IPepeEngineGPUProgramManager()
{
}

// -----------------------------------------------------------------------------------------
ShaderType IPepeEngineGPUProgramManager::getTypeFromFileExtension(const tstring& strFileName) const
{
    if (strFileName.rfind(_T(".ps")) != -1) {
        return GPU_PIXEL_SHADER;
    } else if (strFileName.rfind(_T(".vs")) != -1) {
        return GPU_VERTEX_SHADER;
    } else {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_INVALID_PARAMETERS,
            strFileName + _T(" - Unknown shader type (ps for pixel shader, vs for vertex shader)"),
            _T("IPepeEngineGPUProgramManager::getTypeFromFileExtension")
        );
    }
}

// -----------------------------------------------------------------------------------------
void IPepeEngineGPUProgramManager::setPSShaderModel(const tstring& strShaderModel)
{
    if (isSyntaxSupported(strShaderModel)) {
        m_strDefPSShaderModel = strShaderModel;
    } else {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_INVALID_PARAMETERS,
            _T("This pixel shader model is not available on this device: ") + strShaderModel,
            _T("IPepeEngineGPUProgramManager:::setPSShaderModel")
        );
    }
}

// -----------------------------------------------------------------------------------------
void IPepeEngineGPUProgramManager::setVSShaderModel(const tstring& strShaderModel)
{
    if (isSyntaxSupported(strShaderModel)) {
        m_strDefVSShaderModel = strShaderModel;
    } else {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_INVALID_PARAMETERS,
            _T("This vertex shader model is not available on this device: ") + strShaderModel,
            _T("IPepeEngineGPUProgramManager:::setVSShaderModel")
        );
    }
}

// -----------------------------------------------------------------------------------------
bool IPepeEngineGPUProgramManager::isSyntaxSupported(const tstring& strCode) const
{
    bool bAvail = false;

    for (size_t i = 0; i < m_syntaxCodes.size(); i++) {
        if (m_syntaxCodes[i] == strCode) {
            bAvail = true;
            break;
        }
    }

    return bAvail;
}

// -----------------------------------------------------------------------------------------
IPepeEngineGPUProgramManager* IPepeEngineGPUProgramManager::ms_singleton = 0;

IPepeEngineGPUProgramManager& IPepeEngineGPUProgramManager::getSingleton(void)
{
    assert(ms_singleton);
    return *ms_singleton;
}

IPepeEngineGPUProgramManager* IPepeEngineGPUProgramManager::getSingletonPtr(void)
{
    return ms_singleton;
}

_PEPE_ENGINE_END