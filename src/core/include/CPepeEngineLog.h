/**
 * Project: PepeEngine
 * Tier:    Frontend
 * File:    CPepeEngineLog.h
 *
 * @brief   Declaration of CPepeEngineLog class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-04-06
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINELOG_H
#define CPEPEENGINELOG_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineLog
{
    tofstream   m_log;
    tstring     m_strLogName;
public:
    CPepeEngineLog(const tstring& strLogName, bool bClear = false);
    ~CPepeEngineLog();

    void logMessage(const tstring& strMessage);

    const tstring& getLogName() const {
        return m_strLogName;
    }
};

_PEPE_ENGINE_END

#endif