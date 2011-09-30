#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineDynamicLibrary.h"

#if PEPE_ENGINE_PLATFORM == PEPE_ENGINE_PLATFORM_WIN32
#	include <windows.h>
#endif

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
tstring CPepeEngineDynamicLibrary::dynlibError(void)
{
	#if PEPE_ENGINE_PLATFORM == PEPE_ENGINE_PLATFORM_WIN32
		LPVOID lpMsgBuf; 
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, 
			NULL, 
			GetLastError(), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR) &lpMsgBuf, 
			0, 
			NULL 
		); 
		tstring ret = _TSTR((char*)lpMsgBuf);
		// Free the buffer.
		LocalFree(lpMsgBuf);
		return ret;
	#else
	#	error operating system not supported	
	#endif
}
		
// -----------------------------------------------------------------------------------------
CPepeEngineDynamicLibrary::CPepeEngineDynamicLibrary(const tstring& strName) : IPepeEngineResource(strName)
{	
	m_hInst		= NULL;
}

// -----------------------------------------------------------------------------------------
CPepeEngineDynamicLibrary::~CPepeEngineDynamicLibrary()
{	
	if (isLoaded())
	{
		unload();
	}
}
	
// -----------------------------------------------------------------------------------------
void CPepeEngineDynamicLibrary::load()
{
	if (!isLoaded())
	{
		LOG("Loading dynamic library: " + getName());
		m_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD(m_strName.c_str());

		if(!m_hInst)
		{
			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_FILE_LOAD_ERROR, 
				_T("Could not load dynamic library ") + m_strName + _T(".  System Error: ") + dynlibError(), 
				_T("CPepeEngineDynamicLibrary::load")
			);					
		} else
		{
			m_bIsLoaded	= true;
			LOG("Dynamic library successfully loaded");
		}
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDynamicLibrary::unload()
{
	if (isLoaded())
	{
		LOG("Unloading dynamic library: " + getName());
        if (DYNLIB_UNLOAD(m_hInst))
		{
            PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_INTERNAL_ERROR, 
				_T("Could not unload dynamic library ") + m_strName + _T(".  System Error: ") + dynlibError(), 
				_T("CPepeEngineDynamicLibrary::unload")
			);	
		} else
		{
			m_bIsLoaded	= false;
			LOG("Dynamic library successfully unloaded");
		}
	}
}
	
// -----------------------------------------------------------------------------------------
void* CPepeEngineDynamicLibrary::getSymbol(const tstring& strName) const throw()
{
	char* name;
	CPepeEngineConverter::getChars(strName, &name);

	void* pSymbol = (void*)DYNLIB_GETSYM(m_hInst, name);
	delete[] name;
	
	return pSymbol;
}

// -----------------------------------------------------------------------------------------
DynamicLibraryPtr::DynamicLibraryPtr() : CPepeEngineSharedPtr<CPepeEngineDynamicLibrary>()
{
}

// -----------------------------------------------------------------------------------------
DynamicLibraryPtr::DynamicLibraryPtr(CPepeEngineDynamicLibrary* r)
	: CPepeEngineSharedPtr<CPepeEngineDynamicLibrary>(r)
{
}

// -----------------------------------------------------------------------------------------
DynamicLibraryPtr::DynamicLibraryPtr(const DynamicLibraryPtr& r) 
	: CPepeEngineSharedPtr<CPepeEngineDynamicLibrary>(r)
{
}

// -----------------------------------------------------------------------------------------
DynamicLibraryPtr::DynamicLibraryPtr(const ResourcePtr& r) : CPepeEngineSharedPtr<CPepeEngineDynamicLibrary>()
{
	ptr = (CPepeEngineDynamicLibrary*)r.data();
	if (ptr) 
	{
		ptr->ref();
	}
}

// -----------------------------------------------------------------------------------------
DynamicLibraryPtr& DynamicLibraryPtr::operator=(const ResourcePtr& r)
{
	if (ptr == (CPepeEngineDynamicLibrary*)r.data()) 
	{
		return *this;
	}
	if (ptr)
	{
		ptr->deref();
	}
	ptr = (CPepeEngineDynamicLibrary*)r.data();
	if (ptr) 
	{
		ptr->ref();
	}
	return *this;
}

_PEPE_ENGINE_END