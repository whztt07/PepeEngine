#include "CPepeEngineCore.h"
#include "CPepeEngineDirect3D9Plugin.h"

_PEPE_ENGINE_START

CPepeEngineDirect3D9Plugin* pPlugin;

extern "C" void _PepeEngineD3D9Export dllStartPlugin(void) throw()
{
    pPlugin = new CPepeEngineDirect3D9Plugin();
    CPepeEngineCore::getSingleton().installPlugin(pPlugin);
}

extern "C" void _PepeEngineD3D9Export dllStopPlugin(void)
{
    CPepeEngineCore::getSingleton().uninstallPlugin(pPlugin);
    delete pPlugin;
}

_PEPE_ENGINE_END