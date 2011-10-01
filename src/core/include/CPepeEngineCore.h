/**
 * Project:     PepeEngine
 * Tier:        Frontend
 * File:        CPepeEngineCore.h
 *
 * @class       CPepeEngineCore
 *
 * @brief       Declaration of CPepeEngineCore class and structures used to init engine.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-02-16
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version     1.0
 */

#ifndef CPEPEENGINECORE_H
#define CPEPEENGINECORE_H

#include "PepeEngineInit.h"

#include "CPepeEngineConfig.h"
#include "CPepeEngineDynamicLibraryManager.h"
#include "CPepeEngineImageManager.h"
#include "CPepeEngineSceneManager.h"
#include "CPepeEngineSingleton.h"
#include "IPepeEngineConfigDialog.h"
#include "IPepeEngineFrameListener.h"
#include "IPepeEngineWindow.h"

_PEPE_ENGINE_START


typedef std::vector<IPepeEngineRenderer*> RendererList;

/**
 * Class used to init engine.
 */
class _PepeEngineExport CPepeEngineCore : public CPepeEngineSingleton<CPepeEngineCore>
{

    IPepeEngineWindow*          m_pWindow;
    IPepeEngineTimer*           m_pTimer;
    IPepeEngineConfigDialog*    m_pConfigDialog;

    RendererList*               m_pRendererList;

    IPepeEngineRenderer*        m_pRenderer;
    CPepeEngineConfig*          m_pConfig;

    CPepeEngineDynamicLibraryManager*   m_pDynamicLibraryManager;
    CPepeEngineImageManager*            m_pImageManager;
    CPepeEngineLogManager*              m_pLogManager;
    CPepeEngineMaterialManager*         m_pMaterialManager;
    CPepeEngineMeshManager*             m_pMeshManager;
    CPepeEngineSceneManager*            m_pSceneManager;

    typedef std::vector<DynamicLibraryPtr> PluginLibList;
    typedef std::vector<IPepeEnginePlugin*>         PluginList;
    typedef std::vector<IPepeEngineFrameListener*>  FrameListenerList;

    PluginLibList       m_pluginLibs;
    PluginList          m_plugins;
    FrameListenerList   m_frameListeners;

    /**
     *  Indicates the type of event to be considered by calculateEventTime().
     */
    enum FrameEventTimeType {
        FETT_ANY        = 0,
        FETT_STARTED    = 1,
        FETT_ENDED      = 2
    };

    /**
     *  Contains the times of recently fired events
     */
    std::deque<unsigned long> m_eventTimes[3];
    float m_fFrameSmoothingTime;
    float m_fFrameTime;
    float m_fElapsedTime;

    /**
     *  Internal method for calculating the average time between recently fired events.
     *  @param now  The current time in ms.
     *  @param type The type of event to be considered.
    */
    float calculateEventTime(unsigned long now, FrameEventTimeType type);
public:

    CPepeEngineCore();
    ~CPepeEngineCore();

    void create(
        const tstring& strWindowTitle
    );

    CPepeEngineSceneManager* getSceneManager() const {
        return m_pSceneManager;
    }
    IPepeEngineRenderer* getRenderer() const {
        return m_pRenderer;
    }
    IPepeEngineWindow* getWindow() const {
        return m_pWindow;
    }
    CPepeEngineConfig* getConfig() const {
        return m_pConfig;
    }
    IPepeEngineTimer* getTimer() const {
        return m_pTimer;
    }

    bool fireFrameStarted();
    bool fireFrameEnded();

    void addFrameListener(IPepeEngineFrameListener* pFrameListener);
    void removeFrameListener(IPepeEngineFrameListener* pFrameListener);

    /**
     *  Sets the period over which PepeEngine smooths out fluctuations in frame times.
     *
     *  PepeEngine by default gives you the raw frame time, but can optionally
     *  smooths it out over several frames, in order to reduce the
     *  noticeable effect of occasional hiccups in framerate.
     */
    void setFrameSmoothingPeriod(float fPeriod) {
        m_fFrameSmoothingTime = fPeriod;
    }

    /**
     *  Gets the period over which PepeEngine smooths out fluctuations in frame times.
     */
    float getFrameSmoothingPeriod(void) const {
        return m_fFrameSmoothingTime;
    }

    void run();

    void addRenderer(IPepeEngineRenderer* pRenderer);
    RendererList* getAvailableRenderers() const {
        return m_pRendererList;
    }
    void setActiveRenderer(IPepeEngineRenderer* pRenderer);
    IPepeEngineRenderer* getRendererByName(const tstring& strName) const;

    void addWindow(IPepeEngineWindow* pWindow);
    void addTimer(IPepeEngineTimer* pTimer);
    void addConfigDialog(IPepeEngineConfigDialog* pConfigDialog);

    void loadPlugin(const tstring& strPluginName);
    void unloadPlugin(const tstring& strPluginName);
    void unloadPlugins();
    void installPlugin(IPepeEnginePlugin* pPlugin);
    void uninstallPlugin(IPepeEnginePlugin* pPlugin);

    bool showConfigDialog();

    inline float getElapsedTime() const;
    inline float getFrameTime() const;

    /**
     *  Shows standard program statistics, should be invoked after
     *  rendering scene.
     */
    void drawStats();

    static CPepeEngineCore& getSingleton(void);
    static CPepeEngineCore* getSingletonPtr(void);
};

_PEPE_ENGINE_END

#endif