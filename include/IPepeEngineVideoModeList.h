/**
 * Project: PepeEngine
 * Tier:	Middleware
 * File:	IPepeEngineVideoModeList.h 
 *
 * @brief	Declaration of IPepeEngineVideoModeList interface.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-11-10
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef IPEPEENGINEVIDEOMODELIST_H
#define IPEPEENGINEVIDEOMODELIST_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

static unsigned int g_nModeCount = 0;

class IPepeEngineVideoMode
{	
protected:
	unsigned int m_nModeNumber;
public:
	IPepeEngineVideoMode() 
	{ 
		m_nModeNumber = ++g_nModeCount; 
	}

	virtual ~IPepeEngineVideoMode()
	{
		g_nModeCount--;
	}

	virtual unsigned int getWidth() const = 0;
	virtual unsigned int getHeight() const = 0;
	virtual unsigned int getRefreshRate() const = 0;
	virtual unsigned int getColourDepth() const = 0;
	virtual void increaseRefreshRate(unsigned int rr) = 0;
	virtual tstring getDescription() const = 0;
};

class IPepeEngineVideoModeList
{	
public:	
	virtual ~IPepeEngineVideoModeList() {}

	virtual IPepeEngineVideoMode* item(size_t index) = 0;
	virtual size_t count() = 0;

	virtual IPepeEngineVideoMode* item(const tstring &name) = 0;
};

_PEPE_ENGINE_END

#endif