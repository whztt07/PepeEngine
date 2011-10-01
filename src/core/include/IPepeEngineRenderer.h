/**
 * Project:     PepeEngine
 * Tier:        Middleware
 * File:        IPepeEngineRenderer.h
 *
 * @brief       Declaration of IPepeEngineRenderer abstract class.
 *              This is interface for renderers used by engine.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-02-16
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version     1.0
 */

#ifndef IPEPEENGINERENDERER_H
#define IPEPEENGINERENDERER_H

#include "PepeEngineInit.h"

#include "CPepeEngineEvent.h"
#include "CPepeEngineLight.h"
#include "CPepeEngineMaterial.h"
#include "CPepeEngineMatrix4.h"
#include "CPepeEngineRendererCapabilities.h"
#include "CPepeEngineRenderOperation.h"
#include "CPepeEngineTextureUnitState.h"
#include "IPepeEngineGPUProgram.h"
#include "IPepeEngineTexture.h"
#include "IPepeEngineVideoModeList.h"

_PEPE_ENGINE_START

/**
 *  The polygon mode to use when rasterising.
 */
enum PolygonMode {
    PM_POINTS       = 1,    /**< Only points are rendered.      */
    PM_WIREFRAME    = 2,    /**< Wireframe models are rendered. */
    PM_SOLID        = 3     /**< Solid polygons are rendered.   */
};

/**
 *  Defines the frame buffer types.
 */
enum FrameBufferType {
    FBT_COLOR   = 0x1,
    FBT_DEPTH   = 0x2,
    FBT_STENCIL = 0x4
};

/**
 *  Fog modes.
 */
enum FogMode {
    FOG_NONE,       /**<    No fog.                                                                                                 */
    FOG_EXP,        /**<    Fog density increases  exponentially from the camera (fog = 1/e^(distance * density))                   */
    FOG_EXP2,       /**<    Fog density increases at the square of FOG_EXP, i.e. even quicker (fog = 1/e^(distance * density)^2)    */
    FOG_LINEAR      /**<    Fog density increases linearly between the start and end distances                                      */
};

/**
 * Declaration of interface for renderers.
 */
class _PepeEngineExport IPepeEngineRenderer : public IPepeEngineListener
{
protected:

    /** @{  Render States   */
    CullingMode                     m_cullingMode;
    ShadeMode                       m_shadeMode;
    PolygonMode                     m_polygonMode;
    bool                            m_bLightEnabled;
    bool                            m_bZBufferEnabled;
    bool                            m_bSpecularEnabled;
    bool                            m_bNormalizeNormalsEnabled;
    CPepeEngineColor                m_ambientLightColor;
    SceneBlendFactor                m_srcBlendFactor;
    SceneBlendFactor                m_dstBlendFactor;
    /** @}                  */

    IPepeEngineRenderWindow*        m_pRenderWindow;

    IPepeEngineGPUProgramManager*   m_pGPUProgramManager;
    IPepeEngineTextureManager*      m_pTextureManager;
    IPepeEngineBufferManager*       m_pBufferManager;
    IPepeEngineVideoModeList*       m_pVideoModeList;

    CPepeEngineRendererCapabilities* m_pCapabilities;

    CPepeEngineMatrix4  m_viewMatrix;

    unsigned short  m_nCurrentLights;
    size_t          m_nFacesCount;
public:

    IPepeEngineRenderer();

    virtual ~IPepeEngineRenderer();

    virtual void shutdown();

    virtual const tstring& getName() const = 0;

    virtual void attachWindow(IPepeEngineWindow* pWindow) = 0;

    virtual void render(const CPepeEngineRenderOperation& renderOperation) = 0;

    virtual void beginScene() const = 0;
    virtual void endScene() const = 0;
    virtual void clearFrameBuffer(unsigned int nBuffers, const CPepeEngineColor& color, float depth = 1.0f, unsigned short stencil = 0.0f) = 0;

    virtual void bindGPUProgram(IPepeEngineGPUProgram* pGPUProgram) = 0;
    virtual void unbindGPUPrograms() = 0;
    virtual void bindGPUProgramParameters(const ShaderType& shaderType, const CPepeEngineGPUProgramParameters* params) = 0;

    virtual void setWorldMatrix(const CPepeEngineMatrix4& matrix) = 0;
    virtual void setViewMatrix(const CPepeEngineMatrix4& matrix) = 0;
    virtual void setProjectionMatrix(const CPepeEngineMatrix4& matrix) = 0;

    virtual void setTexture(IPepeEngineTexture* pTexture, unsigned long lStage = 0) = 0;
    virtual void setTextureUnitState(size_t nTexUnit, const CPepeEngineTextureUnitState& texUnitState);
    virtual void setTextureAddressingMode(size_t nTexUnit, const UVWTextureAddressingMode& texAddrMode) = 0;
    virtual void setFilterOptions(
        size_t nTexUnit,
        const FilterType& filterType,
        const FilterOptions& filterOpt,
        const TextureType& texType
    ) = 0;

    virtual void setMaterial(MaterialPtr pMaterial) = 0;
    virtual void setLight(size_t nIndex, CPepeEngineLight* pLight) = 0;
    virtual void setFog(
        const FogMode& fogMode, const CPepeEngineColor& fogColor,
        float fDensity, float fStart, float fEnd
    ) = 0;

    virtual void useLights(const LightList& lights, unsigned short nLimit) = 0;

    virtual void setVertexData(IPepeEngineVertexBuffer* pVertexBuffer) = 0;
    virtual void setIndexData(IPepeEngineIndexBuffer* pIndexBuffer) = 0;

    virtual void setVertexDeclaration(CPepeEngineVertexDeclaration* pVertexDeclaration) = 0;

    virtual VertexElementType getColourVertexElementType() const = 0;

    virtual void convertProjectionMatrix(const CPepeEngineMatrix4& matrix, CPepeEngineMatrix4& dest) = 0;

    /** @{ setting render states    */
    virtual void setLightingEnabled(bool bEnabled, bool bSaveState = true) = 0;
    virtual void setAmbientLight(float r, float g, float b, bool bSaveState = true) = 0;
    virtual void setCullingMode(const CullingMode& mode, bool bSaveState = true) = 0;
    virtual void setZBufferEnabled(bool bEnabled, bool bSaveState = true) = 0;
    virtual void setSpecularEnabled(bool bEnabled, bool bSaveState = true) = 0;
    virtual void setNormalizeNormalsEnabled(bool bEnabled, bool bSaveState = true) = 0;
    virtual void setShadeMode(const ShadeMode& mode, bool bSaveState = true) = 0;
    virtual void setPolygonMode(const PolygonMode& mode, bool bSaveState = true) = 0;
    virtual void setSceneBlending(
        const SceneBlendFactor& srcFactor, const SceneBlendFactor& destFactor, bool bSaveState = true
    ) = 0;
    /** @}                          */

    const CullingMode& getCullingMode() const {
        return m_cullingMode;
    }
    const PolygonMode& getPolygonMode() const {
        return m_polygonMode;
    }
    const ShadeMode& getShadeMode() const {
        return m_shadeMode;
    }

    inline size_t getFacesCount() const;
    inline void resetFacesCount();

    virtual void initCapabilities() = 0;
    const CPepeEngineRendererCapabilities* getRendererCapabilities() const {
        return m_pCapabilities;
    }

    virtual void initConfigOptions();
    virtual void refreshFSAAOptions() = 0;
    virtual tstring validateOptions();

    virtual void drawText(const tstring& strText, size_t xPos, size_t yPos) = 0;

    IPepeEngineRenderWindow* getRenderWindow() const {
        return m_pRenderWindow;
    }
    IPepeEngineVideoModeList* getVideoModeList() const {
        return m_pVideoModeList;
    }

    void restoreRenderState();
};

_PEPE_ENGINE_END

#endif