#include "CDefApplicationFrameListener.h"

// -----------------------------------------------------------------------------------------
CDefApplicationFrameListener::CDefApplicationFrameListener() : m_pInputManager(0), m_pMouse(0), m_pKeyboard(0)
{
    size_t windowHnd = 0;
    CPepeEngineCore::getSingleton().getWindow()->getCustomAttribute(_T("WINDOW"), &windowHnd);

    m_pInputManager     = InputManager::createInputSystem(windowHnd);
    m_pKeyboard         = static_cast<Keyboard*>(m_pInputManager->createInputObject(OISKeyboard, true));
    m_pMouse            = static_cast<Mouse*>(m_pInputManager->createInputObject(OISMouse, true));
    m_translate         = CPepeEngineVector3::ZERO;
    m_rotX              = 0.0f;
    m_rotY              = 0.0f;
    m_fMoveScale        = 10.0f;
    m_fRotScale         = 18.0f;
    m_bCameraNeedUpdate = true;

    m_pKeyboard->setEventCallback(this);
    m_pMouse->setEventCallback(this);
}

// -----------------------------------------------------------------------------------------
CDefApplicationFrameListener::~CDefApplicationFrameListener()
{
    if (m_pInputManager) {
        m_pInputManager->destroyInputObject(m_pMouse);
        m_pInputManager->destroyInputObject(m_pKeyboard);

        InputManager::destroyInputSystem(m_pInputManager);
        m_pInputManager = 0;
    }
}

// -----------------------------------------------------------------------------------------
bool CDefApplicationFrameListener::frameStarted(const FrameEvent& frameEvent)
{

    CPepeEngineVector3 vec(0.0f, 2.5f, -2.0f);
    vec.normalise();

    m_translate = CPepeEngineVector3::ZERO;
    m_rotX      = 0.0f;
    m_rotY      = 0.0f;

    if (!processKeyStats(frameEvent)) {
        return false;
    }

    if (!processMouseStats(frameEvent)) {
        return false;
    }

    if (m_bCameraNeedUpdate) {
        moveCamera();
    }

    return true;
}

// -----------------------------------------------------------------------------------------
bool CDefApplicationFrameListener::frameEnded(const FrameEvent& frameEvent)
{
    CPepeEngineCore::getSingleton().drawStats();
    showInfo();
    showAuthor();
    return true;
}

// -----------------------------------------------------------------------------------------
bool CDefApplicationFrameListener::processKeyStats(const FrameEvent& frameEvent)
{
    m_pKeyboard->capture();

    if (m_pKeyboard->isKeyDown(KC_ESCAPE)) {
        return false;
    }

    if (m_pKeyboard->isKeyDown(KC_A)) {
        m_translate.x = -m_fMoveScale * frameEvent.fTimeSinceLastFrame;
        m_bCameraNeedUpdate = true;
    }

    if (m_pKeyboard->isKeyDown(KC_D)) {
        m_translate.x = m_fMoveScale * frameEvent.fTimeSinceLastFrame;
        m_bCameraNeedUpdate = true;
    }

    if (m_pKeyboard->isKeyDown(KC_W)) {
        m_translate.z = -m_fMoveScale * frameEvent.fTimeSinceLastFrame;
        m_bCameraNeedUpdate = true;
    }

    if (m_pKeyboard->isKeyDown(KC_S)) {
        m_translate.z = m_fMoveScale * frameEvent.fTimeSinceLastFrame;
        m_bCameraNeedUpdate = true;
    }

    if (m_pKeyboard->isKeyDown(KC_1)) {
        CPepeEngineCore::getSingleton().getRenderer()->setPolygonMode(PM_SOLID);
    }

    if (m_pKeyboard->isKeyDown(KC_2)) {
        CPepeEngineCore::getSingleton().getRenderer()->setPolygonMode(PM_WIREFRAME);
    }

    if (m_pKeyboard->isKeyDown(KC_3)) {
        CPepeEngineCore::getSingleton().getRenderer()->setPolygonMode(PM_POINTS);
    }

    return true;
}

// -----------------------------------------------------------------------------------------
bool CDefApplicationFrameListener::keyPressed(const KeyEvent &arg)
{
    return true;
}

// -----------------------------------------------------------------------------------------
bool CDefApplicationFrameListener::keyReleased(const KeyEvent &arg)
{
    if (arg.key == KC_F1) {
        CPepeEngineSceneManager* pSceneManager = CPepeEngineCore::getSingleton().getSceneManager();

        if (pSceneManager->isDebugMode()) {
            pSceneManager->setDebugMode(false);
        } else {
            pSceneManager->setDebugMode(true);
        }
    }

    if (arg.key == KC_F11) {
        IPepeEngineWindow*      pWindow     = CPepeEngineCore::getSingleton().getRenderer()->getRenderWindow()->getWindow();
        IPepeEngineVideoMode*   pVideoMode  = CPepeEngineCore::getSingleton().getRenderer()->getVideoModeList()->item(
                                                  CPepeEngineCore::getSingleton().getConfig()->getOption(_T("Video Mode"))->strValue
                                              );

        if (pWindow->isFullScreen()) {
            pWindow->setFullScreen(false, pVideoMode->getWidth(), pVideoMode->getHeight());
        } else {
            pWindow->setFullScreen(true, pVideoMode->getWidth(), pVideoMode->getHeight());
        }
    }

    return true;
}

// -----------------------------------------------------------------------------------------
bool CDefApplicationFrameListener::processMouseStats(const FrameEvent& frameEvent)
{
    m_pMouse->capture();

    const MouseState &ms = m_pMouse->getMouseState();

    if (ms.buttonDown(MB_Right)) {
        m_translate.x += ms.X.rel * m_fMoveScale * frameEvent.fTimeSinceLastFrame;
        m_translate.y -= ms.Y.rel * m_fMoveScale * frameEvent.fTimeSinceLastFrame;
    } else {
        m_rotX = Degree(-ms.X.rel * m_fRotScale * frameEvent.fTimeSinceLastFrame);
        m_rotY = Degree(-ms.Y.rel * m_fRotScale * frameEvent.fTimeSinceLastFrame);
    }

    return true;
}

// -----------------------------------------------------------------------------------------
bool CDefApplicationFrameListener::mouseMoved(const MouseEvent &e)
{
    m_bCameraNeedUpdate = true;
    return true;
}

// -----------------------------------------------------------------------------------------
bool CDefApplicationFrameListener::mousePressed(const MouseEvent &e, MouseButtonID id)
{
    return true;
}

// -----------------------------------------------------------------------------------------
bool CDefApplicationFrameListener::mouseReleased(const MouseEvent &e, MouseButtonID id)
{
    return true;
}

// -----------------------------------------------------------------------------------------
void CDefApplicationFrameListener::moveCamera()
{
    CPepeEngineCamera* pCamera = CPepeEngineCore::getSingleton().getSceneManager()->getCamera();
    pCamera->yaw(m_rotX);
    pCamera->pitch(m_rotY);
    pCamera->moveRelative(m_translate);
    m_bCameraNeedUpdate = false;
}

// -----------------------------------------------------------------------------------------
void CDefApplicationFrameListener::showInfo()
{
    IPepeEngineRenderer* pRenderer = CPepeEngineCore::getSingleton().getRenderer();

    tstring str =   _T("Camera:\t\t\tWSAD+Mouse\nChange polygon mode:\t1,2,3\nFullscreen ON/OFF:\tF11\nDebug mode ON/OFF:\tF1\nExit:\t\t\tEsc");

    size_t nWidth   = pRenderer->getRenderWindow()->getWindow()->getWidth();
    size_t nHeight  = pRenderer->getRenderWindow()->getWindow()->getHeight();

    pRenderer->drawText(str, nWidth - 270, nHeight - 90);
}

// -----------------------------------------------------------------------------------------
void CDefApplicationFrameListener::showAuthor()
{
    IPepeEngineRenderer* pRenderer = CPepeEngineCore::getSingleton().getRenderer();

    tstring str =   _T("Author: Piotr Picheta");

    size_t nWidth   = pRenderer->getRenderWindow()->getWindow()->getWidth();
    size_t nHeight  = pRenderer->getRenderWindow()->getWindow()->getHeight();

    pRenderer->drawText(str, nWidth - 140, 10);
}