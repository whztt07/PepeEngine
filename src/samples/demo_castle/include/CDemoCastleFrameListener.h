#ifndef CDEMOCASTLEFRAMELISTENER_H
#define CDEMOCASTLEFRAMELISTENER_H

#include "CDefApplicationFrameListener.h"

class CDemoCastleFrameListener : public CDefApplicationFrameListener
{			
public:						
	bool frameStarted(const FrameEvent& frameEvent);	
};

#endif