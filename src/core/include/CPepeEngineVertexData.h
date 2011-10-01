/**
* Project:  PepeEngine
* Tier:     Frontend
* File:     CPepeEngineVertexData.h
*
* @brief    Declaration of CPepeEngineVertexData class.
*
* @author       Piotr 'pepe' Picheta
* @date         2008-07-09
* @copyright    Copyright (c) 2008 Piotr Picheta
*
* @version 1.0
*/

#ifndef CPEPEENGINEVERTEXDATA_H
#define CPEPEENGINEVERTEXDATA_H

#include "PepeEngineInit.h"

#include "IPepeEngineVertexBuffer.h"
#include "CPepeEngineVertexDeclaration.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineVertexData
{
public:

    CPepeEngineVertexData();
    ~CPepeEngineVertexData();

    size_t  m_nVertexStart;
    size_t  m_nVertexCount;

    CPepeEngineVertexDeclaration*   m_pVertexDeclaration;
    IPepeEngineVertexBuffer*        m_pVertexBuffer;
};

_PEPE_ENGINE_END

#endif