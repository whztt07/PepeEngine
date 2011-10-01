/**
 * Project:     PepeEngine
 * Tier:        Frontend
 * File:        CPepeEngineMeshFactory.h
 *
 * @brief       Declaration of CPepeEngineMeshFactory class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-11-20
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version     1.0
 */

#ifndef CPEPEENGINEMESHFACTORY_H
#define CPEPEENGINEMESHFACTORY_H

#include "PepeEngineInit.h"

#include "CPepeEngineMesh.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineMeshFactory
{
public:
    static MeshPtr createPlane(size_t nSize);
    static MeshPtr createSphere(size_t nSize);
    static MeshPtr createTerrain(const tstring& strHightmapName, size_t nScale);
};

_PEPE_ENGINE_END

#endif