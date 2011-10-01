/**
 * Project: PepeEngine
 * Tier:    Frontend
 * File:    CPepeEngineSkyBox.h
 *
 * @brief   Declaration of CPepeEngineSkyBox class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-11-15
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINESKYBOX_H
#define CPEPEENGINESKYBOX_H

#include "PepeEngineInit.h"

#include "CPepeEngineEntity.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineSkyBox : public CPepeEngineEntity
{
public:
    CPepeEngineSkyBox(const tstring& strCubeTexture, int nSize);
    ~CPepeEngineSkyBox();

    void                renderFromCamera();
    inline TexturePtr   getTexture() const;

private:
    int                 m_nSize;
    TexturePtr          m_pSkyBoxTexture;

    void                createSphereMesh();
};

_PEPE_ENGINE_END

#endif