/**
 * Project:		PepeEngine
 * Tier:		Frontend
 * File:		CPepeEngineStringUtil.h   
 *
 * @brief		Declaration of CPepeEngineStringUtil class.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-12-12
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef CPEPEENGINESTRINGUTIL_H
#define CPEPEENGINESTRINGUTIL_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineStringUtil
{	
public:
	static bool startsWith(const tstring& str, const tstring& pattern, bool lowerCase);
	static bool endsWith(const tstring& str, const tstring& pattern, bool lowerCase);
	static void	toLowerCase(tstring& str);
	static void	toUpperCase(tstring& str);
};

_PEPE_ENGINE_END

#endif