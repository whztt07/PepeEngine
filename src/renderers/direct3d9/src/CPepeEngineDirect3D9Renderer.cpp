#include "CPepeEngineConverter.h"
#include "CPepeEngineCore.h"
#include "CPepeEngineDirect3D9BufferManager.h"
#include "CPepeEngineDirect3D9GPUProgram.h"
#include "CPepeEngineDirect3D9GPUProgramManager.h"
#include "CPepeEngineDirect3D9Renderer.h"
#include "CPepeEngineDirect3D9Mapper.h"
#include "CPepeEngineDirect3D9RenderWindow.h"
#include "CPepeEngineDirect3D9TextureManager.h"
#include "CPepeEngineException.h"
#include "CPepeEngineStringUtil.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
HRESULT CPepeEngineDirect3D9Renderer::setRenderState(D3DRENDERSTATETYPE state, unsigned long value)
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();
	unsigned long		oldVal;

	if (FAILED(hr = pd3dDevice->GetRenderState(state, &oldVal)))
	{
		return hr;
	}

	if (oldVal == value)
	{
		return D3D_OK;
	} else
	{
		return pd3dDevice->SetRenderState(state, value);
	}
}

// -----------------------------------------------------------------------------------------
HRESULT CPepeEngineDirect3D9Renderer::setSamplerState(
	unsigned long sampler, D3DSAMPLERSTATETYPE type, unsigned long value)
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();
	unsigned long		oldVal;

	if (FAILED(hr = pd3dDevice->GetSamplerState(sampler, type, &oldVal)))
	{
		return hr;
	}
	if (oldVal == value)
	{
		return D3D_OK;
	} else
	{
		return pd3dDevice->SetSamplerState(sampler, type, value);
	}
}

// -----------------------------------------------------------------------------------------
HRESULT CPepeEngineDirect3D9Renderer::setTextureStageState(
	unsigned long stage, D3DTEXTURESTAGESTATETYPE type, unsigned long value)
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();
	unsigned long		oldVal;

	// can only set fixed-function texture stage state
	if (stage < 8)
	{
		if (FAILED(hr = pd3dDevice->GetTextureStageState(stage, type, &oldVal)))
		{
			return hr;
		}

		if (oldVal == value)
		{
			return D3D_OK;
		} else
		{
			return pd3dDevice->SetTextureStageState(stage, type, value);
		}
	} else
	{
		return D3D_OK;
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::convertVertexShaderCaps(void)
{
	unsigned short major, minor;
	major = static_cast<unsigned short>((m_caps.VertexShaderVersion & 0x0000FF00) >> 8);
	minor = static_cast<unsigned short>(m_caps.VertexShaderVersion & 0x000000FF);

	bool vs2x = false;
	bool vs2a = false;

	// Special case detection for vs_2_x/a support
	if (major >= 2)
	{
		if ((m_caps.VS20Caps.Caps & D3DVS20CAPS_PREDICATION) &&
			(m_caps.VS20Caps.DynamicFlowControlDepth > 0) &&
			(m_caps.VS20Caps.NumTemps >= 12))
		{
			vs2x = true;
		}

		if ((m_caps.VS20Caps.Caps & D3DVS20CAPS_PREDICATION) &&
			(m_caps.VS20Caps.DynamicFlowControlDepth > 0) &&
			(m_caps.VS20Caps.NumTemps >= 13))
		{
			vs2a = true;
		}
	}

	// Populate max version & params
	switch (major)
	{
	case 1:
		m_pCapabilities->setMaxVertexProgramVersion(_T("vs_1_1"));
		// No boolean params allowed
		m_pCapabilities->setVertexProgramConstantBoolCount(0);
		// No integer params allowed
		m_pCapabilities->setVertexProgramConstantIntCount(0);
		// float params, always 4D
		m_pCapabilities->setVertexProgramConstantFloatCount((unsigned short)m_caps.MaxVertexShaderConst);

		break;
	case 2:
		if (vs2a)
		{
			m_pCapabilities->setMaxVertexProgramVersion(_T("vs_2_a"));
		}
		else if (vs2x)
		{
			m_pCapabilities->setMaxVertexProgramVersion(_T("vs_2_x"));
		}
		else
		{
			m_pCapabilities->setMaxVertexProgramVersion(_T("vs_2_0"));
		}
		// 16 boolean params allowed
		m_pCapabilities->setVertexProgramConstantBoolCount(16);
		// 16 integer params allowed, 4D
		m_pCapabilities->setVertexProgramConstantIntCount(16);
		// float params, always 4D
		m_pCapabilities->setVertexProgramConstantFloatCount((unsigned short)m_caps.MaxVertexShaderConst);
		break;
	case 3:
		m_pCapabilities->setMaxVertexProgramVersion(_T("vs_3_0"));
		// 16 boolean params allowed
		m_pCapabilities->setVertexProgramConstantBoolCount(16);
		// 16 integer params allowed, 4D
		m_pCapabilities->setVertexProgramConstantIntCount(16);
		// float params, always 4D
		m_pCapabilities->setVertexProgramConstantFloatCount((unsigned short)m_caps.MaxVertexShaderConst);
		break;
	default:
		m_pCapabilities->setMaxVertexProgramVersion(_T(""));
		break;
	}

	// populate syntax codes in program manager (no breaks in this one so it falls through)
	switch(major)
	{
	case 3:
		m_pGPUProgramManager->pushSyntaxCode(_T("vs_3_0"));
	case 2:
		if (vs2x)
			m_pGPUProgramManager->pushSyntaxCode(_T("vs_2_x"));
		if (vs2a)
			m_pGPUProgramManager->pushSyntaxCode(_T("vs_2_a"));

		m_pGPUProgramManager->pushSyntaxCode(_T("vs_2_0"));
	case 1:
		m_pGPUProgramManager->pushSyntaxCode(_T("vs_1_1"));
		m_pCapabilities->setCapability(RC_VERTEX_PROGRAM);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::convertPixelShaderCaps(void)
{
	unsigned short major, minor;
	major = static_cast<unsigned short>((m_caps.PixelShaderVersion & 0x0000FF00) >> 8);
	minor = static_cast<unsigned short>(m_caps.PixelShaderVersion & 0x000000FF);

	bool ps2a = false;
	bool ps2b = false;
	bool ps2x = false;

	// Special case detection for ps_2_x/a/b support
	if (major >= 2)
	{
		if ((m_caps.PS20Caps.Caps & D3DPS20CAPS_NOTEXINSTRUCTIONLIMIT) &&
			(m_caps.PS20Caps.NumTemps >= 32))
		{
			ps2b = true;
		}

		if ((m_caps.PS20Caps.Caps & D3DPS20CAPS_NOTEXINSTRUCTIONLIMIT) &&
			(m_caps.PS20Caps.Caps & D3DPS20CAPS_NODEPENDENTREADLIMIT) &&
			(m_caps.PS20Caps.Caps & D3DPS20CAPS_ARBITRARYSWIZZLE) &&
			(m_caps.PS20Caps.Caps & D3DPS20CAPS_GRADIENTINSTRUCTIONS) &&
			(m_caps.PS20Caps.Caps & D3DPS20CAPS_PREDICATION) &&
			(m_caps.PS20Caps.NumTemps >= 22))
		{
			ps2a = true;
		}

		// Does this enough?
		if (ps2a || ps2b)
		{
			ps2x = true;
		}
	}

	switch (major)
	{
	case 1:
		switch(minor)
		{
		case 1:
			m_pCapabilities->setMaxPixelProgramVersion(_T("ps_1_1"));
			break;
		case 2:
			m_pCapabilities->setMaxPixelProgramVersion(_T("ps_1_2"));
			break;
		case 3:
			m_pCapabilities->setMaxPixelProgramVersion(_T("ps_1_3"));
			break;
		case 4:
			m_pCapabilities->setMaxPixelProgramVersion(_T("ps_1_4"));
			break;
		}
		// no boolean params allowed
		m_pCapabilities->setPixelProgramConstantBoolCount(0);
		// no integer params allowed
		m_pCapabilities->setPixelProgramConstantIntCount(0);
		// float params, always 4D
		// NB in ps_1_x these are actually stored as fixed point values,
		// but they are entered as floats
		m_pCapabilities->setPixelProgramConstantFloatCount(8);
		break;
	case 2:
		if (ps2a)
		{
			m_pCapabilities->setMaxPixelProgramVersion(_T("ps_2_a"));
		}
		else if (ps2b)
		{
			m_pCapabilities->setMaxPixelProgramVersion(_T("ps_2_b"));
		}
		else if (ps2x)
		{
			m_pCapabilities->setMaxPixelProgramVersion(_T("ps_2_x"));
		}
		else
		{
			m_pCapabilities->setMaxPixelProgramVersion(_T("ps_2_0"));
		}
		// 16 boolean params allowed
		m_pCapabilities->setPixelProgramConstantBoolCount(16);
		// 16 integer params allowed, 4D
		m_pCapabilities->setPixelProgramConstantIntCount(16);
		// float params, always 4D
		m_pCapabilities->setPixelProgramConstantFloatCount(32);
		break;
	case 3:
		if (minor > 0)
		{
			m_pCapabilities->setMaxPixelProgramVersion(_T("ps_3_x"));
		}
		else
		{
			m_pCapabilities->setMaxPixelProgramVersion(_T("ps_3_0"));
		}
		// 16 boolean params allowed
		m_pCapabilities->setPixelProgramConstantBoolCount(16);
		// 16 integer params allowed, 4D
		m_pCapabilities->setPixelProgramConstantIntCount(16);
		// float params, always 4D
		m_pCapabilities->setPixelProgramConstantFloatCount(224);
		break;
	default:
		m_pCapabilities->setMaxPixelProgramVersion(_T(""));
		break;
	}

	// populate syntax codes in program manager (no breaks in this one so it falls through)
	switch (major)
	{
	case 3:
		if (minor > 0)
			m_pGPUProgramManager->pushSyntaxCode(_T("ps_3_x"));

		m_pGPUProgramManager->pushSyntaxCode(_T("ps_3_0"));
	case 2:
		if (ps2x)
			m_pGPUProgramManager->pushSyntaxCode(_T("ps_2_x"));
		if (ps2a)
			m_pGPUProgramManager->pushSyntaxCode(_T("ps_2_a"));
		if (ps2b)
			m_pGPUProgramManager->pushSyntaxCode(_T("ps_2_b"));

		m_pGPUProgramManager->pushSyntaxCode(_T("ps_2_0"));
	case 1:
		if (major > 1 || minor >= 4)
			m_pGPUProgramManager->pushSyntaxCode(_T("ps_1_4"));
		if (major > 1 || minor >= 3)
			m_pGPUProgramManager->pushSyntaxCode(_T("ps_1_3"));
		if (major > 1 || minor >= 2)
			m_pGPUProgramManager->pushSyntaxCode(_T("ps_1_2"));

		m_pGPUProgramManager->pushSyntaxCode(_T("ps_1_1"));
		m_pCapabilities->setCapability(RC_PIXEL_PROGRAM);
	}
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineDirect3D9Renderer::checkVertexTextureFormats()
{
	
	bool anySupported = false;

	LPDIRECT3DSURFACE9 bbSurf = ((CPepeEngineDirect3D9RenderWindow* )m_pRenderWindow)->getRenderSurface();
	D3DSURFACE_DESC bbSurfDesc;
	bbSurf->GetDesc(&bbSurfDesc);

	D3DFORMAT formats[29] = {
		D3DFMT_L8, D3DFMT_L16, D3DFMT_A8, D3DFMT_A4L4, D3DFMT_A8L8, D3DFMT_R3G3B2,
		D3DFMT_A1R5G5B5, D3DFMT_R5G6B5, D3DFMT_A4R4G4B4, D3DFMT_R8G8B8, D3DFMT_A8R8G8B8, 
		D3DFMT_A8B8G8R8, D3DFMT_X8R8G8B8, D3DFMT_X8B8G8R8, D3DFMT_A2B10G10R10, D3DFMT_A2R10G10B10,
		D3DFMT_R16F, D3DFMT_G16R16F, D3DFMT_A16B16G16R16F, D3DFMT_R32F, D3DFMT_G32R32F, D3DFMT_A32B32G32R32F,
		D3DFMT_A16B16G16R16, D3DFMT_G16R16, D3DFMT_DXT1, D3DFMT_DXT2, D3DFMT_DXT3, D3DFMT_DXT4, D3DFMT_DXT5
	};

	for (size_t ipf = 0; ipf < 29; ++ipf)
	{		
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, bbSurfDesc.Format, 
			D3DUSAGE_QUERY_VERTEXTEXTURE, D3DRTYPE_TEXTURE, formats[ipf])))
		{	
			anySupported = true;	
		}
	}

	return anySupported;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineDirect3D9Renderer::checkMultiSampleQuality(
	D3DMULTISAMPLE_TYPE type, unsigned long *outQuality, D3DFORMAT format,
	unsigned int adapterNum, D3DDEVTYPE deviceType, bool fullScreen)
{
	HRESULT hr;
	hr = m_pD3D->CheckDeviceMultiSampleType( 
		adapterNum, 
		deviceType, 
		format, 
		fullScreen, 
		type, 
		outQuality
	);

	if (SUCCEEDED(hr))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CPepeEngineDirect3D9Renderer::createSystemFont()
{	
	HRESULT hr = D3DXCreateFont( 
		getD3DDevice(),					// D3D device
		10,								// Height
		0,								// Width
		FW_BOLD,						// Weight
		1,								// MipLevels, 0 = autogen mipmaps
		FALSE,							// Italic
		DEFAULT_CHARSET,				// CharSet
		OUT_DEFAULT_PRECIS,				// OutputPrecision
		DEFAULT_QUALITY,				// Quality
		DEFAULT_PITCH | FF_DONTCARE,	// PitchAndFamily
		_T("System"),					// pFaceName
		&m_pFont						// ppFont
	);       

	if (FAILED(hr))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Creating default font failed: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::createSystemFont")
		);
	}		
}

// -----------------------------------------------------------------------------------------
CPepeEngineDirect3D9Renderer::CPepeEngineDirect3D9Renderer() : 
	IPepeEngineRenderer(), m_bPerStageConstantSupport(false), m_bDeviceLost(false)
{		
	// Create our Direct3D object
	if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Failed to create Direct3D 9 object."), 
			_T("CPepeEngineDirect3D9Renderer::CPepeEngineDirect3D9Renderer")
		);
	}

	m_pVideoModeList = new CPepeEngineDirect3D9VideoModeList(m_pD3D);
	initConfigOptions();

	m_pFont = NULL;
}

// -----------------------------------------------------------------------------------------
CPepeEngineDirect3D9Renderer::~CPepeEngineDirect3D9Renderer()
{	
	SAFE_RELEASE(m_pD3D);	
	SAFE_RELEASE(m_pFont);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::shutdown()
{
	getD3DDevice()->SetStreamSource(0, NULL, 0, 0);	
	IPepeEngineRenderer::shutdown();	
}

// -----------------------------------------------------------------------------------------
const tstring& CPepeEngineDirect3D9Renderer::getName() const
{
	static const tstring strName = _T("Direct3D9 Renderer");

	return strName;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::attachWindow(IPepeEngineWindow* pWindow)
{	
	m_pRenderWindow = new CPepeEngineDirect3D9RenderWindow(pWindow, m_pD3D);
	refreshFSAAOptions();
	m_pRenderWindow->create();		

	m_pTextureManager		= new CPepeEngineDirect3D9TextureManager(getD3DDevice());
	m_pGPUProgramManager	= new CPepeEngineDirect3D9GPUProgramManager(getD3DDevice());
	m_pBufferManager		= new CPepeEngineDirect3D9BufferManager(getD3DDevice());
	
	initCapabilities();

	createSystemFont();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::render(const CPepeEngineRenderOperation& renderOperation)
{
	if (!renderOperation.m_pVertexData || renderOperation.m_pVertexData->m_nVertexCount == 0) return;

	D3DPRIMITIVETYPE	primType	= D3DPT_TRIANGLELIST;
	unsigned long		primCount	= 0;

	switch (renderOperation.m_operationType)
	{
		case CPepeEngineRenderOperation::OT_POINT_LIST:
			primType	= D3DPT_POINTLIST;
			primCount	= (unsigned long)(
				renderOperation.m_bUseIndexes ? renderOperation.m_pIndexData->m_nIndexCount : renderOperation.m_pVertexData->m_nVertexCount
			);
			break;

		case CPepeEngineRenderOperation::OT_LINE_LIST:
			primType	= D3DPT_LINELIST;
			primCount	= (unsigned long)(
				renderOperation.m_bUseIndexes ? renderOperation.m_pIndexData->m_nIndexCount : renderOperation.m_pVertexData->m_nVertexCount
			) / 2;
			break;

		case CPepeEngineRenderOperation::OT_LINE_STRIP:
			primType	= D3DPT_LINESTRIP;
			primCount	= (unsigned long)(
				renderOperation.m_bUseIndexes ? renderOperation.m_pIndexData->m_nIndexCount : renderOperation.m_pVertexData->m_nVertexCount
			) - 1;
			break;

		case CPepeEngineRenderOperation::OT_TRIANGLE_LIST:
			primType	= D3DPT_TRIANGLELIST;
			primCount	= (unsigned long)(
				renderOperation.m_bUseIndexes ? renderOperation.m_pIndexData->m_nIndexCount : renderOperation.m_pVertexData->m_nVertexCount
			) / 3;
			break;

		case CPepeEngineRenderOperation::OT_TRIANGLE_STRIP:
			primType	= D3DPT_TRIANGLESTRIP;
			primCount	= (unsigned long)(
				renderOperation.m_bUseIndexes ? renderOperation.m_pIndexData->m_nIndexCount : renderOperation.m_pVertexData->m_nVertexCount
			) - 2;
			break;

		case CPepeEngineRenderOperation::OT_TRIANGLE_FAN:
			primType	= D3DPT_TRIANGLEFAN;
			primCount	= (unsigned long)(
				renderOperation.m_bUseIndexes ? renderOperation.m_pIndexData->m_nIndexCount : renderOperation.m_pVertexData->m_nVertexCount
			) - 2;
			break;
	}

	if (!primCount)
		return;

	m_nFacesCount += primCount;

	HRESULT				hr;	
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();
	
	setVertexDeclaration(renderOperation.m_pVertexData->m_pVertexDeclaration);	
	setVertexData(renderOperation.m_pVertexData->m_pVertexBuffer);
	
	if (renderOperation.m_bUseIndexes)
	{
		setIndexData(renderOperation.m_pIndexData->m_pIndexBuffer);
		hr = pd3dDevice->DrawIndexedPrimitive(
			primType, 
			static_cast<unsigned int>(renderOperation.m_pVertexData->m_nVertexStart),
			0, 
			static_cast<unsigned int>(renderOperation.m_pVertexData->m_nVertexCount),
			static_cast<unsigned int>(renderOperation.m_pIndexData->m_nIndexStart),
			static_cast<unsigned int>(primCount)
		);
	} else
	{
		hr = pd3dDevice->DrawPrimitive(
			primType, 
			static_cast<unsigned int>(renderOperation.m_pVertexData->m_nVertexStart),
			static_cast<unsigned int>(primCount)
		); 
	}

	if (hr != S_OK)
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Draw primitive error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::render")
		);
	}		
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::beginScene() const
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();
		
	if (FAILED(hr = pd3dDevice->BeginScene()))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Error begining frame: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::beginScene")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::endScene() const
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();

	if (FAILED(hr = pd3dDevice->EndScene()))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Error ending frame: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::endScene")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::clearFrameBuffer(unsigned int nBuffers, const CPepeEngineColor& color, float depth, unsigned short stencil)
{
	DWORD flags = 0;
	if (nBuffers & FBT_COLOR)
	{
		flags |= D3DCLEAR_TARGET;
	}
	
	if ( (nBuffers & FBT_DEPTH) && m_pRenderWindow->hasDepthBuffer())
	{
		flags |= D3DCLEAR_ZBUFFER;
	}
	// Only try to clear the stencil buffer if supported	
	if ((nBuffers & FBT_STENCIL) && m_pCapabilities->hasCapability(RC_HWSTENCIL))
	{
		flags |= D3DCLEAR_STENCIL;
	}

	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();

	if (FAILED(hr = pd3dDevice->Clear(0, NULL, flags, color.getAsARGB(), depth, stencil)))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Error cleaning frame buffer: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::clearFrameBuffer")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::bindGPUProgram(IPepeEngineGPUProgram* pGPUProgram)
{	
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();

	assert(pGPUProgram != NULL);
	
	switch (pGPUProgram->getShaderType())
	{
		case GPU_PIXEL_SHADER:			
			if (FAILED(hr = pd3dDevice->SetPixelShader( ((CPepeEngineDirect3D9PixelShader*)pGPUProgram)->getD3D9Shader() )))
			{
				tstring msg = DXGetErrorDescription9(hr);

				PEPE_ENGINE_EXCEPT(
					CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
					_T("Set pixel shader error: ") + msg, 
					_T("CPepeEngineDirect3D9Renderer::setGPUProgram")
				);
			}
		break;

		case GPU_VERTEX_SHADER:			
			if (FAILED(hr = pd3dDevice->SetVertexShader( ((CPepeEngineDirect3D9VertexShader*)pGPUProgram)->getD3D9Shader() )))
			{
				tstring msg = DXGetErrorDescription9(hr);

				PEPE_ENGINE_EXCEPT(
					CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
					_T("Set vertex shader error: ") + msg, 
					_T("CPepeEngineDirect3D9Renderer::setGPUProgram")
				);
			}
		break;
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::unbindGPUPrograms()
{
	IDirect3DDevice9 *pd3dDevice = getD3DDevice();

	pd3dDevice->SetVertexShader(NULL);
	pd3dDevice->SetPixelShader(NULL);	
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::bindGPUProgramParameters(const ShaderType& shaderType, const CPepeEngineGPUProgramParameters* params)
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();
		 
	const GpuLogicalBufferStruct* floatLogical	= params->getFloatLogicalBufferStruct();
	const GpuLogicalBufferStruct* intLogical	= params->getIntLogicalBufferStruct();

	switch(shaderType)
	{
	case GPU_VERTEX_SHADER:		 
		{
			for (GpuLogicalIndexUseMap::const_iterator i = floatLogical->map.begin();
				i != floatLogical->map.end(); ++i)
			{
				size_t logicalIndex = i->first;
				const float* pFloat = params->getFloatPointer(i->second.m_nPhysicalIndex);
				size_t slotCount	= i->second.m_nCurrentSize / 4;
				
				assert(i->second.m_nCurrentSize % 4 == 0 && "Should not have any elements less than 4 wide for D3D9");

				if (FAILED (hr = pd3dDevice->SetVertexShaderConstantF(logicalIndex, pFloat, slotCount)))
				{
					tstring msg = DXGetErrorDescription9(hr);

					PEPE_ENGINE_EXCEPT(
						CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
						_T("Unable to upload vertex shader float parameters: ") + msg, 
						_T("CPepeEngineDirect3D9Renderer::bindGPUProgramParameters")
					);
				}
			}

		}
		// bind ints
		{
			for (GpuLogicalIndexUseMap::const_iterator i = intLogical->map.begin();
				i != intLogical->map.end(); ++i)
			{
				size_t logicalIndex = i->first;
				const int* pInt		= params->getIntPointer(i->second.m_nPhysicalIndex);
				size_t slotCount	= i->second.m_nCurrentSize / 4;
				
				assert (i->second.m_nCurrentSize % 4 == 0 && "Should not have any elements less than 4 wide for D3D9");

				if (FAILED(hr = pd3dDevice->SetVertexShaderConstantI(logicalIndex, pInt, slotCount)))
				{
					tstring msg = DXGetErrorDescription9(hr);

					PEPE_ENGINE_EXCEPT(
						CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
						_T("Unable to upload vertex shader int parameters: ") + msg, 
						_T("CPepeEngineDirect3D9Renderer::bindGPUProgramParameters")
					);				
				}
			}
		}

		break;
	case GPU_PIXEL_SHADER:
		{
			for (GpuLogicalIndexUseMap::const_iterator i = floatLogical->map.begin();
				i != floatLogical->map.end(); ++i)
			{
				size_t logicalIndex = i->first;
				const float* pFloat = params->getFloatPointer(i->second.m_nPhysicalIndex);
				size_t slotCount	= i->second.m_nCurrentSize / 4;

				assert (i->second.m_nCurrentSize % 4 == 0 && "Should not have any elements less than 4 wide for D3D9");

				if (FAILED(hr = pd3dDevice->SetPixelShaderConstantF(logicalIndex, pFloat, slotCount)))
				{
					tstring msg = DXGetErrorDescription9(hr);

					PEPE_ENGINE_EXCEPT(
						CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
						_T("Unable to upload pixel shader float parameters: ") + msg, 
						_T("CPepeEngineDirect3D9Renderer::bindGPUProgramParameters")
					);		
				}
			}
		}
		// bind ints
		{
			for (GpuLogicalIndexUseMap::const_iterator i = intLogical->map.begin();
				i != intLogical->map.end(); ++i)
			{
				size_t logicalIndex = i->first;
				const int* pInt		= params->getIntPointer(i->second.m_nPhysicalIndex);
				size_t slotCount	= i->second.m_nCurrentSize / 4;
				assert (i->second.m_nCurrentSize % 4 == 0 && "Should not have any elements less than 4 wide for D3D9");

				if (FAILED(hr = pd3dDevice->SetPixelShaderConstantI(logicalIndex, pInt, slotCount)))
				{
					tstring msg = DXGetErrorDescription9(hr);

					PEPE_ENGINE_EXCEPT(
						CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
						_T("Unable to upload pixel shader int parameters: ") + msg, 
						_T("CPepeEngineDirect3D9Renderer::bindGPUProgramParameters")
					);	
				}
			}
		}
		break;
	};
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setWorldMatrix(const CPepeEngineMatrix4& matrix)
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();

	hr = pd3dDevice->SetTransform(D3DTS_WORLD, &CPepeEngineDirect3D9Mapper::get(matrix));

	if (hr != S_OK)
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set world matrix error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setWorldMatrix")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setViewMatrix(const CPepeEngineMatrix4& matrix)
{	
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();

	// save latest view matrix
	m_viewMatrix		= matrix;
	m_viewMatrix[2][0]	= -m_viewMatrix[2][0];
	m_viewMatrix[2][1]	= -m_viewMatrix[2][1];
    m_viewMatrix[2][2]	= -m_viewMatrix[2][2];
    m_viewMatrix[2][3]	= -m_viewMatrix[2][3];

	hr = pd3dDevice->SetTransform(D3DTS_VIEW, &CPepeEngineDirect3D9Mapper::get(m_viewMatrix));

	if (hr != S_OK)
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set view matrix error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setViewMatrix")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setProjectionMatrix(const CPepeEngineMatrix4& matrix)
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();

	hr = pd3dDevice->SetTransform(D3DTS_PROJECTION, &CPepeEngineDirect3D9Mapper::get(matrix));

	if (hr != S_OK)
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set projection matrix error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setProjectionMatrix")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setTexture(IPepeEngineTexture* pTexture, unsigned long lStage)
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();

	if (pTexture != NULL)
	{
		CPepeEngineDirect3D9Texture* pD3D9Texture = (CPepeEngineDirect3D9Texture*)pTexture;

		switch (pD3D9Texture->getTextureType())
		{
			case TEX_TYPE_1D:
			case TEX_TYPE_2D:
				hr = pd3dDevice->SetTexture(lStage, pD3D9Texture->getD3D9Texture());
				break;
			case TEX_TYPE_3D:
				hr = pd3dDevice->SetTexture(lStage, pD3D9Texture->getD3D9VolumeTexture());
				break;
			case TEX_TYPE_CUBE_MAP:
				hr = pd3dDevice->SetTexture(lStage, pD3D9Texture->getD3D9CubeTexture());
				break;
			default:
				PEPE_ENGINE_EXCEPT(
					CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
					_T("Unknown texture type."), 
					_T("CPepeEngineDirect3D9Renderer::setTexture")
				);
		}
	} else
	{
		hr = pd3dDevice->SetTexture(0, 0);
	}

	if (hr != S_OK)
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set texture(") + pTexture->getName() + _T(") error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setTexture")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setTextureAddressingMode(
	size_t nTexUnit, const UVWTextureAddressingMode& texAddrMode)
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();

	assert(pd3dDevice != NULL);

	hr = setSamplerState(nTexUnit, D3DSAMP_ADDRESSU, CPepeEngineDirect3D9Mapper::get(texAddrMode.u, m_caps));
	
	if (hr != S_OK)
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set texture addressing mode 'u' error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setTextureAddressingMode")
		);
	}


	hr = setSamplerState(nTexUnit, D3DSAMP_ADDRESSV, CPepeEngineDirect3D9Mapper::get(texAddrMode.v, m_caps));

	if (hr != S_OK)
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set texture addressing mode 'v' error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setTextureAddressingMode")
		);
	}

	hr = setSamplerState(nTexUnit, D3DSAMP_ADDRESSW, CPepeEngineDirect3D9Mapper::get(texAddrMode.w, m_caps));

	if (hr != S_OK)
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set texture addressing mode 'w' error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setTextureAddressingMode")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setFilterOptions(
	size_t nTexUnit, 
	const FilterType& filterType,
	const FilterOptions& filterOpt, 
	const TextureType& texType
)
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();

	assert(pd3dDevice != NULL);

	hr = setSamplerState(
		nTexUnit, 
		CPepeEngineDirect3D9Mapper::get(filterType), 
		CPepeEngineDirect3D9Mapper::get(filterType, filterOpt, m_caps, texType)
	);
	
	if (hr != S_OK)
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set texture addressing mode 'u' error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setTextureAddressingMode")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setMaterial(MaterialPtr pMaterial)
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();
	
	D3DMATERIAL9 d3d9Material = CPepeEngineDirect3D9Mapper::get(pMaterial);
	
	hr = pd3dDevice->SetMaterial(&d3d9Material);

	if (hr != S_OK)
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set material(") + pMaterial->getName() + _T(") error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setMaterial")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setLight(size_t nIndex, CPepeEngineLight* pLight)
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();

	if (pLight == NULL)
	{
		if (FAILED(hr = pd3dDevice->LightEnable(nIndex, FALSE)))
		{
			tstring msg = DXGetErrorDescription9(hr);

			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
				_T("Unable to disable light: ") + msg, 
				_T("CPepeEngineDirect3D9Renderer::setLight")
			);
		}		
	} else
	{
		D3DLIGHT9 d3dLight;
		ZeroMemory(&d3dLight, sizeof(d3dLight));

		d3dLight.Type = CPepeEngineDirect3D9Mapper::get(pLight->getLightType());

		if (d3dLight.Type == D3DLIGHT_SPOT)
		{
			d3dLight.Falloff	= pLight->getFalloff();
			d3dLight.Theta		= pLight->getTheta().valueRadians();
			d3dLight.Phi		= pLight->getPhi().valueRadians();
		}

		d3dLight.Diffuse	= CPepeEngineDirect3D9Mapper::get(pLight->getDiffuse());		
		d3dLight.Specular	= CPepeEngineDirect3D9Mapper::get(pLight->getSpecular());		
		
		if (pLight->getLightType() != LT_DIRECTIONAL)
		{
			d3dLight.Position = CPepeEngineDirect3D9Mapper::get(pLight->getDerivedPosition());
		}

		if (pLight->getLightType() != LT_POINT )
		{			
			d3dLight.Direction = CPepeEngineDirect3D9Mapper::get(pLight->getDerivedDirection());
		}

		d3dLight.Range			= pLight->getRange();
		d3dLight.Attenuation0	= pLight->getConstantAttenuation();
		d3dLight.Attenuation1	= pLight->getLinearAttenuation();
		d3dLight.Attenuation2	= pLight->getQuadricAttenuation();

		if (FAILED(hr = pd3dDevice->SetLight(nIndex, &d3dLight)))
		{
			tstring msg = DXGetErrorDescription9(hr);

			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
				_T("Unable to set light details: ") + msg, 
				_T("CPepeEngineDirect3D9Renderer::setLight")
			);
		}

		if (FAILED(hr = pd3dDevice->LightEnable(nIndex, TRUE)))
		{
			tstring msg = DXGetErrorDescription9(hr);

			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
				_T("Unable to enable light: ") + msg, 
				_T("CPepeEngineDirect3D9Renderer::setLight")
			);
		}
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setFog(
	const FogMode& fogMode, const CPepeEngineColor& fogColor, 
	float fDensity, float fStart, float fEnd
)
{
	HRESULT				hr;
	D3DRENDERSTATETYPE	fogType, fogTypeNot;

	if (m_caps.RasterCaps & D3DPRASTERCAPS_FOGTABLE)
	{
		fogType		= D3DRS_FOGTABLEMODE;
		fogTypeNot	= D3DRS_FOGVERTEXMODE;
	}
	else
	{
		fogType		= D3DRS_FOGVERTEXMODE;
		fogTypeNot	= D3DRS_FOGTABLEMODE;
	}

	if (fogMode == FOG_NONE)
	{
		// just disable
		hr = setRenderState(fogType, D3DFOG_NONE);
		hr = setRenderState(D3DRS_FOGENABLE, FALSE);
	} else
	{
		// Allow fog
		hr = setRenderState(D3DRS_FOGENABLE, TRUE);
		hr = setRenderState(fogTypeNot, D3DFOG_NONE);
		hr = setRenderState(fogType, CPepeEngineDirect3D9Mapper::get(fogMode));

		hr = setRenderState(D3DRS_FOGCOLOR, fogColor.getAsARGB());
		hr = setRenderState(D3DRS_FOGSTART, *(LPDWORD)(&fStart));
		hr = setRenderState(D3DRS_FOGEND, *(LPDWORD)(&fEnd));
		hr = setRenderState(D3DRS_FOGDENSITY, *(LPDWORD)(&fDensity));
	}

	if (FAILED(hr))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Error setting render state: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setFogt")
		);		
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::useLights(const LightList& lights, unsigned short nLimit)
{
	LightList::const_iterator i, iend;	

	iend				= lights.end();
	unsigned short num	= 0;

	for (i = lights.begin(); i != iend && num < nLimit; ++i, ++num)
	{
		setLight(num, *i);
	}
	// Disable extra lights
	for (; num < m_nCurrentLights; ++num)
	{
		setLight(num, NULL);
	}      
	
	m_nCurrentLights = min(nLimit, static_cast<unsigned short>(lights.size()));
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setVertexData(IPepeEngineVertexBuffer* pVertexBuffer)
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();
	
	CPepeEngineDirect3D9VertexBuffer* pD3D9VertexBuffer = static_cast<CPepeEngineDirect3D9VertexBuffer*>(
		pVertexBuffer
	);

	hr = pd3dDevice->SetStreamSource(0, pD3D9VertexBuffer->getD3DVertexBuffer(), 0, pVertexBuffer->getVertexSize());

	if (hr != S_OK)
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set Vertex Data error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setVertexData")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setIndexData(IPepeEngineIndexBuffer* pIndexBuffer)
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();
	
	CPepeEngineDirect3D9IndexBuffer* pD3D9IndexBuffer = static_cast<CPepeEngineDirect3D9IndexBuffer*>(
		pIndexBuffer
	);

	hr = pd3dDevice->SetIndices(pD3D9IndexBuffer->getD3DIndexBuffer());

	if (hr != S_OK)
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set Indices error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setIndexData")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setVertexDeclaration(CPepeEngineVertexDeclaration* pVertexDeclaration)
{
	HRESULT				hr;
	IDirect3DDevice9	*pd3dDevice = getD3DDevice();
	
	CPepeEngineDirect3D9VertexDeclaration* pD3D9VertexDeclaration = static_cast<CPepeEngineDirect3D9VertexDeclaration*>(
		pVertexDeclaration
	);

	hr = pd3dDevice->SetVertexDeclaration(pD3D9VertexDeclaration->getD3DVertexDeclaration());

	if (hr != S_OK)
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set vertex declaration error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setVertexDeclaration")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::convertProjectionMatrix(
	const CPepeEngineMatrix4& matrix, 
	CPepeEngineMatrix4& dest)
{
	dest = matrix;

	// Convert depth range from [-1,+1] to [0,1]
	dest[2][0] = (dest[2][0] + dest[3][0]) / 2;
	dest[2][1] = (dest[2][1] + dest[3][1]) / 2;
	dest[2][2] = (dest[2][2] + dest[3][2]) / 2;
	dest[2][3] = (dest[2][3] + dest[3][3]) / 2;

	// Convert right-handed to left-handed
	dest[0][2] = -dest[0][2];
	dest[1][2] = -dest[1][2];
	dest[2][2] = -dest[2][2];
	dest[3][2] = -dest[3][2];	
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setLightingEnabled(bool bEnabled, bool bSaveState)
{	
	if (bSaveState)
	{
		m_bLightEnabled = bEnabled;
	}
	HRESULT hr;
	if (FAILED(hr = setRenderState(D3DRS_LIGHTING, bEnabled ? TRUE : FALSE)))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set lighting enabled error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setLightingEnabled")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setAmbientLight(float r, float g, float b, bool bSaveState)
{
	if (bSaveState)
	{
		m_ambientLightColor.set(r, g, b);
	}

	HRESULT hr;
	if (FAILED(hr = setRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(r, g, b, 1.0f))))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set ambient light error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setAmbientLight")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setCullingMode(const CullingMode& mode, bool bSaveState)
{
	if (bSaveState)
	{
		m_cullingMode = mode;
	}

	HRESULT hr;
	if (FAILED(hr = setRenderState(D3DRS_CULLMODE, CPepeEngineDirect3D9Mapper::get(mode))))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set cull mode error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setCullingMode")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setZBufferEnabled(bool bEnabled, bool bSaveState)
{
	if (bSaveState)
	{
		m_bZBufferEnabled = bEnabled;
	}

	HRESULT hr;
	if (FAILED(hr = setRenderState(D3DRS_ZENABLE, bEnabled ? TRUE : FALSE)))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set z buffer enabled error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setZBufferEnabled")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setSpecularEnabled(bool bEnabled, bool bSaveState)
{
	if (bSaveState)
	{
		m_bSpecularEnabled = bEnabled;
	}

	HRESULT hr;
	if (FAILED(hr = setRenderState(D3DRS_SPECULARENABLE, bEnabled ? TRUE : FALSE)))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set specular enabled error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setSpecularEnabled")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setNormalizeNormalsEnabled(bool bEnabled, bool bSaveState)
{
	if (bSaveState)
	{
		m_bNormalizeNormalsEnabled = bEnabled;
	}

	HRESULT hr;
	if (FAILED(hr = setRenderState(D3DRS_NORMALIZENORMALS, bEnabled ? TRUE : FALSE)))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set normalize normals enabled error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setNormalizeNormalsEnabled")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setShadeMode(const ShadeMode& mode, bool bSaveState)
{
	if (bSaveState)
	{
		m_shadeMode = mode;
	}

	HRESULT hr;
	if (FAILED(hr = setRenderState(D3DRS_SHADEMODE, CPepeEngineDirect3D9Mapper::get(mode))))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set shade mode error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setShadeMode")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setPolygonMode(const PolygonMode& mode, bool bSaveState)
{
	if (bSaveState)
	{
		m_polygonMode = mode;
	}

	HRESULT hr;
	if (FAILED(hr = setRenderState(D3DRS_FILLMODE, CPepeEngineDirect3D9Mapper::get(mode))))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Set polygon mode error: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::setPolygonMode")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::setSceneBlending(
	const SceneBlendFactor& srcFactor, const SceneBlendFactor& destFactor, bool bSaveState
)
{
	if (bSaveState)
	{
		m_srcBlendFactor = srcFactor;
		m_dstBlendFactor = destFactor;	
	}

	HRESULT hr;
	if (srcFactor == SBF_ONE && destFactor == SBF_ZERO)
	{
		if (FAILED(hr = setRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		{
			tstring msg = DXGetErrorDescription9(hr);

			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
				_T("Failed to set alpha blending option: ") + msg, 
				_T("CPepeEngineDirect3D9Renderer::setSceneBlending")
			);			
		}
	}
	else
	{
		if (FAILED(hr = setRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		{
			tstring msg = DXGetErrorDescription9(hr);

			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
				_T("Failed to set alpha blending option: ") + msg, 
				_T("CPepeEngineDirect3D9Renderer::setSceneBlending")
			);				
		}

		if (FAILED(hr = setRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE)))
		{
			tstring msg = DXGetErrorDescription9(hr);

			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
				_T("Failed to set separate alpha blending option: ") + msg, 
				_T("CPepeEngineDirect3D9Renderer::setSceneBlending")
			);				
		}

		if (FAILED(hr = setRenderState(D3DRS_SRCBLEND, CPepeEngineDirect3D9Mapper::get(srcFactor))))
		{
			tstring msg = DXGetErrorDescription9(hr);

			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
				_T("Failed to set source blend: ") + msg, 
				_T("CPepeEngineDirect3D9Renderer::setSceneBlending")
			);				
		}

		if (FAILED(hr = setRenderState(D3DRS_DESTBLEND, CPepeEngineDirect3D9Mapper::get(destFactor))))
		{
			tstring msg = DXGetErrorDescription9(hr);

			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
				_T("Failed to set destination blend: ") + msg, 
				_T("CPepeEngineDirect3D9Renderer::setSceneBlending")
			);				
		}
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::initCapabilities()
{	
	IDirect3DDevice9 *pd3dDevice = getD3DDevice();

	// get caps
	pd3dDevice->GetDeviceCaps(&m_caps);

	// Check for hardware stencil support
	LPDIRECT3DSURFACE9	pSurf;
	D3DSURFACE_DESC		surfDesc;
	pd3dDevice->GetDepthStencilSurface(&pSurf);
	if (pSurf)
	{
		pSurf->GetDesc(&surfDesc);
		pSurf->Release();

		if (surfDesc.Format == D3DFMT_D24S8 || surfDesc.Format == D3DFMT_D24X8)
		{
			m_pCapabilities->setCapability(RC_HWSTENCIL);
			// Actually, it's always 8-bit
			m_pCapabilities->setStencilBufferBitDepth(8);
		}
	}

	// Set number of texture units
	m_pCapabilities->setNumTextureUnits((unsigned short)m_caps.MaxSimultaneousTextures);

	// Anisotropy?
	if (m_caps.MaxAnisotropy > 1)
	{
		m_pCapabilities->setCapability(RC_ANISOTROPY);
	}

	// Automatic mipmap generation?
	if (m_caps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP)
	{
		m_pCapabilities->setCapability(RC_AUTOMIPMAP);
	}

	// Blending between stages supported
	m_pCapabilities->setCapability(RC_BLENDING);
	
	// Dot 3
	if (m_caps.TextureOpCaps & D3DTEXOPCAPS_DOTPRODUCT3)
	{
		m_pCapabilities->setCapability(RC_DOT3);
	}

	// Cube map
	if (m_caps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP)
	{
		m_pCapabilities->setCapability(RC_CUBEMAPPING);
	}

	// We always support compression, D3DX will decompress if device does not support
	m_pCapabilities->setCapability(RC_TEXTURE_COMPRESSION);
	m_pCapabilities->setCapability(RC_TEXTURE_COMPRESSION_DXT);

	// We always support VBOs
	m_pCapabilities->setCapability(RC_VBO);

	// Scissor test
	if (m_caps.RasterCaps & D3DPRASTERCAPS_SCISSORTEST)
	{
		m_pCapabilities->setCapability(RC_SCISSOR_TEST);
	}

	// Two-sided stencil
	if (m_caps.StencilCaps & D3DSTENCILCAPS_TWOSIDED)
	{
		m_pCapabilities->setCapability(RC_TWO_SIDED_STENCIL);
	}

	// stencil wrap
	if ((m_caps.StencilCaps & D3DSTENCILCAPS_INCR) &&  (m_caps.StencilCaps & D3DSTENCILCAPS_DECR))
	{
		m_pCapabilities->setCapability(RC_STENCIL_WRAP);
	}

	// Check for hardware occlusion support
	if ((pd3dDevice->CreateQuery(D3DQUERYTYPE_OCCLUSION,  NULL ) ) == D3D_OK)	
	{
		m_pCapabilities->setCapability(RC_HWOCCLUSION);
	}

	convertVertexShaderCaps();
	convertPixelShaderCaps();

	// User clip planes
	if (m_caps.MaxUserClipPlanes > 0)
	{
		m_pCapabilities->setCapability(RC_USER_CLIP_PLANES);
	}

	// UBYTE4 type?
	if (m_caps.DeclTypes & D3DDTCAPS_UBYTE4)
	{
		m_pCapabilities->setCapability(RC_VERTEX_FORMAT_UBYTE4);
	}

	// Adapter details	
	D3DADAPTER_IDENTIFIER9	adapterID;
	m_pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &adapterID);

	/* 
		Infinite projection?
		We have no capability for this, so we have to base this on our
		experience and reports from users
		Non-vertex program capable hardware does not appear to support it
	*/
	if (m_pCapabilities->hasCapability(RC_VERTEX_PROGRAM))
	{
		/*
			GeForce4 Ti (and presumably GeForce3) does not
			render infinite projection properly, even though it does in GL
			So exclude all cards prior to the FX range from doing infinite
		*/
		if (adapterID.VendorId != 0x10DE ||										// not nVidia
			!((adapterID.DeviceId >= 0x200 && adapterID.DeviceId <= 0x20F) ||	// gf3
			(adapterID.DeviceId >= 0x250 && adapterID.DeviceId <= 0x25F) ||		// gf4ti
			(adapterID.DeviceId >= 0x280 && adapterID.DeviceId <= 0x28F) ||		// gf4ti
			(adapterID.DeviceId >= 0x170 && adapterID.DeviceId <= 0x18F) ||		// gf4 go
			(adapterID.DeviceId >= 0x280 && adapterID.DeviceId <= 0x28F)))		// gf4ti go
		{
			m_pCapabilities->setCapability(RC_INFINITE_FAR_PLANE);
		}

	}

	// 3D textures?
	if (m_caps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP)
	{
		m_pCapabilities->setCapability(RC_TEXTURE_3D);
	}

	if ((m_caps.TextureCaps & D3DPTEXTURECAPS_POW2) == 0)
	{
		// unrestricted non POW2
		m_pCapabilities->setCapability(RC_NON_POWER_OF_2_TEXTURES);
	}
	else if (m_caps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL)
	{
		// Conditional support for non POW2
		m_pCapabilities->setCapability(RC_NON_POWER_OF_2_TEXTURES);
		m_pCapabilities->setNonPOW2TexturesLimited(true);
	}

	// We always support rendertextures bigger than the frame buffer
	m_pCapabilities->setCapability(RC_HWRENDER_TO_TEXTURE);

	// Determine if any floating point texture format is supported
	D3DFORMAT floatFormats[6] = {
		D3DFMT_R16F, D3DFMT_G16R16F, 
		D3DFMT_A16B16G16R16F, D3DFMT_R32F, D3DFMT_G32R32F, 
		D3DFMT_A32B32G32R32F
	};

	LPDIRECT3DSURFACE9 bbSurf = ((CPepeEngineDirect3D9RenderWindow* )m_pRenderWindow)->getRenderSurface();	
	D3DSURFACE_DESC bbSurfDesc;
	bbSurf->GetDesc(&bbSurfDesc);

	for (int i = 0; i < 6; ++i)
	{
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(
				D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, bbSurfDesc.Format, 0, D3DRTYPE_TEXTURE, floatFormats[i])
			)
		)
		{
			m_pCapabilities->setCapability(RC_TEXTURE_FLOAT);
			break;
		}

	}

	// Number of render targets
	m_pCapabilities->setNumMultiRenderTargets((unsigned short)m_caps.NumSimultaneousRTs);

	// Point sprites 
	if (m_caps.MaxPointSize > 1.0f)
	{
		m_pCapabilities->setCapability(RC_POINT_SPRITES);
		// sprites and extended parameters go together in D3D
		m_pCapabilities->setCapability(RC_POINT_EXTENDED_PARAMETERS);
		m_pCapabilities->setMaxPointSize(m_caps.MaxPointSize);
	}

	// Vertex textures
	if (m_pGPUProgramManager->isSyntaxSupported(_T("vs_3_0")))
	{
		/*
			Run through all the texture formats looking for any which support
			vertex texture fetching. Must have at least one!
			All ATI Radeon up to X1n00 say they support vs_3_0, 
			but they support no texture formats for vertex texture fetch (cheaters!)
		*/
		if (checkVertexTextureFormats())
		{
			m_pCapabilities->setCapability(RC_VERTEX_TEXTURE_FETCH);
			// always 4 vertex texture units in vs_3_0, and never shared
			m_pCapabilities->setNumVertexTextureUnits(4);
			m_pCapabilities->setVertexTextureUnitsShared(false);

		}
	}

	// Mipmap LOD biasing?
	if (m_caps.RasterCaps & D3DPRASTERCAPS_MIPMAPLODBIAS)
	{
		m_pCapabilities->setCapability(RC_MIPMAP_LOD_BIAS);
	}
	
	/*
		Do we support per-stage src_manual constants?
		HACK - ATI drivers seem to be buggy and don't support per-stage constants properly?
	*/
	m_bPerStageConstantSupport = (m_caps.PrimitiveMiscCaps & D3DPMISCCAPS_PERSTAGECONSTANT) != 0;

	m_pCapabilities->log();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::initConfigOptions()
{
	IPepeEngineRenderer::initConfigOptions();

	CPepeEngineDirect3D9VideoMode*	videoMode;
	CPepeEngineConfig*				pConfig		= CPepeEngineCore::getSingleton().getConfig();
	ConfigOptionValue*				videoModes	= pConfig->getOption(_T("Video Mode"));
	
	videoModes->possibleValues.clear();
	
	for (unsigned int k = 0; k < m_pVideoModeList->count(); k++)
	{
		videoMode = static_cast<CPepeEngineDirect3D9VideoMode*>(m_pVideoModeList->item(k));
		videoModes->possibleValues.push_back(videoMode->getDescription());
	}

	// Reset video mode to default if previous doesn't avail in new possible values
	pConfig->setOptionValue(_T("Video Mode"), _T("800 x 600 @ 32-bit colour"), false);

	refreshFSAAOptions();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::refreshFSAAOptions()
{	
	CPepeEngineDirect3D9VideoMode*	videoMode;
	CPepeEngineConfig*				pConfig		= CPepeEngineCore::getSingleton().getConfig();
	ConfigOptionValue*				optFSAA		= pConfig->getOption(_T("Anti-aliasing"));

	videoMode = static_cast<CPepeEngineDirect3D9VideoMode*>(m_pVideoModeList->item(pConfig->getOption(_T("Video Mode"))->strValue));

	if (videoMode)
	{
		optFSAA->possibleValues.clear();
		optFSAA->possibleValues.push_back(_T("none"));
		// get non maskable FSAA for this VMODE
		unsigned long numLevels = 0;

		bool bOK = this->checkMultiSampleQuality(
			D3DMULTISAMPLE_NONMASKABLE, 
			&numLevels, 
			videoMode->getFormat(), 
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			true
		);

		if (bOK && numLevels > 0)
		{
			for (unsigned long n = 0; n < numLevels; n++)
			{
				optFSAA->possibleValues.push_back(_T("NonMaskable ") + CPepeEngineConverter::parseString((float)n + 1));
			}
		}

		// set maskable levels supported
		for (unsigned int n = 2; n < 17; n++)
		{
			bOK = this->checkMultiSampleQuality(
				(D3DMULTISAMPLE_TYPE)n, 
				&numLevels, 
				videoMode->getFormat(), 
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				true);

			if (bOK)
			{
				optFSAA->possibleValues.push_back(_T("Level ") + CPepeEngineConverter::parseString((float)n));
			}
		}

		D3DMULTISAMPLE_TYPE fsaa	= D3DMULTISAMPLE_NONE;
		DWORD				level = 0;

		if (CPepeEngineStringUtil::startsWith(optFSAA->strValue, _T("NonMaskable"), false))
		{
			fsaa			= D3DMULTISAMPLE_NONMASKABLE;
			size_t pos		= optFSAA->strValue.find_last_of(_T(" "));
			tstring sNum	= optFSAA->strValue.substr(pos + 1);
			level			= CPepeEngineConverter::parseInt(sNum);
			level			-= 1;
		}
		else if (CPepeEngineStringUtil::startsWith(optFSAA->strValue, _T("Level"), false))
		{
			size_t pos		= optFSAA->strValue.find_last_of(_T(" "));
			tstring sNum	= optFSAA->strValue.substr(pos + 1);
			fsaa			= (D3DMULTISAMPLE_TYPE)CPepeEngineConverter::parseInt(sNum);
		}

		if (m_pRenderWindow)
		{
			static_cast<CPepeEngineDirect3D9RenderWindow *>(m_pRenderWindow)->setFSAA(fsaa, level);
		}
	}
}

// -----------------------------------------------------------------------------------------
tstring CPepeEngineDirect3D9Renderer::validateOptions()
{
	tstring err = IPepeEngineRenderer::validateOptions();
		
	if (err.empty()) return _T("");

	return err;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::drawText(const tstring& strText, size_t xPos, size_t yPos)
{
	RECT rc;
	rc.left		= xPos;
	rc.top		= yPos;
	rc.right	= m_pRenderWindow->getWindow()->getWidth();
	rc.bottom	= m_pRenderWindow->getWindow()->getHeight();
	m_pFont->DrawText(
		NULL, strText.c_str(), -1, &rc, DT_LEFT | DT_WORDBREAK | DT_EXPANDTABS,
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) 
	);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::notifyDeviceLost()
{
	m_bDeviceLost = true;
	if (m_pFont)
	{
		m_pFont->OnLostDevice();
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::restoreLostDevice()
{
	LPDIRECT3DDEVICE9 pd3dDevice = getD3DDevice();
	// Release all non-managed resources

	// same in shutdown()!!

	// Cleanup depth stencils
	//_cleanupDepthStencils();

	// Set all texture units to nothing
	
	//_disableTextureUnitsFrom(0);

	
	// Unbind any vertex streams
	/*
	for (size_t i = 0; i < mLastVertexSourceCount; ++i)
	{
		pd3dDevice->SetStreamSource(i, NULL, 0, 0);
	}
	mLastVertexSourceCount = 0;*/
	pd3dDevice->SetStreamSource(0, NULL, 0, 0);

	// We have to deal with non-managed textures and vertex buffers
	// GPU programs don't have to be restored
	static_cast<CPepeEngineDirect3D9TextureManager*>(m_pTextureManager)->releaseDefaultPoolResources();
	static_cast<CPepeEngineDirect3D9BufferManager*>(m_pBufferManager)->releaseDefaultPoolResources();
	
	SAFE_RELEASE(m_pFont);

	D3DPRESENT_PARAMETERS* pd3dpp = static_cast<CPepeEngineDirect3D9RenderWindow*>(m_pRenderWindow)->getPresentationParameters();
	// Reset the device, using the primary window presentation params
	HRESULT hr = pd3dDevice->Reset(pd3dpp);
	
	
	if (hr == D3DERR_DEVICELOST)
	{
		// Don't continue
		return;
	} else if (FAILED(hr))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Cannot reset device: ") + msg, 
			_T("CPepeEngineDirect3D9Renderer::restoreLostDevice")
		);		
	}

	/* 
		If windowed, we have to reset the size here
		since a FullScreen switch may have occurred
	*/	
	if (m_pRenderWindow->getWindow()->isSwitchingFullScreen())
	{
		m_pRenderWindow->getWindow()->finishSwitchingFullScreen(pd3dpp->BackBufferWidth, pd3dpp->BackBufferHeight);
	}

	// Recreate all non-managed resources
	static_cast<CPepeEngineDirect3D9TextureManager*>(m_pTextureManager)->recreateDefaultPoolResources();
	static_cast<CPepeEngineDirect3D9BufferManager*>(m_pBufferManager)->recreateDefaultPoolResources();

	createSystemFont();
	restoreRenderState();

	m_bDeviceLost = false;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Renderer::notify(const CPepeEngineEvent& event)
{
	switch (event.getEventType())
	{
	case EVENT_CONFIG_CHANGED:
		refreshFSAAOptions();
		break;
	}

}

// -----------------------------------------------------------------------------------------
LPDIRECT3DDEVICE9 CPepeEngineDirect3D9Renderer::getD3DDevice() const
{ 
	return static_cast<CPepeEngineDirect3D9RenderWindow *>(m_pRenderWindow)->getD3DDevice();
}

_PEPE_ENGINE_END