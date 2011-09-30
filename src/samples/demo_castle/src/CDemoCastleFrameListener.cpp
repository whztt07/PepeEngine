#include "CDemoCastleFrameListener.h"

Radian g_fRot;

// -----------------------------------------------------------------------------------------
bool CDemoCastleFrameListener::frameStarted(const FrameEvent& frameEvent)
{
	g_fRot = Degree(45.0f * frameEvent.fTimeSinceLastFrame);

	CPepeEngineCore::getSingleton().getSceneManager()->getSceneNode(_T("LightRoot"))->yaw(g_fRot);	


	return CDefApplicationFrameListener::frameStarted(frameEvent);
}