/**
 * Project:		PepeEngine
 * Tier:		Frontend
 * File:		CPepeEngineAutoParamDataSource.h 
 *
 * @brief		Declaration of CPepeEngineAutoParamDataSource class.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-09-22
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef CPEPEENGINEAUTOPARAMDATASOURCE_H
#define CPEPEENGINEAUTOPARAMDATASOURCE_H

#include "PepeEngineInit.h"

#include "CPepeEngineCamera.h"
#include "CPepeEngineColor.h"
#include "CPepeEngineLight.h"
#include "CPepeEngineVector4.h"

_PEPE_ENGINE_START

using namespace std;

class _PepeEngineExport CPepeEngineAutoParamDataSource
{			
public:		
											CPepeEngineAutoParamDataSource();
											~CPepeEngineAutoParamDataSource();

	void									setCurrentRenderable(const IPepeEngineRenderable* pRenderable);
	void									setWorldMatrix(const CPepeEngineMatrix4& matrix) const;
	inline const CPepeEngineMatrix4&		getWorldMatrix() const;
	void									setViewMatrix(const CPepeEngineMatrix4& matrix) const ;
	inline const CPepeEngineMatrix4&		getViewMatrix() const;
	void									setProjectionMatrix(const CPepeEngineMatrix4& matrix) const;
	inline const CPepeEngineMatrix4&		getProjectionMatrix() const;
	inline const CPepeEngineMatrix4&		getWorldViewProjectionMatrix() const;
	inline const CPepeEngineMatrix4&		getViewProjectionMatrix() const;
	inline const CPepeEngineMatrix4&		getWorldViewMatrix() const;
	
	inline void								setMaterialAmbientColor(const CPepeEngineColor& color) const;
	inline const CPepeEngineColor&			getMaterialAmbientColor() const;

	inline void								setMaterialDiffuseColor(const CPepeEngineColor& color) const;
	inline const CPepeEngineColor&			getMaterialDiffuseColor() const;

	inline void								setMaterialSpecularColor(const CPepeEngineColor& color) const;
	inline const CPepeEngineColor&			getMaterialSpecularColor() const;

	inline void								setMaterialEmissiveColor(const CPepeEngineColor& color) const;
	inline const CPepeEngineColor&			getMaterialEmissiveColor() const;
	
	inline void								setMaterialPower(const float fPower) const;
	inline const float						getMaterialPower() const;
	
	inline void								setAmbientLightColor(const CPepeEngineColor& color) const;
	inline const CPepeEngineColor&			getAmbientLightColor() const;

	void									setLightList(const LightList* pLightList) const;
	inline const LightList*					getLightList() const;
	inline const CPepeEngineLight*			getLight(size_t nIndex) const;
	
	inline void								setCamera(CPepeEngineCamera* pCamera) const;
	inline const CPepeEngineCamera*			getCamera() const;

	inline float							getTime(void) const;
	inline float							getTime_0_X(float x) const;
	inline float							getCosTime_0_X(float x) const;
	inline float							getSinTime_0_X(float x) const;
	inline float							getTanTime_0_X(float x) const;
	inline CPepeEngineVector4				getTime_0_X_packed(float x) const;
	inline float							getTime_0_1(float x) const;
	inline float							getCosTime_0_1(float x) const;
	inline float							getSinTime_0_1(float x) const;
	inline float							getTanTime_0_1(float x) const;
	inline CPepeEngineVector4				getTime_0_1_packed(float x) const;
	inline float							getTime_0_2Pi(float x) const;
	inline float							getCosTime_0_2Pi(float x) const;
	inline float							getSinTime_0_2Pi(float x) const;
	inline float							getTanTime_0_2Pi(float x) const;
	inline CPepeEngineVector4				getTime_0_2Pi_packed(float x) const;
	inline float							getFrameTime(void) const;
	inline float							getFPS() const;
private:

	CPepeEngineLight*						m_pBlankLight;

	mutable CPepeEngineMatrix4				m_worldMatrix;
	mutable CPepeEngineMatrix4				m_viewMatrix;
	mutable CPepeEngineMatrix4				m_projectionMatrix;

	mutable CPepeEngineMatrix4				m_worldViewProjectionMatrix;
	mutable CPepeEngineMatrix4				m_worldViewMatrix;
	mutable CPepeEngineMatrix4				m_viewProjectionMatrix;

	mutable CPepeEngineColor				m_materialAmbientColor;
	mutable CPepeEngineColor				m_materialDiffuseColor;
	mutable CPepeEngineColor				m_materialSpecularColor;
	mutable CPepeEngineColor				m_materialEmissiveColor;
	mutable float							m_fMaterialPower;

	mutable CPepeEngineColor				m_ambientLightColor;
	mutable const LightList*				m_pCurrentLightList;
	mutable CPepeEngineCamera*				m_pCamera;

	mutable const IPepeEngineRenderable*	m_pCurrentRenderable;

	mutable bool							m_bWorldMatrixChanged;	
	mutable bool							m_bProjectionMatrixChanged;
	mutable bool							m_bViewMatrixChanged;
	mutable bool							m_bWorldViewMatrixChanged;
	mutable bool							m_bWorldViewProjectionMatrixChanged;
	mutable bool							m_bViewProjectionMatrixChanged;
};

_PEPE_ENGINE_END

#endif