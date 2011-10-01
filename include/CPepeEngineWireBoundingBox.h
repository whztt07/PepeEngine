/**
 * Project:     PepeEngine
 * Tier:        Frontend
 * File:        CPepeEngineWireBoundingBox.h
 *
 * @brief       Declaration of CPepeEngineWireBoundingBox class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-11-30
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version     1.0
 */

#ifndef CPEPEENGINEWIREBOUNDINGBOX_H
#define CPEPEENGINEWIREBOUNDINGBOX_H

#include "PepeEngineInit.h"

#include "IPepeEngineSimpleRenderable.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineWireBoundingBox : public IPepeEngineSimpleRenderable
{
public:
    CPepeEngineWireBoundingBox();
    CPepeEngineWireBoundingBox(const CPepeEngineAxisAlignedBox& aabb);
    ~CPepeEngineWireBoundingBox();

    void loadDataFromAABB(const CPepeEngineAxisAlignedBox& aabb);
};

_PEPE_ENGINE_END

#endif