/**
 * Project: PepeEngine
 * Tier:    Backend
 * File:    CPepeEngineWin32Plugin.h
 *
 * @brief   Declaration of CPepeEngineWin32Plugin class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-05-29
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEWIN32PLUGIN_H
#define CPEPEENGINEWIN32PLUGIN_H

#include "PepeEngineWin32Init.h"

#include "CPepeEngineWin32ConfigDialog.h"
#include "CPepeEngineWin32Timer.h"
#include "CPepeEngineWin32Window.h"
#include "IPepeEnginePlugin.h"

_PEPE_ENGINE_START

class CPepeEngineWin32Plugin : public IPepeEnginePlugin
{
protected:
    CPepeEngineWin32Window*         m_pWindow;
    CPepeEngineWin32Timer*          m_pTimer;
    CPepeEngineWin32ConfigDialog*   m_pConfigDialog;
public:

    CPepeEngineWin32Plugin();

    const tstring& getName() const;

    void install();
    void initialise();
    void shutdown();
    void uninstall();
};

_PEPE_ENGINE_END

#endif