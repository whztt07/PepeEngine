#include "CPepeEngineCore.h"
#include "CPepeEngineWin32Plugin.h"

_PEPE_ENGINE_START

CPepeEngineWin32Plugin* pPlugin;

extern "C" void _PepeEngineWin32Export dllStartPlugin(void) throw()
{
	pPlugin = new CPepeEngineWin32Plugin();
	CPepeEngineCore::getSingleton().installPlugin(pPlugin);
}

extern "C" void _PepeEngineWin32Export dllStopPlugin(void)
{
	CPepeEngineCore::getSingleton().uninstallPlugin(pPlugin);
	delete pPlugin;
}

_PEPE_ENGINE_END