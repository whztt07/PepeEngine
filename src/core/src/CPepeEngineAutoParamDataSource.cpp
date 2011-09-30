#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineAutoParamDataSource.h"
#include "IPepeEngineRenderable.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineAutoParamDataSource::CPepeEngineAutoParamDataSource()
{
	m_bWorldMatrixChanged				= true;
	m_bViewMatrixChanged				= true;
	m_bProjectionMatrixChanged			= true;
	m_bWorldViewMatrixChanged			= true;
	m_bWorldViewProjectionMatrixChanged	= true;
	m_bViewProjectionMatrixChanged		= true;
	m_pCurrentLightList					= NULL;
	m_pCamera							= NULL;
	m_pCurrentRenderable				= NULL;
	m_pBlankLight						= new CPepeEngineLight(_T("blank light"));
}

// -----------------------------------------------------------------------------------------
CPepeEngineAutoParamDataSource::~CPepeEngineAutoParamDataSource()
{
	delete m_pBlankLight;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAutoParamDataSource::setCurrentRenderable(const IPepeEngineRenderable* pRenderable)
{
	m_bWorldMatrixChanged				= true;	
	m_bWorldViewProjectionMatrixChanged = true;
	m_bWorldViewMatrixChanged			= true;
	m_pCurrentRenderable				= pRenderable;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAutoParamDataSource::setWorldMatrix(const CPepeEngineMatrix4& matrix) const 
{
	m_bWorldMatrixChanged	= true; 
	m_worldMatrix			= matrix;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineMatrix4& CPepeEngineAutoParamDataSource::getWorldMatrix() const 
{
	if (m_bWorldMatrixChanged && (m_pCurrentRenderable != NULL))
	{
		m_worldMatrix			= m_pCurrentRenderable->getWorldTransforms();
		m_bWorldMatrixChanged	= false;
	}
	return m_worldMatrix;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAutoParamDataSource::setViewMatrix(const CPepeEngineMatrix4& matrix) const 
{
	m_bViewMatrixChanged			= true;
	m_bViewProjectionMatrixChanged	= true;
	m_viewMatrix					= matrix;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineMatrix4& CPepeEngineAutoParamDataSource::getViewMatrix() const 
{
	if (m_bViewMatrixChanged && (m_pCamera != NULL))
	{
		m_viewMatrix			= m_pCamera->getTransformMatrix();
		m_bViewMatrixChanged	= false;
	}
	return m_viewMatrix;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAutoParamDataSource::setProjectionMatrix(const CPepeEngineMatrix4& matrix) const 
{
	m_bViewProjectionMatrixChanged	= true;
	m_bProjectionMatrixChanged		= true;
	m_projectionMatrix				= matrix;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineMatrix4& CPepeEngineAutoParamDataSource::getProjectionMatrix() const 
{
	if (m_bProjectionMatrixChanged && (m_pCamera != NULL))
	{
		m_projectionMatrix			= m_pCamera->getProjectionMatrix();
		m_bProjectionMatrixChanged	= false;
	}
	return m_projectionMatrix;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineMatrix4& CPepeEngineAutoParamDataSource::getWorldViewProjectionMatrix() const
{
	if (m_bWorldViewProjectionMatrixChanged)
	{
		m_worldViewProjectionMatrix			= getProjectionMatrix() * getWorldViewMatrix();		
		m_bWorldViewProjectionMatrixChanged = false;
	}
	return m_worldViewProjectionMatrix;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineMatrix4& CPepeEngineAutoParamDataSource::getViewProjectionMatrix() const
{
	if (m_bViewProjectionMatrixChanged)
	{
		m_viewProjectionMatrix			= getProjectionMatrix().concatenate(getViewMatrix());
		m_bViewProjectionMatrixChanged	= false;
	}
	return m_viewProjectionMatrix;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineMatrix4& CPepeEngineAutoParamDataSource::getWorldViewMatrix() const
{
	if (m_bWorldViewMatrixChanged)
	{
		m_worldViewMatrix			= getViewMatrix().concatenateAffine(getWorldMatrix());
		m_bWorldViewMatrixChanged	= false;
	}
	return m_worldViewMatrix;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAutoParamDataSource::setMaterialAmbientColor(const CPepeEngineColor& color) const 
{
	m_materialAmbientColor = color;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineColor& CPepeEngineAutoParamDataSource::getMaterialAmbientColor() const 
{
	return m_materialAmbientColor;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAutoParamDataSource::setMaterialDiffuseColor(const CPepeEngineColor& color) const 
{
	m_materialDiffuseColor = color;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineColor& CPepeEngineAutoParamDataSource::getMaterialDiffuseColor() const 
{
	return m_materialDiffuseColor;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAutoParamDataSource::setMaterialSpecularColor(const CPepeEngineColor& color) const 
{
	m_materialSpecularColor = color;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineColor& CPepeEngineAutoParamDataSource::getMaterialSpecularColor() const 
{
	return m_materialSpecularColor;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAutoParamDataSource::setMaterialEmissiveColor(const CPepeEngineColor& color) const 
{
	m_materialEmissiveColor = color;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineColor& CPepeEngineAutoParamDataSource::getMaterialEmissiveColor() const 
{
	return m_materialEmissiveColor;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAutoParamDataSource::setMaterialPower(const float fPower) const 
{
	m_fMaterialPower = fPower;
}

// -----------------------------------------------------------------------------------------
const float CPepeEngineAutoParamDataSource::getMaterialPower() const 
{
	return m_fMaterialPower;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAutoParamDataSource::setAmbientLightColor(const CPepeEngineColor& color) const 
{
	m_ambientLightColor = color;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineColor& CPepeEngineAutoParamDataSource::getAmbientLightColor() const 
{
	return m_ambientLightColor;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAutoParamDataSource::setLightList(const LightList* pLightList) const 
{
	m_pCurrentLightList = pLightList;
}

// -----------------------------------------------------------------------------------------
const LightList* CPepeEngineAutoParamDataSource::getLightList() const 
{
	return m_pCurrentLightList;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineLight* CPepeEngineAutoParamDataSource::getLight(size_t nIndex) const
{		 
	nIndex = (nIndex - 1 < 0) ? 0 : nIndex - 1;	
	if (m_pCurrentLightList->size() <= nIndex)
	{
		return m_pBlankLight;
	}
	else
	{
		return (*m_pCurrentLightList)[nIndex];
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineAutoParamDataSource::setCamera(CPepeEngineCamera* pCamera) const
{
	m_pCamera							= pCamera;
	m_bWorldViewProjectionMatrixChanged = true;
	m_bViewProjectionMatrixChanged		= true;
	m_bWorldViewMatrixChanged			= true;
	m_bViewMatrixChanged				= true;
	m_bProjectionMatrixChanged			= true;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineCamera* CPepeEngineAutoParamDataSource::getCamera() const
{
	return m_pCamera;
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAutoParamDataSource::getTime(void) const
{
	return CPepeEngineCore::getSingleton().getElapsedTime();
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAutoParamDataSource::getTime_0_X(float x) const
{
	return fmod(this->getTime(), x);
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAutoParamDataSource::getCosTime_0_X(float x) const
{ 
	return cos(this->getTime_0_X(x)); 
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAutoParamDataSource::getSinTime_0_X(float x) const
{ 
	return sin(this->getTime_0_X(x)); 
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAutoParamDataSource::getTanTime_0_X(float x) const
{ 
	return tan(this->getTime_0_X(x)); 
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector4 CPepeEngineAutoParamDataSource::getTime_0_X_packed(float x) const
{
	float t = this->getTime_0_X(x);
	return CPepeEngineVector4(t, sin(t), cos(t), tan(t));
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAutoParamDataSource::getTime_0_1(float x) const
{ 
	return this->getTime_0_X(x)/x; 
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAutoParamDataSource::getCosTime_0_1(float x) const
{ 
	return cos(this->getTime_0_1(x)); 
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAutoParamDataSource::getSinTime_0_1(float x) const
{ 
	return sin(this->getTime_0_1(x)); 
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAutoParamDataSource::getTanTime_0_1(float x) const
{ 
	return tan(this->getTime_0_1(x)); 
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector4 CPepeEngineAutoParamDataSource::getTime_0_1_packed(float x) const
{
	float t = this->getTime_0_1(x);
	return CPepeEngineVector4(t, sin(t), cos(t), tan(t));
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAutoParamDataSource::getTime_0_2Pi(float x) const
{ 
	return this->getTime_0_X(x) / x * 2 * CPepeEngineMath::PI; 
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAutoParamDataSource::getCosTime_0_2Pi(float x) const
{ 
	return cos(this->getTime_0_2Pi(x)); 
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAutoParamDataSource::getSinTime_0_2Pi(float x) const
{ 
	return sin(this->getTime_0_2Pi(x)); 
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAutoParamDataSource::getTanTime_0_2Pi(float x) const
{ 
	return tan(this->getTime_0_2Pi(x)); 
}

// -----------------------------------------------------------------------------------------
CPepeEngineVector4 CPepeEngineAutoParamDataSource::getTime_0_2Pi_packed(float x) const
{
	float t = this->getTime_0_2Pi(x);
	return CPepeEngineVector4(t, sin(t), cos(t), tan(t));
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAutoParamDataSource::getFrameTime(void) const
{
	return CPepeEngineCore::getSingleton().getFrameTime();
}

// -----------------------------------------------------------------------------------------
float CPepeEngineAutoParamDataSource::getFPS() const
{
	return CPepeEngineCore::getSingleton().getSceneManager()->getFrameStats().fLastFPS;
}

_PEPE_ENGINE_END