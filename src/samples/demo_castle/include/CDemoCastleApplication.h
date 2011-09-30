#ifndef CDEMOCASTLEAPPLICATION_H
#define CDEMOCASTLEAPPLICATION_H

#include "CDefApplication.h"

class CDemoCastleApplication : public CDefApplication
{
public:
		 CDemoCastleApplication();
		 ~CDemoCastleApplication();
	

	void createCamera();	
	void createScene();	
	bool setup();
};

#endif