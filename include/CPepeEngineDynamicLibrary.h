/**
 * Project:     PepeEngine
 * Tier:        Frontend
 * File:        CPepeEngineDynamicLibrary.h
 *
 * @brief       Declaration of CPepeEngineDynamicLibrary class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-10-29
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version     1.0
 */

#ifndef CPEPEENGINEDYNAMICLIBRARY_H
#define CPEPEENGINEDYNAMICLIBRARY_H

#include "PepeEngineInit.h"

#include "IPepeEngineResource.h"

#if PEPE_ENGINE_PLATFORM == PEPE_ENGINE_PLATFORM_WIN32
#   define DYNLIB_HANDLE hInstance
#   define DYNLIB_LOAD(a) LoadLibrary( a )
#   define DYNLIB_GETSYM(a, b) GetProcAddress(a, b)
#   define DYNLIB_UNLOAD(a) !FreeLibrary(a)

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#else
#   error operating system not supported    
#endif


_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineDynamicLibrary : public IPepeEngineResource
{
public:
    CPepeEngineDynamicLibrary(const tstring& strName);
    ~CPepeEngineDynamicLibrary();

    void                load();
    void                unload();

    const tstring&      getName(void) const {
        return m_strName;
    }
    void*               getSymbol(const tstring& strName) const throw();
protected:

    tstring             dynlibError(void);

    /**
     *  Handle to the loaded library.
     */
    DYNLIB_HANDLE       m_hInst;
};

struct _PepeEngineExport DynamicLibraryPtr : public CPepeEngineSharedPtr<CPepeEngineDynamicLibrary> {
public:
    inline                      DynamicLibraryPtr();
    inline explicit             DynamicLibraryPtr(CPepeEngineDynamicLibrary* r);
    inline                      DynamicLibraryPtr(const DynamicLibraryPtr& r);
    inline                      DynamicLibraryPtr(const ResourcePtr& r);
    inline DynamicLibraryPtr&   operator=(const ResourcePtr& r);
};

_PEPE_ENGINE_END

#endif