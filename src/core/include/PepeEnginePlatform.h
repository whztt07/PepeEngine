/**
 * Project: PepeEngine
 * File:	PepeEnginePlatform.h  
 *
 * @brief	Platform depended declaration.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-05-21
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef PEPEENGINEPLATFORM_H
#define PEPEENGINEPLATFORM_H

#include "PepeEngineConfig.h"

_PEPE_ENGINE_START

#define PEPE_ENGINE_PLATFORM_WIN32 1

#define PEPE_ENGINE_COMPILER_MSVC 1
#define PEPE_ENGINE_COMPILER_GNUC 2
#define PEPE_ENGINE_COMPILER_BORL 3

#define PEPE_ENGINE_ARCHITECTURE_32 1
#define PEPE_ENGINE_ARCHITECTURE_64 2

#if defined(__WIN32__) || defined(_WIN32)
	#define PEPE_ENGINE_PLATFORM PEPE_ENGINE_PLATFORM_WIN32	
#endif

#if defined( _MSC_VER )
#   define PEPE_ENGINE_COMPILER PEPE_ENGINE_COMPILER_MSVC
#   define PEPE_ENGINE_COMP_VER _MSC_VER
#elif defined( __GNUC__ )
#   define PEPE_ENGINE_COMPILER PEPE_ENGINE_COMPILER_GNUC
#   define PEPE_ENGINE_COMP_VER (((__GNUC__)*100) + (__GNUC_MINOR__*10) + __GNUC_PATCHLEVEL__)
#elif defined( __BORLANDC__ )
#   define PEPE_ENGINE_COMPILER PEPE_ENGINE_COMPILER_BORL
#   define PEPE_ENGINE_COMP_VER __BCPLUSPLUS__
#else
#   pragma error "Unknown compiler error."
#endif


/** 
 *	Find the arch type 
 */
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define PEPE_ENGINE_ARCH_TYPE PEPE_ENGINE_ARCHITECTURE_64
#else
#   define PEPE_ENGINE_ARCH_TYPE PEPE_ENGINE_ARCHITECTURE_32
#endif

#if PEPE_ENGINE_PLATFORM == PEPE_ENGINE_PLATFORM_WIN32
#	if defined(PEPE_ENGINE_STATIC_LIB)
#		define _PepeEngineExport
#	else
#		if defined(PEPE_ENGINE_NONCLIENT_BUILD)
#			define _PepeEngineExport __declspec(dllexport)
#		else
# 			define _PepeEngineExport __declspec(dllimport)
#		endif
#	endif
#else
#	error operating system not supported	
#endif

_PEPE_ENGINE_END

#endif