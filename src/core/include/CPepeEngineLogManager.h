/**
 * Project: PepeEngine
 * Tier:	Frontend
 * File:	CPepeEngineLogManager.h   
 *
 * @brief	Declaration of CPepeEngineLogManager class.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-04-06
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINELOGMANAGER_H
#define CPEPEENGINELOGMANAGER_H

#include "PepeEngineInit.h"

#include "CPepeEngineLog.h"
#include "CPepeEngineSingleton.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineLogManager : public CPepeEngineSingleton<CPepeEngineLogManager>
{
	typedef std::map<tstring, CPepeEngineLog*, std::less<tstring>> LogList;

	LogList m_logList;

	CPepeEngineLog* m_pDefaultLog;
public:

	CPepeEngineLogManager();
	~CPepeEngineLogManager();

	CPepeEngineLog* createLog(const tstring& strLogName, bool bClear = false, bool bDefaultLog = false);
	void destroyLog(const tstring& strLogName);

	void logMessage(const tstring& strLogName, const tstring& strMessage);	
	void logMessage(const tstring& strMessage);	

	CPepeEngineLog* getDefaultLog() const {return m_pDefaultLog;}

	static CPepeEngineLogManager& getSingleton(void);
	static CPepeEngineLogManager* getSingletonPtr(void);
};

_PEPE_ENGINE_END

#endif