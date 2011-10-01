/**
 * Project: PepeEngine
 * Tier:    Frontend
 * File:    CPepeEngineDynamicLibraryManager.h
 *
 * @brief   Declaration of CPepeEngineDynamicLibraryManager class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-10-29
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEDYNAMICLIBRARYMANAGER_H
#define CPEPEENGINEDYNAMICLIBRARYMANAGER_H

#include "PepeEngineInit.h"

#include "CPepeEngineDynamicLibrary.h"
#include "CPepeEngineSingleton.h"
#include "IPepeEngineResourceManager.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineDynamicLibraryManager
    : public IPepeEngineResourceManager, public CPepeEngineSingleton<CPepeEngineDynamicLibraryManager>
{
public:
    ResourcePtr                                 create(const tstring& strName);

    static CPepeEngineDynamicLibraryManager&    getSingleton(void);
    static CPepeEngineDynamicLibraryManager*    getSingletonPtr(void);
};

_PEPE_ENGINE_END

#endif