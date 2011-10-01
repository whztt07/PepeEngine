#include "CPepeEngineConfig.h"
#include "CPepeEngineCore.h"
#include "CPepeEngineWin32Window.h"
#include "IPepeEngineRenderWindow.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
LRESULT CALLBACK CPepeEngineWin32Window::_wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE) {
        // Store pointer to Win32Window in user data area
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)(((LPCREATESTRUCT)lParam)->lpCreateParams));
        return 0;
    }

    IPepeEngineWindow* win = (IPepeEngineWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    if (!win) {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    switch (uMsg) {
    case WM_SYSKEYDOWN:
        switch( wParam ) {
        case VK_CONTROL:
        case VK_SHIFT:
        case VK_MENU: //ALT
            //return zero to bypass _wndProc and signal we processed the message
            return 0;
        }

        break;

    case WM_SYSKEYUP:
        switch( wParam ) {
        case VK_CONTROL:
        case VK_SHIFT:
        case VK_MENU: //ALT
        case VK_F10:
            //return zero to bypass _wndProc and signal we processed the message
            return 0;
        }

        break;

    case WM_SYSCHAR:

        // return zero to bypass _wndProc and signal we processed the message, unless it's an ALT-space
        if (wParam != VK_SPACE) {
            return 0;
        }

        break;

    case WM_MOVE:
        win->windowMovedOrResized();
        break;

    case WM_DISPLAYCHANGE:
        win->windowMovedOrResized();
        break;

    case WM_SIZE:
        win->windowMovedOrResized();
        break;

    case WM_GETMINMAXINFO:
        // Prevent the window from going smaller than some minimum size
        ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
        ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
        break;

    case WM_CLOSE:
        win->destroy();
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// -----------------------------------------------------------------------------------------
CPepeEngineWin32Window::CPepeEngineWin32Window(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
    m_hWnd      = 0;

    initConfigOptions();
}

// -----------------------------------------------------------------------------------------
CPepeEngineWin32Window::~CPepeEngineWin32Window()
{
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWin32Window::create(
    const tstring& strName,
    size_t nWidth, size_t nHeight,
    bool bFullScreen
)
{
    if (m_hWnd) {
        destroy();
    }

    // ------------------ set config options ----------------------------------------
    m_bFullScreen       = bFullScreen;
    m_strBorder         = CPepeEngineCore::getSingleton().getConfig()->getOption(_T("border"))->strValue;
    m_nWidth            = nWidth;
    m_nHeight           = nHeight;
    m_iLeft             = -1;       // Defaults to screen center
    m_iTop              = -1;       // Defaults to screen center
    m_strWindowName     = strName;
    //end --------------- set config options ----------------------------------------

    DWORD   dwStyle = WS_VISIBLE | WS_CLIPCHILDREN;
    RECT    rc;

    if (!m_bFullScreen) {
        if (m_strBorder == _T("none")) {
            dwStyle |= WS_POPUP;
        } else if (m_strBorder == _T("fixed")) {
            dwStyle |= WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
        } else {
            dwStyle |= WS_OVERLAPPEDWINDOW;
        }

        // Calculate window dimensions required
        // to get the requested client area
        SetRect(&rc, 0, 0, m_nWidth, m_nHeight);
        AdjustWindowRect(&rc, dwStyle, false);
        m_nWidth    = rc.right - rc.left;
        m_nHeight   = rc.bottom - rc.top;

        // Clamp width and height to the desktop dimensions
        int screenw = GetSystemMetrics(SM_CXSCREEN);
        int screenh = GetSystemMetrics(SM_CYSCREEN);

        if ((int)m_nWidth > screenw) {
            m_nWidth = screenw;
        }

        if ((int)m_nHeight > screenh) {
            m_nHeight = screenh;
        }

        if (m_iLeft < 0) {
            m_iLeft = (screenw - m_nWidth) / 2;
        }

        if (m_iTop < 0) {
            m_iTop = (screenh - m_nHeight) / 2;
        }
    } else {
        dwStyle |=  WS_POPUP;
        m_iTop = m_iLeft = 0;
    }

    tstring strClassName = _T("PepeEngineWnd");

    // Register the window class
    WNDCLASS wc = {
        0, CPepeEngineWin32Window::_wndProc, 0, 0, m_hInstance,
        LoadIcon(0, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)GetStockObject(BLACK_BRUSH), 0, strClassName.c_str()
    };
    RegisterClass(&wc);

    // Create the application's window
    m_hWnd = CreateWindow(
                 strClassName.c_str(), strName.c_str(), dwStyle,
                 m_iLeft, m_iTop, m_nWidth, m_nHeight, 0, 0, m_hInstance, this
             );

    GetWindowRect(m_hWnd, &rc);
    m_iTop  = rc.top;
    m_iLeft = rc.left;

    GetClientRect(m_hWnd, &rc);
    m_nWidth    = rc.right;
    m_nHeight   = rc.bottom;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWin32Window::destroy()
{
    DestroyWindow(m_hWnd);
    m_hWnd = 0;

    tstring strClassName = _T("PepeEngineWnd");
    UnregisterClass(strClassName.c_str(), m_hInstance);
    m_hInstance = 0;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineWin32Window::run() const
{
    // Custom main loop
    bool quit = false;
    MSG  msg;

    msg.message = WM_NULL;

    // Use PeekMessage() so we can use idle time to render the scene
    while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
        // Translate and dispatch the message
        if (0 == TranslateAccelerator(m_hWnd, NULL, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT) {
            quit = true;
        }
    }

    return !quit;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWin32Window::resize(size_t nWidth, size_t nHeight)
{
    if (m_hWnd && !m_bFullScreen) {
        RECT rc = {0, 0, nWidth, nHeight};
        AdjustWindowRect(&rc, GetWindowLong(m_hWnd, GWL_STYLE), false);
        nWidth  = rc.right - rc.left;
        nHeight = rc.bottom - rc.top;
        SetWindowPos(m_hWnd, 0, 0, 0, nWidth, nHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWin32Window::reposition(size_t nPosX, size_t nPosY)
{
    if (m_hWnd && !m_bFullScreen) {
        SetWindowPos(m_hWnd, 0, nPosX, nPosY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWin32Window::windowMovedOrResized()
{
    if (!m_hWnd || IsIconic(m_hWnd)) {
        return;
    }

    RECT rc;
    // top and left represent outer window position
    GetWindowRect(m_hWnd, &rc);
    m_iTop  = rc.top;
    m_iLeft = rc.left;

    // width and height represent drawable area only
    GetClientRect(m_hWnd, &rc);
    unsigned int width  = rc.right;
    unsigned int height = rc.bottom;

    if (m_nWidth == width && m_nHeight == height) {
        return;
    }

    m_nWidth    = width;
    m_nHeight   = height;

    CPepeEngineCore::getSingleton().getRenderer()->getRenderWindow()->notifyWindowChange();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWin32Window::setFullScreen(bool bFullScreen, size_t nWidth, size_t nHeight)
{
    if (bFullScreen != m_bFullScreen || nWidth != m_nWidth || nHeight != m_nHeight) {
        if (bFullScreen != m_bFullScreen) {
            m_bSwitchingFullScreen = true;
        }

        DWORD dwStyle = WS_VISIBLE | WS_CLIPCHILDREN;

        bool bOldFullScreen = m_bFullScreen;
        m_bFullScreen       = bFullScreen;

        if (bFullScreen) {
            dwStyle |= WS_POPUP;
            m_iTop      = m_iLeft = 0;
            m_nWidth    = nWidth;
            m_nHeight   = nHeight;

            // need different ordering here
            if (bOldFullScreen) {
                // was previously bFullScreen, just changing the resolution
                SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, nWidth, nHeight, SWP_NOACTIVATE);
            } else {
                SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, nWidth, nHeight, SWP_NOACTIVATE);
                //MoveWindow(m_hWnd, mLeft, mTop, m_nWidth, m_nHeight, FALSE);
                SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
                SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
            }
        } else {
            if (m_strBorder == _T("none")) {
                dwStyle |= WS_POPUP;
            } else if (m_strBorder == _T("fixed")) {
                dwStyle |= WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
            } else {
                dwStyle |= WS_OVERLAPPEDWINDOW;
            }

            // Calculate window dimensions required
            // to get the requested client area
            RECT rc;
            SetRect(&rc, 0, 0, nWidth, nHeight);
            AdjustWindowRect(&rc, dwStyle, false);
            unsigned int winWidth   = rc.right - rc.left;
            unsigned int winHeight  = rc.bottom - rc.top;

            SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
            SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, winWidth, winHeight,
                         SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOACTIVATE);
            // Note that we also set the position via finishSwitchingFullScreen
        }

        CPepeEngineCore::getSingleton().getRenderer()->getRenderWindow()->notifySwitchingFullScreen(
            bOldFullScreen, bFullScreen, nWidth, nHeight
        );
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWin32Window::finishSwitchingFullScreen(size_t nWidth, size_t nHeight)
{
    if (m_bFullScreen) {
        // Need to reset the region on the window sometimes, when the
        // windowed mode was constrained by desktop
        HRGN hRgn = CreateRectRgn(0, 0, nWidth, nHeight);
        SetWindowRgn(m_hWnd, hRgn, FALSE);
    } else {
        // When switching back to windowed mode, need to reset window size
        // after device has been restored
        RECT rc;
        SetRect(&rc, 0, 0, nWidth, nHeight);
        AdjustWindowRect(&rc, GetWindowLong(m_hWnd, GWL_STYLE), false);
        unsigned int winWidth   = rc.right - rc.left;
        unsigned int winHeight  = rc.bottom - rc.top;
        int screenw             = GetSystemMetrics(SM_CXSCREEN);
        int screenh             = GetSystemMetrics(SM_CYSCREEN);

        if ((int)winWidth > screenw) {
            winWidth = screenw;
        }

        if ((int)winHeight > screenh) {
            winHeight = screenh;
        }

        int left    = (screenw - winWidth) / 2;
        int top     = (screenh - winHeight) / 2;

        SetWindowPos(m_hWnd, HWND_NOTOPMOST, left, top, winWidth, winHeight,
                     SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOACTIVATE);

    }

    m_bSwitchingFullScreen = false;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWin32Window::setWindowTitle(const tstring& strName) const
{
    SetWindowText(m_hWnd, (LPCTSTR)strName.c_str());
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWin32Window::getCustomAttribute(const tstring& strParam, void* pData) const
{
    if (strParam == _T("WINDOW")) {
        HWND *pHwnd = (HWND*)pData;
        *pHwnd      = getWindowHandle();
        return;
    } else if (strParam == _T("INSTANCE")) {
        HINSTANCE *phInstance   = (HINSTANCE*)pData;
        *phInstance             = getInstance();
        return;
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWin32Window::initConfigOptions()
{
    IPepeEngineWindow::initConfigOptions();

    CPepeEngineConfig* pConfig = CPepeEngineCore::getSingleton().getConfig();

    ConfigOptionValue* border = new ConfigOptionValue;
    border->type        = OPERATING_SYSTEM_OPTION;
    border->strDefValue = _T("fixed");
    border->strValue    = _T("fixed");
    border->immutable   = false;
    border->possibleValues.push_back(_T("none"));
    border->possibleValues.push_back(_T("fixed"));
    pConfig->addOption(_T("border"), border);
}

_PEPE_ENGINE_END