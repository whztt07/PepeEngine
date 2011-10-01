/**
 * Project: PepeEngine
 * Tier:    Backend
 * File:    CPepeEngineWin32ConfigDialog.h
 *
 * @brief   Declaration of CPepeEngineWin32ConfigDialog class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-11-08
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEWIN32CONFIGDIALOG_H
#define CPEPEENGINEWIN32CONFIGDIALOG_H

#include "PepeEngineWin32Init.h"

#include "IPepeEngineConfigDialog.h"

_PEPE_ENGINE_START

// win hack
#ifdef max
#  undef max
#endif
#ifdef min
#  undef min
#endif

class _PepeEngineWin32Export CPepeEngineWin32ConfigDialog : public IPepeEngineConfigDialog
{
protected:

    /**
     *  Callback to process window events
     */
#if PEPE_ENGINE_ARCH_TYPE == PEPE_ENGINE_ARCHITECTURE_64
    static INT_PTR CALLBACK dlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
#else
    static BOOL CALLBACK dlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
#endif

    HINSTANCE m_hInstance;

public:
    CPepeEngineWin32ConfigDialog();
    ~CPepeEngineWin32ConfigDialog();

    bool display();
};

_PEPE_ENGINE_END

#endif