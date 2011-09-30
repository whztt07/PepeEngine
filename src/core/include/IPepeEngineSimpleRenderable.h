/**
 * Project:		PepeEngine
 * Tier:		Frontend
 * File:		IPepeEngineSimpleRenderable.h   
 *
 * @brief		Declaration of IPepeEngineSimpleRenderable interface.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-11-30
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef IPEPEENGINESIMPLERENDERABLE_H
#define IPEPEENGINESIMPLERENDERABLE_H

#include "PepeEngineInit.h"

#include "IPepeEngineRenderable.h"

_PEPE_ENGINE_START

class _PepeEngineExport IPepeEngineSimpleRenderable : public IPepeEngineRenderable
{	
public:	
								IPepeEngineSimpleRenderable();
	virtual 					~IPepeEngineSimpleRenderable();

	void						render() const;
	
	const CPepeEngineVector3&	getWorldPosition() const;
	const CPepeEngineMatrix4&	getWorldTransforms() const;

protected:
	CPepeEngineRenderOperation	m_renderOperation;
	CPepeEngineVector3			m_position;
	CPepeEngineMatrix4			m_transform;
	MaterialPtr		m_pMaterial;
};

_PEPE_ENGINE_END

#endif