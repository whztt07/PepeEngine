#include "CDemoOceanApplication.h"

#if defined(DEBUG) || defined(_DEBUG)
#	ifndef _VLD_
#		define _VLD_
#		include <VLD/vld.h>
#	endif
#endif

#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR     lpCmdLine,
				   int       nCmdShow)

{	
	CDemoOceanApplication app;
	try
	{						
		app.run();
	} catch(CPepeEngineException &e)
	{		
		CPepeEngineLogManager::getSingleton().logMessage((LPTSTR)e.what());
		PEPE_ENGINE_SHOW_MESSAGE(e.what());
	}

	return 0;
}