#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineLog.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineLog::CPepeEngineLog(const tstring& strLogName, bool bClear)
{
	if (!bClear)
	{
		m_log.open(strLogName.c_str(), std::ios_base::out|std::ios_base::app);
	} else
	{
		m_log.open(strLogName.c_str(), std::ios_base::out|std::ios_base::beg);
	}
}

// -----------------------------------------------------------------------------------------
CPepeEngineLog::~CPepeEngineLog()
{
	m_log.close();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLog::logMessage(const tstring& strMessage)
{
	struct tm 	tm_time;
	time_t		ctTime; 

	time(&ctTime);	
	localtime_s(&tm_time, &ctTime);
	
	m_log	<< (tm_time.tm_year + 1900) << _T('-') << (tm_time.tm_mon + 1) << _T('-') << tm_time.tm_mday << _T(", ") 
			<< std::setw(2) << std::setfill(_T('0')) << tm_time.tm_hour
			<< _T(":") << std::setw(2) << std::setfill(_T('0')) << tm_time.tm_min
			<< _T(":") << std::setw(2) << std::setfill(_T('0')) << tm_time.tm_sec 
			<< _T(": ") << strMessage << std::endl;
			
	m_log.flush();
}

_PEPE_ENGINE_END