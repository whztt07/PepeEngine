/**
 * Project: PepeEngine
 * Tier:    Frontend
 * File:    CPepeEngineSubEntity.h
 *
 * @brief   Declaration of CPepeEngineSubEntity class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-10-01
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINESUBENTITY_H
#define CPEPEENGINESUBENTITY_H

#include "PepeEngineInit.h"

#include "IPepeEngineRenderable.h"
#include "CPepeEngineSubMesh.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineSubEntity
{
    friend CPepeEngineEntity;

    CPepeEngineSubMesh* m_pSubMesh;
    CPepeEngineEntity*  m_pParentEntity;

    tstring m_strMaterialName;

    CPepeEngineSubEntity(CPepeEngineEntity* pParentEntity, CPepeEngineSubMesh* pSubMesh);
public:

    void render() const;

    void setMaterialName(const tstring &strMaterialName) {
        m_strMaterialName = strMaterialName;
    }
    const tstring& getMaterialName() const {
        return m_strMaterialName;
    }
};

_PEPE_ENGINE_END

#endif