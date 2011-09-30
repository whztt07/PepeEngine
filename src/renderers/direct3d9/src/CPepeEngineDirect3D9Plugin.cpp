#include "CPepeEngineCore.h"
#include "CPepeEngineDirect3D9Plugin.h"

_PEPE_ENGINE_START

const tstring strPluginName = _T("Direct3D 9 Renderer");

// -----------------------------------------------------------------------------------------
CPepeEngineDirect3D9Plugin::CPepeEngineDirect3D9Plugin()
: m_pRenderer(0)
{

}

// -----------------------------------------------------------------------------------------
const tstring& CPepeEngineDirect3D9Plugin::getName() const
{
	return strPluginName;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Plugin::install()
{
	// Create the DirectX 9 rendering api
	#ifdef PEPE_ENGINE_STATIC_LIB
		HINSTANCE hInst = GetModuleHandle( NULL );
	#else
		HINSTANCE hInst = GetModuleHandle(_T("Renderer_Direct3D9.dll"));
	#endif

	m_pRenderer = new CPepeEngineDirect3D9Renderer();

	// Register the render system
	CPepeEngineCore::getSingleton().addRenderer(m_pRenderer);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Plugin::initialise()
{
	// nothing to do
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Plugin::shutdown()
{
	// nothing to do
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Plugin::uninstall()
{
	delete m_pRenderer;
	m_pRenderer = 0;
}

_PEPE_ENGINE_END