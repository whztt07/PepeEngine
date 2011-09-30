/**
 * Project: PepeEngine
 * Tier:	Backend 
 * File:	CPepeEngineDirect3D9RenderWindow.h 
 *
 * @class	CPepeEngineDirect3D9RenderWindow
 *
 * @brief	Declaration of CPepeEngineDirect3D9RenderWindow class. 
 *			This is renderer using Direct3D 9 library.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-10-15
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEDIRECT3D9RENDERWINDOW_H
#define CPEPEENGINEDIRECT3D9RENDERWINDOW_H

#include "PepeEngineDirect3D9Init.h"

#if PEPE_ENGINE_PLATFORM == PEPE_ENGINE_PLATFORM_WIN32
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#	endif
#	include <windows.h>
#else
#	error direct3d9 doesn't supported on this operating system
#endif


#include "IPepeEngineWindow.h"
#include "IPepeEngineRenderWindow.h"

_PEPE_ENGINE_START

class CPepeEngineDirect3D9RenderWindow : public IPepeEngineRenderWindow
{		
	D3DPRESENT_PARAMETERS	m_d3dpp;
	LPDIRECT3DDEVICE9		m_pD3DDevice;
	LPDIRECT3D9				m_pD3D;
	LPDIRECT3DSURFACE9		m_pRenderSurface;
	LPDIRECT3DSURFACE9		m_pRenderZSurface;

	D3DMULTISAMPLE_TYPE		m_nFSAAType;
	unsigned long			m_nFSAAQualityLevel;

	#if PEPE_ENGINE_PLATFORM == PEPE_ENGINE_PLATFORM_WIN32
		HWND					m_hWnd;
		HINSTANCE				m_hInstance;
	#else
	#	error direct3d9 doesn't supported on this operating system
	#endif
public:

	CPepeEngineDirect3D9RenderWindow(IPepeEngineWindow* pWindow, LPDIRECT3D9 pD3D);
	~CPepeEngineDirect3D9RenderWindow();

	void create();
		
	void swapBuffers(); 
	void update();

	void notifyWindowChange();
	void notifySwitchingFullScreen(
		bool bOldFullScreen, bool bFullScreen, 
		size_t nWidth, size_t nHeight
	);

	LPDIRECT3DDEVICE9 getD3DDevice() const {return m_pD3DDevice;}
	LPDIRECT3DSURFACE9 getRenderSurface() const {return m_pRenderSurface;}				
	D3DPRESENT_PARAMETERS* getPresentationParameters(void) {return &m_d3dpp;}	
	void setFSAA(D3DMULTISAMPLE_TYPE type, unsigned long lQualityLevel)
	{		
		m_nFSAAType			= type;
		m_nFSAAQualityLevel	= (size_t)lQualityLevel;	
	}
};

_PEPE_ENGINE_END

#endif