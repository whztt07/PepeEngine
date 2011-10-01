/**
 * Project: PepeEngine
 * Tier:    Middleware
 * File:    IPepeEngineRenderWindow.h
 *
 * @brief   Declaration of IPepeEngineRenderWindow abstract class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-10-15
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef IPEPEENGINERENDERWINDOW_H
#define IPEPEENGINERENDERWINDOW_H

#include "PepeEngineInit.h"

#include "IPepeEngineWindow.h"

_PEPE_ENGINE_START

class _PepeEngineExport IPepeEngineRenderWindow
{
protected:

    bool    m_bVSync;
    bool    m_bDepthBuffer;
    size_t  m_nColorDepth;
    size_t  m_nDisplayFrequency;

    IPepeEngineWindow* m_pWindow;
public:

    IPepeEngineRenderWindow(IPepeEngineWindow* pWindow)
        : m_pWindow(pWindow), m_bVSync(false), m_bDepthBuffer(true), m_nColorDepth(32), m_nDisplayFrequency(85) {}
    virtual ~IPepeEngineRenderWindow() {}

    virtual void create() = 0;
    virtual void swapBuffers() = 0;
    virtual void update() = 0;

    virtual void notifyWindowChange() = 0;
    virtual void notifySwitchingFullScreen(
        bool bOldFullScreen, bool bFullScreen,
        size_t nWidth, size_t nHeight
    ) = 0;

    bool isVSync() const {
        return m_bVSync;
    }
    bool hasDepthBuffer() const {
        return m_bDepthBuffer;
    }
    IPepeEngineWindow* getWindow() const {
        return m_pWindow;
    }
};

_PEPE_ENGINE_END

#endif