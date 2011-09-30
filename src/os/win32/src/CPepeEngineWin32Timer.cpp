#include "CPepeEngineWin32Timer.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineWin32Timer::CPepeEngineWin32Timer() : m_dwTimerMask(0)
{	
	reset();
}

// -----------------------------------------------------------------------------------------
CPepeEngineWin32Timer::~CPepeEngineWin32Timer()
{
}

// -----------------------------------------------------------------------------------------
void CPepeEngineWin32Timer::reset()
{
	// Get the current process core mask
	DWORD dwProcMask;
	DWORD dwSysMask;
	#if _MSC_VER >= 1400 && defined (_M_X64)
		GetProcessAffinityMask(GetCurrentProcess(), (PDWORD_PTR)&dwProcMask, (PDWORD_PTR)&dwSysMask);
	#else
		GetProcessAffinityMask(GetCurrentProcess(), &dwProcMask, &dwSysMask);
	#endif

	// Find the lowest core that this process uses
	if (m_dwTimerMask == 0)
	{
		m_dwTimerMask = 1;
		while ((m_dwTimerMask & dwProcMask ) == 0 )
		{
			m_dwTimerMask <<= 1;
		}
	}

	HANDLE hThread = GetCurrentThread();

	// Set affinity to the first core
	DWORD dwOldMask = SetThreadAffinityMask(hThread, m_dwTimerMask);

	// Get the constant frequency
	QueryPerformanceFrequency(&m_frequency);

	// Query the timer
	QueryPerformanceCounter(&m_startTime);
	m_dwStartTick = GetTickCount();

	// Reset affinity
	SetThreadAffinityMask(hThread, dwOldMask);

	m_lastTime	= 0;
	m_zeroClock = clock();
}

// -----------------------------------------------------------------------------------------
unsigned long CPepeEngineWin32Timer::getMilliseconds()
{
	LARGE_INTEGER	curTime;
	HANDLE			hThread = GetCurrentThread();

	// Set affinity to the first core
	DWORD dwOldMask = SetThreadAffinityMask(hThread, m_dwTimerMask);

	// Query the timer
	QueryPerformanceCounter(&curTime);

	// Reset affinity
	SetThreadAffinityMask(hThread, dwOldMask);

	LONGLONG newTime = curTime.QuadPart - m_startTime.QuadPart;

	// scale by 1000 for milliseconds
	unsigned long newTicks = (unsigned long) (1000 * newTime / m_frequency.QuadPart);

	// detect and compensate for performance counter leaps
	// (surprisingly common, see Microsoft KB: Q274323)
	unsigned long check = GetTickCount() - m_dwStartTick;
	signed long msecOff = (signed long)(newTicks - check);
	if (msecOff < -100 || msecOff > 100)
	{		
		LONGLONG adjust			= (std::min)(msecOff * m_frequency.QuadPart / 1000, newTime - m_lastTime);
		m_startTime.QuadPart	+= adjust;
		newTime					-= adjust;

		// Re-calculate milliseconds
		newTicks = (unsigned long) (1000 * newTime / m_frequency.QuadPart);
	}

	// Record last time for adjust
	m_lastTime = newTime;

	return newTicks;
}

// -----------------------------------------------------------------------------------------
unsigned long CPepeEngineWin32Timer::getMicroseconds()
{
	LARGE_INTEGER	curTime;
	HANDLE			hThread = GetCurrentThread();

	// Set affinity to the first core
	DWORD dwOldMask = SetThreadAffinityMask(hThread, m_dwTimerMask);

	// Query the timer
	QueryPerformanceCounter(&curTime);

	// Reset affinity
	SetThreadAffinityMask(hThread, dwOldMask);

	LONGLONG newTime = curTime.QuadPart - m_startTime.QuadPart;

	// get milliseconds to check against GetTickCount
	unsigned long newTicks = (unsigned long) (1000 * newTime / m_frequency.QuadPart);

	// detect and compensate for performance counter leaps
	// (surprisingly common, see Microsoft KB: Q274323)
	unsigned long check = GetTickCount() - m_dwStartTick;
	signed long msecOff = (signed long)(newTicks - check);
	if (msecOff < -100 || msecOff > 100)
	{		
		LONGLONG adjust			= (std::min)(msecOff * m_frequency.QuadPart / 1000, newTime - m_lastTime);
		m_startTime.QuadPart	+= adjust;
		newTime					-= adjust;
	}

	// Record last time for adjust
	m_lastTime = newTime;

	// scale by 1000000 for microseconds
	unsigned long newMicro = (unsigned long) (1000000 * newTime / m_frequency.QuadPart);

	return newMicro;
}

// -----------------------------------------------------------------------------------------
unsigned long CPepeEngineWin32Timer::getMillisecondsCPU()
{
	clock_t newClock = clock();
	return (unsigned long)((float)(newClock - m_zeroClock) / ((float)CLOCKS_PER_SEC / 1000.0)) ;
}

// -----------------------------------------------------------------------------------------
unsigned long CPepeEngineWin32Timer::getMicrosecondsCPU()
{
	clock_t newClock = clock();
	return (unsigned long)((float)(newClock - m_zeroClock ) / ((float)CLOCKS_PER_SEC / 1000000.0));
}

_PEPE_ENGINE_END