#ifndef CDEMOSCENEMANAGERAPPLICATION_H
#define CDEMOSCENEMANAGERAPPLICATION_H

#include "CDefApplication.h"

class CDemoSceneManagerApplication : public CDefApplication
{
public:
    CDemoSceneManagerApplication();
    ~CDemoSceneManagerApplication();

    void createCamera();
    void createScene();
    bool setup();
};

#endif