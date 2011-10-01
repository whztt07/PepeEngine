#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineConfig.h"
#include "CPepeEngineMaterialManager.h"
#include "CPepeEngineMeshManager.h"
#include "IPepeEngineBufferManager.h"
#include "IPepeEngineGPUProgramManager.h"
#include "IPepeEngineRenderer.h"
#include "IPepeEngineRenderWindow.h"
#include "IPepeEngineTextureManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
IPepeEngineRenderer::IPepeEngineRenderer()
{
    m_pCapabilities             = new CPepeEngineRendererCapabilities();
    m_nCurrentLights            = 0;
    m_nFacesCount               = 0;
    m_pRenderWindow             = NULL;
    m_pTextureManager           = NULL;
    m_pBufferManager            = NULL;
    m_pGPUProgramManager        = NULL;
    m_pVideoModeList            = NULL;

    m_cullingMode               = CULL_ANTICLOCKWISE;
    m_shadeMode                 = SM_GOURAUD;
    m_polygonMode               = PM_SOLID;
    m_bLightEnabled             = true;
    m_bZBufferEnabled           = true;
    m_bSpecularEnabled          = true;
    m_bNormalizeNormalsEnabled  = true;
    m_ambientLightColor         = CPepeEngineColor::BLACK;
    m_srcBlendFactor            = SBF_ONE;
    m_dstBlendFactor            = SBF_ZERO;
}

// -----------------------------------------------------------------------------------------
IPepeEngineRenderer::~IPepeEngineRenderer()
{
    delete m_pTextureManager;
    delete m_pBufferManager;
    delete m_pGPUProgramManager;
    delete m_pCapabilities;
    delete m_pRenderWindow;
    delete m_pVideoModeList;
}

// -----------------------------------------------------------------------------------------
void IPepeEngineRenderer::shutdown()
{
    delete m_pTextureManager;
    delete m_pBufferManager;
    delete m_pGPUProgramManager;
    delete m_pRenderWindow;
}

// -----------------------------------------------------------------------------------------
void IPepeEngineRenderer::setTextureUnitState(size_t nTexUnit, const CPepeEngineTextureUnitState& texUnitState)
{
    setTextureAddressingMode(nTexUnit, texUnitState.getTextureUVWAddressingMode());
    setFilterOptions(nTexUnit, FT_MIN, texUnitState.getFilterOptions(FT_MIN), texUnitState.getTextureType());
    setFilterOptions(nTexUnit, FT_MAG, texUnitState.getFilterOptions(FT_MAG), texUnitState.getTextureType());
    setFilterOptions(nTexUnit, FT_MIP, texUnitState.getFilterOptions(FT_MIP), texUnitState.getTextureType());
}

// -----------------------------------------------------------------------------------------
size_t IPepeEngineRenderer::getFacesCount() const
{
    return m_nFacesCount;
}

// -----------------------------------------------------------------------------------------
void IPepeEngineRenderer::resetFacesCount()
{
    m_nFacesCount = 0;
}

// -----------------------------------------------------------------------------------------
void IPepeEngineRenderer::initConfigOptions()
{
    CPepeEngineConfig* pConfig = CPepeEngineCore::getSingleton().getConfig();

    ConfigOptionValue* vsync = new ConfigOptionValue;
    vsync->strDefValue  = _T("false");
    vsync->strValue     = _T("false");
    vsync->immutable    = false;
    vsync->possibleValues.push_back(_T("true"));
    vsync->possibleValues.push_back(_T("false"));
    pConfig->addOption(_T("vsync"), vsync);

    ConfigOptionValue* fullScreen = new ConfigOptionValue;;
    fullScreen->strDefValue = _T("false");
    fullScreen->strValue    = _T("false");
    fullScreen->immutable   = false;
    fullScreen->possibleValues.push_back(_T("true"));
    fullScreen->possibleValues.push_back(_T("false"));
    pConfig->addOption(_T("FullScreen"), fullScreen);

    // need to update this in renderer implementation
    ConfigOptionValue* videoMode = new ConfigOptionValue;
    videoMode->strDefValue  = _T("800 x 600 @ 32-bit colour");
    videoMode->strValue     = _T("800 x 600 @ 32-bit colour");
    videoMode->possibleValues.push_back(_T("800 x 600 @ 32-bit colour"));
    videoMode->immutable = false;
    pConfig->addOption(_T("Video Mode"), videoMode);

    ConfigOptionValue* depthBuffer = new ConfigOptionValue;
    depthBuffer->strDefValue    = _T("true");
    depthBuffer->strValue       = _T("true");
    depthBuffer->immutable      = false;
    depthBuffer->possibleValues.push_back(_T("true"));
    depthBuffer->possibleValues.push_back(_T("false"));
    pConfig->addOption(_T("Depth Buffer"), depthBuffer);

    // need to update this in renderer implementation
    ConfigOptionValue* fsaa = new ConfigOptionValue;
    fsaa->strValue      = _T("none");
    fsaa->strDefValue   = _T("none");
    fsaa->immutable     = false;
    fsaa->possibleValues.push_back(_T("none"));
    pConfig->addOption(_T("Anti-aliasing"), fsaa);
}

// -----------------------------------------------------------------------------------------
tstring IPepeEngineRenderer::validateOptions()
{
    CPepeEngineConfig* pConfig  = CPepeEngineCore::getSingleton().getConfig();
    ConfigOptionValue* val      = pConfig->getOption(_T("Video Mode"));

    if (val->strValue.empty()) {
        return _T("Video Mode is required.");
    }

    return _T("");
}

// -----------------------------------------------------------------------------------------
void IPepeEngineRenderer::restoreRenderState()
{
    setCullingMode(m_cullingMode);
    setShadeMode(m_shadeMode);
    setPolygonMode(m_polygonMode);
    setLightingEnabled(m_bLightEnabled);
    setZBufferEnabled(m_bZBufferEnabled);
    setSpecularEnabled(m_bSpecularEnabled);
    setNormalizeNormalsEnabled(m_bNormalizeNormalsEnabled);
    setAmbientLight(m_ambientLightColor.r, m_ambientLightColor.g, m_ambientLightColor.b);
    setSceneBlending(m_srcBlendFactor, m_dstBlendFactor);
}

_PEPE_ENGINE_END