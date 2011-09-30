/**
 * Project: PepeEngine
 * Tier:	Frontend
 * File:	IPepeEngineFrameListener.h   
 *
 * @brief	Declaration of IPepeEngineFrameListener interface.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-10-30
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef IPEPEENGINEFRAMELISTENER_H
#define IPEPEENGINEFRAMELISTENER_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

struct FrameEvent
{
	/** 
	 *	Elapsed time in seconds since the last frame.
	 *	This gives you time between frame start & frame end,
	 *	and between frame end and next frame start.	
	 */	
	float fTimeSinceLastFrame;
};

class _PepeEngineExport IPepeEngineFrameListener
{		
public:	
	virtual ~IPepeEngineFrameListener() {}

	virtual bool frameStarted(const FrameEvent& frameEvent) {return true;}
	virtual bool frameEnded(const FrameEvent& frameEvent) {return true;}

};

_PEPE_ENGINE_END

#endif