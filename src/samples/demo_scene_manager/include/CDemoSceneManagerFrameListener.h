#ifndef CDEMOSCENEMANAGERFRAMELISTENER_H
#define CDEMOSCENEMANAGERFRAMELISTENER_H

#include "CDefApplicationFrameListener.h"

class CDemoSceneManagerFrameListener : public CDefApplicationFrameListener
{			
public:						
	bool frameStarted(const FrameEvent& frameEvent);	
};

#endif