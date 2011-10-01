#ifndef CDEMOOCEANAPPLICATION_H
#define CDEMOOCEANAPPLICATION_H

#include "CDefApplication.h"

class CDemoOceanApplication : public CDefApplication
{
public:
    CDemoOceanApplication();
    ~CDemoOceanApplication();

    void createCamera();
    void createScene();
    bool setup();
};

#endif