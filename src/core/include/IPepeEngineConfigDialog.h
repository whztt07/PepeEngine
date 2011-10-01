/**
 * Project: PepeEngine
 * Tier:    Middleware
 * File:    IPepeEngineConfigDialog.h
 *
 * @brief   Declaration of IPepeEngineConfigDialog interface.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-11-08
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef IPEPEENGINECONFIGDIALOG_H
#define IPEPEENGINECONFIGDIALOG_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

class _PepeEngineExport IPepeEngineConfigDialog
{
protected:
    IPepeEngineRenderer* m_pSelectedRenderer;
public:
    IPepeEngineConfigDialog() : m_pSelectedRenderer(0) {}
    virtual ~IPepeEngineConfigDialog() {}

    virtual bool display() = 0;
};

_PEPE_ENGINE_END

#endif