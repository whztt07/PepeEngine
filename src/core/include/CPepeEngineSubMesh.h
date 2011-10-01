/**
 * Project: PepeEngine
 * Tier:    Frontend
 * File:    CPepeEngineSubMesh.h
 *
 * @brief   Declaration of CPepeEngineSubMesh class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-03-19
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINESUBMESH_H
#define CPEPEENGINESUBMESH_H

#include "PepeEngineInit.h"

#include "CPepeEngineRenderOperation.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineSubMesh
{
    friend CPepeEngineMesh;

    CPepeEngineRenderOperation      m_renderOperation;
    CPepeEngineMesh*                m_pParentMesh;
    CPepeEngineIndexData*           m_pIndexData;

    tstring m_strMaterialName;

    CPepeEngineSubMesh(MeshPtr pParentMesh);
public:

    unsigned int m_nFaces;

    ~CPepeEngineSubMesh();

    void setMaterialName(const tstring &strMaterialName) {
        m_strMaterialName = strMaterialName;
    }
    const tstring& getMaterialName() const {
        return m_strMaterialName;
    }

    const CPepeEngineRenderOperation& getRenderOperation() const {
        return m_renderOperation;
    }

    void notifyVertexDataChange();

    void setIndexData(CPepeEngineIndexData* pIndexData);
    CPepeEngineIndexData* getIndexData() const {
        return m_pIndexData;
    }

};

_PEPE_ENGINE_END

#endif