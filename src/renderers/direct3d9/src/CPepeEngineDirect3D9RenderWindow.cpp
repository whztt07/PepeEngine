#include "CPepeEngineConverter.h"
#include "CPepeEngineCore.h"
#include "CPepeEngineDirect3D9Renderer.h"
#include "CPepeEngineDirect3D9RenderWindow.h"
#include "CPepeEngineException.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineDirect3D9RenderWindow::CPepeEngineDirect3D9RenderWindow(
	IPepeEngineWindow* pWindow, LPDIRECT3D9 pD3D
) : IPepeEngineRenderWindow(pWindow)
{
	#if PEPE_ENGINE_PLATFORM == PEPE_ENGINE_PLATFORM_WIN32
		size_t windowHnd		= 0;	
		size_t windowInstance	= 0;

		pWindow->getCustomAttribute(_T("WINDOW"), &windowHnd);
		pWindow->getCustomAttribute(_T("INSTANCE"), &windowInstance);
		m_hWnd		= (HWND)windowHnd;
		m_hInstance	= (HINSTANCE)windowInstance;
	#else
	#	error direct3d9 doesn't supported on this operating system
	#endif

	m_pD3D				= pD3D;
	m_pD3DDevice		= NULL;
	m_pRenderSurface	= NULL;
	m_pRenderZSurface	= NULL;
	m_nFSAAType			= D3DMULTISAMPLE_NONE;
	m_nFSAAQualityLevel	= 0;	
}

// -----------------------------------------------------------------------------------------
CPepeEngineDirect3D9RenderWindow::~CPepeEngineDirect3D9RenderWindow()
{
	SAFE_RELEASE(m_pD3DDevice);
	SAFE_RELEASE(m_pRenderSurface);

	m_pRenderZSurface = NULL;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9RenderWindow::create()
{	
	CPepeEngineConfig* pConfig = CPepeEngineCore::getSingleton().getConfig();

	IPepeEngineVideoMode* pVideoMode = CPepeEngineCore::getSingleton().getRenderer()->getVideoModeList()->item(
		pConfig->getOption(_T("Video Mode"))->strValue
	);

	// ------------------ set config options ----------------------------------------
	m_bVSync			= CPepeEngineConverter::parseBool(pConfig->getOption(_T("vsync"))->strValue);
	m_bDepthBuffer		= CPepeEngineConverter::parseBool(pConfig->getOption(_T("Depth Buffer"))->strValue);

	m_nColorDepth		= pVideoMode->getColourDepth();	
	m_nDisplayFrequency	= pVideoMode->getRefreshRate();				
	//end --------------- set config options ----------------------------------------

	HRESULT			hr;
	D3DDISPLAYMODE	d3ddm;
		
	SAFE_RELEASE(m_pRenderSurface);

	if (FAILED(hr = m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Failed to get adapter display mode: ") + msg, 
			_T("CPepeEngineDirect3D9RenderWindow::create")
		);
	}
	
	ZeroMemory(&m_d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	m_d3dpp.BackBufferCount			= m_bVSync ? 2 : 1;
	m_d3dpp.EnableAutoDepthStencil	= m_bDepthBuffer;
	if (m_bVSync)
	{
		m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}
	else
	{
		m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	m_d3dpp.Windowed					= !m_pWindow->isFullScreen();
	m_d3dpp.BackBufferWidth				= m_pWindow->getWidth();
	m_d3dpp.BackBufferHeight			= m_pWindow->getHeight();
	m_d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	m_d3dpp.hDeviceWindow				= m_hWnd;
	m_d3dpp.FullScreen_RefreshRateInHz	= m_pWindow->isFullScreen() ? m_nDisplayFrequency : 0;
	
	if (m_nColorDepth > 16)
	{
		m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	} else
	{
		m_d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
	}		
	
	if (m_nColorDepth > 16)
	{
		// Try to create a 32-bit depth, 8-bit stencil
		if (FAILED(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL, m_d3dpp.BackBufferFormat, D3DUSAGE_DEPTHSTENCIL, 
			D3DRTYPE_SURFACE, D3DFMT_D24S8)))
		{
			// Bugger, no 8-bit hardware stencil, just try 32-bit zbuffer 
			if (FAILED(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL, m_d3dpp.BackBufferFormat, D3DUSAGE_DEPTHSTENCIL, 
				D3DRTYPE_SURFACE, D3DFMT_D32)))
			{
				// Jeez, what a naff card. Fall back on 16-bit depth buffering
				m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
			}
			else
			{
				m_d3dpp.AutoDepthStencilFormat = D3DFMT_D32;
			}
		}
		else
		{			
			if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
				m_d3dpp.BackBufferFormat, m_d3dpp.BackBufferFormat, D3DFMT_D24S8)))
			{
				m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; 
			} 
			else 
			{
				m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8; 
			}
		}
	}
	else
	{
		// 16-bit depth, software stencil
		m_d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
	}

	m_d3dpp.MultiSampleType		= (D3DMULTISAMPLE_TYPE)m_nFSAAType;
	m_d3dpp.MultiSampleQuality	= (m_nFSAAQualityLevel == 0) ? 0 : m_nFSAAQualityLevel;
	
	DWORD extraFlags = 0;
	
	hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | extraFlags, &m_d3dpp, &m_pD3DDevice
	);

	if (FAILED(hr))
	{		
		hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING | extraFlags, &m_d3dpp, &m_pD3DDevice);
	}

	if (FAILED(hr))
	{
		hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING | extraFlags, &m_d3dpp, &m_pD3DDevice);
		if (FAILED(hr))
		{
			hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING | extraFlags, &m_d3dpp, &m_pD3DDevice);
		}
	}
	
	if (FAILED(hr))
	{		
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Failed to create Direct3D 9 device: ") + msg, 
			_T("CPepeEngineDirect3D9RenderWindow::create")
		);
	}

	m_pD3DDevice->GetRenderTarget(0, &m_pRenderSurface);
	m_pD3DDevice->GetDepthStencilSurface(&m_pRenderZSurface);	
	if (m_pRenderZSurface)
	{
		m_pRenderZSurface->Release();
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9RenderWindow::swapBuffers()
{	
	HRESULT hr;
	
	hr = m_pD3DDevice->Present(NULL, NULL, 0, NULL);
	
	if (D3DERR_DEVICELOST == hr)
	{
		SAFE_RELEASE(m_pRenderSurface);
		static_cast<CPepeEngineDirect3D9Renderer*>(
			CPepeEngineCore::getSingleton().getRenderer()
		)->notifyDeviceLost();
	}
	else if (FAILED(hr))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Error presenting surfaces: ") + msg, 
			_T("CPepeEngineDirect3D9RenderWindow::swapBuffers")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9RenderWindow::update()
{

	CPepeEngineDirect3D9Renderer* renderer = static_cast<CPepeEngineDirect3D9Renderer*>(
		CPepeEngineCore::getSingleton().getRenderer());

	if (renderer->isDeviceLost())
	{
		// Test the cooperative mode first
		HRESULT hr = m_pD3DDevice->TestCooperativeLevel();
		if (hr == D3DERR_DEVICELOST)
		{
			// Device lost, and we can't reset
			// can't do anything about it here, wait until we get 
			// D3DERR_DEVICENOTRESET; rendering calls will silently fail until 
			// then (except Present, but we ignore device lost there too)
			SAFE_RELEASE(m_pRenderSurface);
			// need to release if swap chain
			
			SAFE_RELEASE(m_pRenderZSurface);
			Sleep(50);
			return;
		} else
		{
			// device lost, and we can reset
			renderer->restoreLostDevice();

			// Still lost?
			if (renderer->isDeviceLost())
			{
				// Wait a while
				Sleep(50);
				return;
			}

			// re-qeuery buffers
			m_pD3DDevice->GetRenderTarget(0, &m_pRenderSurface);
			m_pD3DDevice->GetDepthStencilSurface(&m_pRenderZSurface);
			// release immediately so we don't hog them
			m_pRenderZSurface->Release();		
		}

	}	
	
	swapBuffers();
	renderer->resetFacesCount();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9RenderWindow::notifyWindowChange()
{
	SAFE_RELEASE(m_pRenderSurface);

	m_d3dpp.BackBufferWidth		= m_pWindow->getWidth();
	m_d3dpp.BackBufferHeight	= m_pWindow->getHeight();
	
	static_cast<CPepeEngineDirect3D9Renderer*>(
		CPepeEngineCore::getSingleton().getRenderer()
	)->notifyDeviceLost();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9RenderWindow::notifySwitchingFullScreen(
	bool bOldFullScreen, bool bFullScreen, 
	size_t nWidth, size_t nHeight
)
{		
	if (!bFullScreen)
	{
		m_d3dpp.FullScreen_RefreshRateInHz = 0;
	} else
	{
		IPepeEngineVideoMode* pVideoMode = CPepeEngineCore::getSingleton().getRenderer()->getVideoModeList()->item(
			CPepeEngineCore::getSingleton().getConfig()->getOption(_T("Video Mode"))->strValue
		);
		m_d3dpp.FullScreen_RefreshRateInHz = pVideoMode->getRefreshRate();
	}
	m_d3dpp.Windowed			= !bFullScreen;
	m_d3dpp.BackBufferWidth		= nWidth;
	m_d3dpp.BackBufferHeight	= nHeight;
	
	SAFE_RELEASE(m_pRenderSurface);
	static_cast<CPepeEngineDirect3D9Renderer*>(
		CPepeEngineCore::getSingleton().getRenderer()
	)->notifyDeviceLost();
}

_PEPE_ENGINE_END