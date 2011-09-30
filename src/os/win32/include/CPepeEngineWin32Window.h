/**
 * Project: PepeEngine
 * Tier:	Backend 
 * File:	CPepeEngineWin32Window.h   
 *
 * @brief	Declaration of CPepeEngineWin32Window class. 
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-10-21
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEWIN32WINDOW_H
#define CPEPEENGINEWIN32WINDOW_H

#include "PepeEngineWin32Init.h"

#include "IPepeEngineWindow.h"

_PEPE_ENGINE_START

class _PepeEngineWin32Export CPepeEngineWin32Window : public IPepeEngineWindow
{	
	HWND					m_hWnd;				/**<	Win32 Window handle	*/
	HINSTANCE				m_hInstance;


	static LRESULT CALLBACK _wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:

	CPepeEngineWin32Window(HINSTANCE hInstance);
	~CPepeEngineWin32Window();

	void create(
		const tstring& strName, 
		size_t nWidth, size_t nHeight,	
		bool bFullScreen
	);

	void destroy();
	bool run() const;
	void resize(size_t nWidth, size_t nHeight);
	void reposition(size_t nPosX, size_t nPosY);
	void windowMovedOrResized();
	void setFullScreen(bool bFullScreen, size_t nWidth, size_t nHeight);
	void finishSwitchingFullScreen(size_t nWidth, size_t nHeight);

	HWND getWindowHandle() const {return m_hWnd;}
	HINSTANCE getInstance() const {return m_hInstance;}

	void setWindowTitle(const tstring& strName) const;
	
	void getCustomAttribute(const tstring& strParam, void* pData) const;

	void initConfigOptions();
};

_PEPE_ENGINE_END

#endif