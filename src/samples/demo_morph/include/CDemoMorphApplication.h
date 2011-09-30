#ifndef CDEMOMORPHAPPLICATION_H
#define CDEMOMORPHAPPLICATION_H

#include "CDefApplication.h"

class CDemoMorphApplication : public CDefApplication
{
public:
		 CDemoMorphApplication();
		 ~CDemoMorphApplication();
	
	void createScene();	
	bool setup();
};

#endif