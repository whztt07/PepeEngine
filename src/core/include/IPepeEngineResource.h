/**
 * Project:     PepeEngine
 * Tier:        Frontend
 * File:        IPepeEngineResource.h
 *
 * @brief       Declaration of IPepeEngineResource class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-04-23
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version     1.0
 */

#ifndef IPEPEENGINERESOURCE_H
#define IPEPEENGINERESOURCE_H

#include "PepeEngineInit.h"

#include "CPepeEngineSharedPtr.h"

_PEPE_ENGINE_START

class _PepeEngineExport IPepeEngineResource : public CPepeEngineShared
{
public:

    inline                  IPepeEngineResource(tstring strName);
    inline virtual          ~IPepeEngineResource();

    virtual void            load() = 0;
    virtual void            unload() = 0;

    inline bool             isLoaded() const;
    inline void             setLoaded(bool bLoaded);

    inline const tstring&   getName() const;

protected:
    bool                    m_bIsLoaded;
    tstring                 m_strName;
};

typedef CPepeEngineSharedPtr<IPepeEngineResource> ResourcePtr;

_PEPE_ENGINE_END

#endif