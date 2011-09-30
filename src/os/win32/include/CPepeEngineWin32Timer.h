/**
 * Project: PepeEngine
 * Tier:	Backend 
 * File:	CPepeEngineWin32Timer.h   
 *
 * @brief	Declaration of CPepeEngineWin32Timer class. 
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-10-24
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEWIN32TIMER_H
#define CPEPEENGINEWIN32TIMER_H

#include "PepeEngineWin32Init.h"

#include "IPepeEngineTimer.h"

_PEPE_ENGINE_START

// win hack
#ifdef max
#  undef max
#endif
#ifdef min
#  undef min
#endif

class _PepeEngineWin32Export CPepeEngineWin32Timer : public IPepeEngineTimer
{	
	clock_t m_zeroClock;

	DWORD			m_dwStartTick;
	LONGLONG		m_lastTime;
	LARGE_INTEGER	m_startTime;
	LARGE_INTEGER	m_frequency;
	DWORD			m_dwTimerMask;
public:

	CPepeEngineWin32Timer();
	~CPepeEngineWin32Timer();

	/** 
	 *	Resets timer 
	 */
	void reset();

	/** 
	 *	Returns milliseconds since initialisation or last reset 
	 */
	unsigned long getMilliseconds();

	/** 
	 *	Returns microseconds since initialisation or last reset 
	 */
	unsigned long getMicroseconds();

	/** 
	 *	Returns milliseconds since initialisation or last reset, only CPU time measured 
	 */	
	unsigned long getMillisecondsCPU();

	/** 
	 *	Returns microseconds since initialisation or last reset, only CPU time measured 
	 */	
	unsigned long getMicrosecondsCPU();
};

_PEPE_ENGINE_END

#endif