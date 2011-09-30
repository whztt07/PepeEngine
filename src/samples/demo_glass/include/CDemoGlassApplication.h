#ifndef CDEMOGLASSAPPLICATION_H
#define CDEMOGLASSAPPLICATION_H

#include "CDefApplication.h"

class CDemoGlassApplication : public CDefApplication
{
public:
		 CDemoGlassApplication();
		 ~CDemoGlassApplication();
	
	void createScene();	
	bool setup();
};

#endif