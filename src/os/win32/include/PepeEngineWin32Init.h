/**
 * Project: PepeEngine
 * Tier:    backend
 * File:    PepeEngineWin32Init.h
 *
 * @brief   Declaration of constant values, macros etc. used by PepeEngine in Windows.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-05-23
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef PEPEENGINEWIN32INIT_H
#define PEPEENGINEWIN32INIT_H

#include "PepeEngineInit.h"

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
#endif

#include <windows.h>

_PEPE_ENGINE_START

class CPepeEngineWin32ConfigDialog;
class CPepeEngineWin32Plugin;
class CPepeEngineWin32Timer;
class CPepeEngineWin32Window;

_PEPE_ENGINE_END


#if PEPE_ENGINE_PLATFORM == PEPE_ENGINE_PLATFORM_WIN32
#   if defined(PEPE_ENGINE_WIN32_STATIC_LIB)
#       define _PepeEngineWin32Export
#   else
#       if defined(PEPE_ENGINE_WIN32_NONCLIENT_BUILD)
#           define _PepeEngineWin32Export __declspec(dllexport)
#       else
#           define _PepeEngineWin32Export __declspec(dllimport)
#       endif
#   endif
#else
#   error operating system not supported    
#endif

#endif