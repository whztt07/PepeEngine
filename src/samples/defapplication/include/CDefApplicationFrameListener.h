#ifndef CDEFAPPLICATIONFRAMELISTENER_H
#define CDEFAPPLICATIONFRAMELISTENER_H

#include "PepeEngine.h"

#ifndef OIS_DYNAMIC_LIB
#   define OIS_DYNAMIC_LIB
#endif

#include <ois/OIS.h>

using namespace pepeengine;
using namespace OIS;

class CDefApplicationFrameListener : public IPepeEngineFrameListener, KeyListener, MouseListener
{
public:
    CDefApplicationFrameListener();
    virtual             ~CDefApplicationFrameListener();

    virtual bool        frameStarted(const FrameEvent& frameEvent);
    virtual bool        frameEnded(const FrameEvent& frameEvent);

    virtual bool        processKeyStats(const FrameEvent& frameEvent);
    virtual bool        keyPressed(const KeyEvent &arg);
    virtual bool        keyReleased(const KeyEvent &arg);

    virtual bool        processMouseStats(const FrameEvent& frameEvent);
    virtual bool        mouseMoved(const MouseEvent &e);
    virtual bool        mousePressed(const MouseEvent &e, MouseButtonID id);
    virtual bool        mouseReleased(const MouseEvent &e, MouseButtonID id);

    virtual void        moveCamera();
    virtual void        showInfo();
    void                showAuthor();
protected:
    InputManager*       m_pInputManager;
    Mouse*              m_pMouse;
    Keyboard*           m_pKeyboard;

    CPepeEngineVector3  m_translate;
    Radian              m_rotX;
    Radian              m_rotY;

    float               m_fMoveScale;
    float               m_fRotScale;

    bool                m_bCameraNeedUpdate;
};

#endif