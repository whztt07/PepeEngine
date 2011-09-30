/**
 * Project:		PepeEngine
 * Tier:		Frontend
 * File:		IPepeEngineRenderable.h   
 *
 * @brief		Declaration of IPepeEngineRenderable interface.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-03-05
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef IPEPEENGINERENDERABLE_H
#define IPEPEENGINERENDERABLE_H

#include "PepeEngineInit.h"

#include "CPepeEngineAxisAlignedBox.h"
#include "CPepeEngineEvent.h"

_PEPE_ENGINE_START

class _PepeEngineExport IPepeEngineRenderable : public CPepeEngineObservable
{	
protected:
	mutable CPepeEngineAxisAlignedBox m_AABB;
public:
	virtual ~IPepeEngineRenderable() {}
	virtual void render() const = 0;
	
	virtual const CPepeEngineVector3& getWorldPosition() const = 0;
	virtual const CPepeEngineMatrix4& getWorldTransforms() const = 0;
	
	virtual inline const CPepeEngineAxisAlignedBox& getAABB() const {return m_AABB;}
};

_PEPE_ENGINE_END

#endif