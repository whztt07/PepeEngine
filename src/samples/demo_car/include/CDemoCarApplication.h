#ifndef CDEMOCARAPPLICATION_H
#define CDEMOCARAPPLICATION_H

#include "CDefApplication.h"

class CDemoCarApplication : public CDefApplication
{
public:
    CDemoCarApplication();
    ~CDemoCarApplication();

    void createCamera();
    void createScene();
    bool setup();
};

#endif