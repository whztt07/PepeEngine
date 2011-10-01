#ifndef CDEFAPPLICATION_H
#define CDEFAPPLICATION_H

#include "CDefApplicationFrameListener.h"

class CDefApplication
{
public:
    CDefApplication();
    virtual                         ~CDefApplication();

    virtual void                    createCamera();
    virtual void                    createFrameListener();
    virtual void                    createScene();
    virtual void                    run();
    virtual bool                    setup();
protected:
    CPepeEngineCore*                m_pCore;
    CPepeEngineCamera*              m_pCamera;
    CPepeEngineSceneManager*        m_pSceneManager;
    CDefApplicationFrameListener*   m_pFrameListener;
};

#endif