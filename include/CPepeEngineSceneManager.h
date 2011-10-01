/**
 * Project: PepeEngine
 * Tier:    Frontend
 * File:    CPepeEngineSceneManager.h
 *
 * @brief   Declaration of CPepeEngineSceneManager class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-02-28
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINESCENEMANAGER_H
#define CPEPEENGINESCENEMANAGER_H

#include "PepeEngineInit.h"

#include "CPepeEngineAutoParamDataSource.h"
#include "CPepeEngineCamera.h"
#include "CPepeEngineColor.h"
#include "CPepeEngineSceneNode.h"
#include "CPepeEngineSkyBox.h"
#include "IPepeEngineRenderer.h"
#include "IPepeEngineTimer.h"

_PEPE_ENGINE_START

struct FrameStats {
    float           fLastFPS;
    float           fAvgFPS;
    float           fBestFPS;
    float           fWorstFPS;
    unsigned long   ulBestFrameTime;
    unsigned long   ulWorstFrameTime;
    size_t          nTriangleCount;
    size_t          nBatchCount;
};

class _PepeEngineExport CPepeEngineSceneManager
{
    typedef stdext::hash_map<tstring, CPepeEngineSceneNode*> SceneNodeMap;

    friend                          CPepeEngineSceneNode;

    CPepeEngineSceneNode*           m_pRoot;
    CPepeEngineCamera*              m_pCamera;
    CPepeEngineSkyBox*              m_pSkyBox;

    LightList                       m_lights;
    mutable LightList               m_lightsAffectingFrustum;

    CPepeEngineColor                m_bgColor;

    CPepeEngineAutoParamDataSource  m_autoParamDataSource;
    MaterialPtr                     m_pCurrentMaterial;

    FrameStats                      m_stats;

    unsigned long                   m_ulLastSecond;
    unsigned long                   m_ulLastTime;
    size_t                          m_nFrameCount;

    SceneNodeMap                    m_nodesMap;

    void                            addToNodeList(CPepeEngineSceneNode* pNode);
    void                            removeFromNodeList(const tstring& strName);
    void                            removeFromNodeList(CPepeEngineSceneNode* pNode);

    bool                            m_bDebugMode;
public:
    CPepeEngineSceneManager();
    ~CPepeEngineSceneManager();

    void attachCamera(CPepeEngineCamera* pCamera) {
        delete m_pCamera;
        m_pCamera = pCamera;
    }
    CPepeEngineCamera* getCamera() const {
        return m_pCamera;
    }

    CPepeEngineSceneNode* getRootSceneNode() const;
    void setRootSceneNode(CPepeEngineSceneNode *pNode);

    CPepeEngineSceneNode* createSceneNode(const tstring& strName);

    void createSkyBox(const tstring& strSkyBoxName, size_t nSize);
    void removeSkyBox();
    CPepeEngineSkyBox* getSkyBox() const {
        return m_pSkyBox;
    }
    bool hasSkyBox() const {
        return m_pSkyBox != NULL;
    }

    void setAmbientLight(float r, float g, float b);
    CPepeEngineLight* createLight(const tstring& strName);
    void addLight(CPepeEngineLight* pLight);
    bool hasLight(const CPepeEngineLight* pLight);
    bool hasLight(const tstring& strName);
    void removeLight(CPepeEngineLight* pLight);
    void removeLight(const tstring& strName);
    CPepeEngineLight* getLight(const tstring& strName);

    void setBgColor(const CPepeEngineColor& color) {
        m_bgColor = color;
    }
    const CPepeEngineColor& getBgColor() const {
        return m_bgColor;
    }

    void setCurrentMaterial(MaterialPtr pMaterial);
    const MaterialPtr getCurrentMaterial() const {
        return m_pCurrentMaterial;
    }

    void drawAll();

    void findLightsAffectingFrustum();

    const CPepeEngineAutoParamDataSource& getAutoParamDataSource() const {
        return m_autoParamDataSource;
    }

    void updateStats();
    void resetStats();

    const FrameStats&               getFrameStats() const {
        return m_stats;
    }
    inline CPepeEngineSceneNode*    getSceneNode(const tstring& strName) const;

    inline void                     setDebugMode(bool bDebugMode);
    inline bool                     isDebugMode() const;
};

_PEPE_ENGINE_END

#endif