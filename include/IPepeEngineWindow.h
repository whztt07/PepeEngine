/**
 * Project: PepeEngine
 * Tier:    Middleware
 * File:    IPepeEngineWindow.h
 *
 * @brief   Declaration of IPepeEngineWindow interface.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-10-21
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef IPEPEENGINEWINDOW_H
#define IPEPEENGINEWINDOW_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

class _PepeEngineExport IPepeEngineWindow
{
protected:
    unsigned int m_nWidth;      /**< Window width   */
    unsigned int m_nHeight;     /**< Window height  */

    /** @{ Window position */
    int m_iLeft;
    int m_iTop;
    /** @} */

    tstring m_strWindowName;
    tstring m_strBorder;

    bool m_bFullScreen;
    bool m_bSwitchingFullScreen;
public:

    IPepeEngineWindow() :
        m_nWidth(0), m_nHeight(0),
        m_iLeft(0), m_iTop(0), m_bFullScreen(false), m_bSwitchingFullScreen(false) {}

    virtual void create(
        const tstring& strName,
        size_t nWidth, size_t nHeight,
        bool bFullScreen
    ) = 0;

    virtual void destroy() = 0;
    virtual bool run() const = 0;
    virtual void resize(size_t nWidth, size_t nHeight) = 0;
    virtual void reposition(size_t nPosX, size_t nPosY) = 0;
    virtual void windowMovedOrResized() = 0;
    virtual void setFullScreen(bool bFullScreen, size_t nWidth, size_t nHeight) = 0;
    virtual void finishSwitchingFullScreen(size_t nWidth, size_t nHeight) = 0;

    bool isFullScreen() const {
        return m_bFullScreen;
    }
    bool isSwitchingFullScreen() const {
        return m_bSwitchingFullScreen;
    }

    size_t getWidth() const {
        return m_nWidth;
    }
    size_t getHeight() const {
        return m_nHeight;
    }

    virtual void setWindowTitle(const tstring& strName) const = 0;

    virtual void getCustomAttribute(const tstring& strParam, void* pData) const = 0;

    virtual void initConfigOptions();
};

_PEPE_ENGINE_END

#endif