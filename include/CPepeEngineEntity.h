/**
 * Project:		PepeEngine
 * Tier:		Frontend
 * File:		CPepeEngineEntity.h   
 *
 * @brief		Declaration of CPepeEngineEntity class.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-07-28
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef CPEPEENGINEENTITY_H
#define CPEPEENGINEENTITY_H

#include "PepeEngineInit.h"

#include "CPepeEngineMaterial.h"
#include "CPepeEngineMatrix4.h"
#include "CPepeEngineMesh.h"
#include "CPepeEngineQuaternion.h"
#include "CPepeEngineSubEntity.h"
#include "IPepeEngineRenderable.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineEntity : public IPepeEngineRenderable
{	
public:	
												CPepeEngineEntity(const tstring& strName);
												CPepeEngineEntity(const tstring& strName, const tstring& strMeshName);
												CPepeEngineEntity(const tstring& strName, MeshPtr pMesh);

	virtual										~CPepeEngineEntity();
	
	void										makeTransforms() const;

	inline const CPepeEngineVector3&			getWorldPosition() const;
	inline const CPepeEngineMatrix4&			getWorldTransforms() const;

	virtual void								render() const;

	inline void									setPosition(const CPepeEngineVector3& position) const;
	inline void									setPosition(float x, float y, float z) const;
	inline void									setTranslation(const CPepeEngineVector3& translation) const;
	inline void									setTranslation(float x, float y, float z) const;
	inline void									setScale(const CPepeEngineVector3& scale) const;
	inline void									setScale(float x, float y, float z) const;
	inline void									setRotation(const CPepeEngineQuaternion& rotation) const;
	inline void									setRotation(float angle, const CPepeEngineVector3& axis) const;

	inline CPepeEngineVector3&					getTranslation() const;
	inline CPepeEngineVector3&					getScale() const;
	inline CPepeEngineQuaternion&				getRotation() const;
	
	void										setPixelShader(const tstring &strGPUProgramName);
	void										setVertexShader(const tstring &strGPUProgramName);

	void										setLightingEnabled(bool bEnabled);
	void										setCullingMode(const CullingMode& mode);
	void										setShadeMode(const ShadeMode& mode);	
	void										setSpecularEnabled(bool bEnabled);
	void										setSceneBlending(const SceneBlendType& blendType);
	void										setSceneBlending(
													const SceneBlendFactor& srcFactor, 
													const SceneBlendFactor& destFactor);

	void										setAlpha(float fAlpha);

	inline MeshPtr								getMesh() const;
	void										addSubEntity(CPepeEngineSubMesh *pSubMesh);

	inline const tstring&						getName() const;	
protected:
	typedef std::vector<CPepeEngineSubEntity*>	SubEntityList;

	SubEntityList								m_subEntityList;

	mutable CPepeEngineMatrix4					m_transformMatrix;
	mutable CPepeEngineVector3					m_position;
	mutable CPepeEngineVector3					m_translation;
	mutable CPepeEngineVector3					m_scale;
	mutable CPepeEngineQuaternion				m_rotation;
		
	MeshPtr										m_pMesh;

	tstring										m_strName;
};

_PEPE_ENGINE_END

#endif