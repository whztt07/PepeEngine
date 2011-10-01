#include "CDemoSceneManagerFrameListener.h"

Radian g_fRot;
Radian g_fRot2;

// -----------------------------------------------------------------------------------------
bool CDemoSceneManagerFrameListener::frameStarted(const FrameEvent& frameEvent)
{
    g_fRot  = Degree(90.0f * frameEvent.fTimeSinceLastFrame);
    g_fRot2 = Degree(120.0f * frameEvent.fTimeSinceLastFrame);

    CPepeEngineCore::getSingleton().getSceneManager()->getSceneNode(_T("Sphere0"))->pitch(g_fRot);
    CPepeEngineCore::getSingleton().getSceneManager()->getSceneNode(_T("Sphere1"))->roll(g_fRot2);
    CPepeEngineCore::getSingleton().getSceneManager()->getSceneNode(_T("Sphere2"))->yaw(g_fRot);

    CPepeEngineCore::getSingleton().getSceneManager()->getSceneNode(_T("Sphere3"))->pitch(g_fRot);

    CPepeEngineCore::getSingleton().getSceneManager()->getSceneNode(_T("Sphere4"))->roll(g_fRot);
    CPepeEngineCore::getSingleton().getSceneManager()->getSceneNode(_T("Sphere5"))->pitch(g_fRot2);
    CPepeEngineCore::getSingleton().getSceneManager()->getSceneNode(_T("Sphere5"))->yaw(g_fRot);

    CPepeEngineCore::getSingleton().getSceneManager()->getSceneNode(_T("Sphere6"))->pitch(g_fRot);
    CPepeEngineCore::getSingleton().getSceneManager()->getSceneNode(_T("Sphere7"))->roll(g_fRot);
    CPepeEngineCore::getSingleton().getSceneManager()->getSceneNode(_T("Sphere8"))->pitch(g_fRot2);



    return CDefApplicationFrameListener::frameStarted(frameEvent);
}