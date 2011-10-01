#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineLogManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineLogManager::CPepeEngineLogManager()
{
    m_pDefaultLog = NULL;
}

// -----------------------------------------------------------------------------------------
CPepeEngineLogManager::~CPepeEngineLogManager()
{
    // Destroy all logs
    LogList::iterator i;

    for (i = m_logList.begin(); i != m_logList.end(); i++) {
        delete i->second;
    }
}

// -----------------------------------------------------------------------------------------
CPepeEngineLog* CPepeEngineLogManager::createLog(const tstring& strLogName, bool bClear, bool bDefaultLog)
{
    CPepeEngineLog* newLog = new CPepeEngineLog(strLogName, bClear);

    if (!m_pDefaultLog || bDefaultLog) {
        m_pDefaultLog = newLog;
    }

    m_logList.insert(LogList::value_type(strLogName, newLog));

    return newLog;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLogManager::destroyLog(const tstring& strLogName)
{
    LogList::iterator i = m_logList.find(strLogName);

    if (i != m_logList.end()) {
        if (m_pDefaultLog == i->second) {
            m_pDefaultLog = NULL;
        }

        delete i->second;
        m_logList.erase(i);
    }

    // Set another default log if this one removed
    if (!m_pDefaultLog && !m_logList.empty()) {
        m_pDefaultLog = m_logList.begin()->second;
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLogManager::logMessage(const tstring& strLogName, const tstring& strMessage)
{
    CPepeEngineLog* log = m_logList[strLogName];
    log->logMessage(strMessage);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLogManager::logMessage(const tstring& strMessage)
{
    if (m_pDefaultLog) {
        m_pDefaultLog->logMessage(strMessage);
    }
}

// -----------------------------------------------------------------------------------------
template<> CPepeEngineLogManager* CPepeEngineSingleton<CPepeEngineLogManager>::ms_singleton = 0;

CPepeEngineLogManager& CPepeEngineLogManager::getSingleton(void)
{
    assert(ms_singleton);
    return *ms_singleton;
}

CPepeEngineLogManager* CPepeEngineLogManager::getSingletonPtr(void)
{
    return ms_singleton;
}

_PEPE_ENGINE_END