#include "CPepeEngineCore.h"
#include "CPepeEngineWin32Plugin.h"

_PEPE_ENGINE_START

const tstring strPluginName = _T("Win32 Operating System");

// -----------------------------------------------------------------------------------------
CPepeEngineWin32Plugin::CPepeEngineWin32Plugin()
    : m_pWindow(0), m_pTimer(0)
{

}

// -----------------------------------------------------------------------------------------
const tstring& CPepeEngineWin32Plugin::getName() const
{
    return strPluginName;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWin32Plugin::install()
{
#ifdef PEPE_ENGINE_STATIC_LIB
    HINSTANCE hInst = GetModuleHandle(NULL);
#else
    HINSTANCE hInst = GetModuleHandle(_T("OS_Win32.dll"));
#endif

    m_pWindow       = new CPepeEngineWin32Window(hInst);
    m_pTimer        = new CPepeEngineWin32Timer();
    m_pConfigDialog = new CPepeEngineWin32ConfigDialog();

    CPepeEngineCore::getSingleton().addWindow(m_pWindow);
    CPepeEngineCore::getSingleton().addTimer(m_pTimer);
    CPepeEngineCore::getSingleton().addConfigDialog(m_pConfigDialog);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWin32Plugin::initialise()
{
    // nothing to do
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWin32Plugin::shutdown()
{
    // nothing to do
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWin32Plugin::uninstall()
{
    delete m_pWindow;
    m_pWindow = 0;

    delete m_pTimer;
    m_pTimer = 0;

    delete m_pConfigDialog;
    m_pConfigDialog = 0;
}

_PEPE_ENGINE_END