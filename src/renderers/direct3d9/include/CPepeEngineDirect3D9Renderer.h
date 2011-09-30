/**
 * Project: PepeEngine
 * Tier:	Backend 
 * File:	CPepeEngineDirect3D9Renderer.h 
 *
 * @class	CPepeEngineDirect3D9Renderer
 *
 * @brief	Declaration of CPepeEngineDirect3D9Renderer class. 
 *			This is renderer using Direct3D 9 library.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-02-16
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEDIRECT3D9RENDERER_H
#define CPEPEENGINEDIRECT3D9RENDERER_H

#include "PepeEngineDirect3D9Init.h"

#include "CPepeEngineDirect3D9VideoModeList.h"
#include "IPepeEngineRenderer.h"

_PEPE_ENGINE_START

/**
 * Declaration of CPepeEngineDirect3DRenderer class.
 * Implementation of IPepeEngineRenderer.
 *
 * @see IPepeEngineRenderer
 */
class CPepeEngineDirect3D9Renderer : public IPepeEngineRenderer
{	

	D3DCAPS9		m_caps;
	LPDIRECT3D9		m_pD3D;
	ID3DXFont*		m_pFont;

	/**
	 *	Per-stage constant support? (not in main caps since D3D specific & minor)
	 */
	bool m_bPerStageConstantSupport;
	bool m_bDeviceLost;
	
	HRESULT setRenderState(D3DRENDERSTATETYPE state, unsigned long value);

	HRESULT CPepeEngineDirect3D9Renderer::setSamplerState(
		unsigned long sampler, D3DSAMPLERSTATETYPE type, unsigned long value);

	HRESULT CPepeEngineDirect3D9Renderer::setTextureStageState(
		unsigned long stage, D3DTEXTURESTAGESTATETYPE type, unsigned long value);

	void convertVertexShaderCaps(void);
	void convertPixelShaderCaps(void);
	bool checkVertexTextureFormats();
	bool checkMultiSampleQuality(
		D3DMULTISAMPLE_TYPE type, unsigned long *outQuality, D3DFORMAT format,
		unsigned int adapterNum, D3DDEVTYPE deviceType, bool fullScreen
	);
	void createSystemFont();
public:

	/**
	 * Constructor of CPepeEngineDirectRenderer class
	 */
	CPepeEngineDirect3D9Renderer();

	
	/**
	 * Destructor of CPepeEngineDirectRenderer class
	 */
	~CPepeEngineDirect3D9Renderer();

	void shutdown();

	const tstring& getName() const;
	
	void attachWindow(IPepeEngineWindow* pWindow);	
	
	void render(const CPepeEngineRenderOperation& renderOperation);

	void beginScene() const;
	void endScene() const;
	void clearFrameBuffer(unsigned int nBuffers, const CPepeEngineColor& color, float depth = 1.0f, unsigned short stencil = 0.0f);

	void bindGPUProgram(IPepeEngineGPUProgram* pGPUProgram);
	void unbindGPUPrograms();
	void bindGPUProgramParameters(const ShaderType& shaderType, const CPepeEngineGPUProgramParameters* params);
		
	void setWorldMatrix(const CPepeEngineMatrix4& matrix);
	void setViewMatrix(const CPepeEngineMatrix4& matrix);
	void setProjectionMatrix(const CPepeEngineMatrix4& matrix);

	void setTexture(IPepeEngineTexture* pTexture, unsigned long lStage = 0);
	void setTextureAddressingMode(size_t nTexUnit, const UVWTextureAddressingMode& texAddrMode);
	void setFilterOptions(
		size_t nTexUnit, 
		const FilterType& filterType, 
		const FilterOptions& filterOpt,
		const TextureType& texType
	);

	void setMaterial(MaterialPtr pMaterial);
	void setLight(size_t nIndex, CPepeEngineLight* pLight);
	void setFog(
		const FogMode& fogMode, const CPepeEngineColor& fogColor = CPepeEngineColor::BLACK, 
		float fDensity = 0.0f, float fStart = 0.0f, float fEnd = 0.0f
	);
	
	void useLights(const LightList& lights, unsigned short nLimit);

	void setVertexData(IPepeEngineVertexBuffer* pVertexBuffer);
	void setIndexData(IPepeEngineIndexBuffer* pIndexBuffer);

	void setVertexDeclaration(CPepeEngineVertexDeclaration* pVertexDeclaration);

	VertexElementType getColourVertexElementType() const {return VET_COLOUR_ARGB;}

	void convertProjectionMatrix(const CPepeEngineMatrix4& matrix, CPepeEngineMatrix4& dest);
	
	/** @{ setting render states	*/
	void setLightingEnabled(bool bEnabled, bool bSaveState = true);
	void setAmbientLight(float r, float g, float b, bool bSaveState = true);
	void setCullingMode(const CullingMode& mode, bool bSaveState = true);	
	void setZBufferEnabled(bool bEnabled, bool bSaveState = true);
	void setSpecularEnabled(bool bEnabled, bool bSaveState = true);
	void setNormalizeNormalsEnabled(bool bEnabled, bool bSaveState = true);
	void setShadeMode(const ShadeMode& mode, bool bSaveState = true);	
	void setPolygonMode(const PolygonMode& mode, bool bSaveState = true);	
	void setSceneBlending(
		const SceneBlendFactor& srcFactor, const SceneBlendFactor& destFactor, bool bSaveState = true
	);
	/** @}							*/

	void initCapabilities();
	void initConfigOptions();
	void refreshFSAAOptions();
	tstring validateOptions();

	void drawText(const tstring& strText, size_t xPos, size_t yPos);

	void notifyDeviceLost();
	void restoreLostDevice();
	bool isDeviceLost() const {return m_bDeviceLost;}

	void notify(const CPepeEngineEvent& event);

	LPDIRECT3DDEVICE9 getD3DDevice() const;
};

_PEPE_ENGINE_END

#endif