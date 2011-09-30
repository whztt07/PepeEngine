#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineLight.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineLight::CPepeEngineLight(const tstring& strName)
{
	m_lightType				= LT_DIRECTIONAL;
	m_fRange				= 0.0f; 
    m_fFalloff				= 0.0f; 
    m_fConstantAttenuation	= 0.0f; 
    m_fLinearAttenuation	= 0.0f; 
    m_fQuadricAttenuation	= 0.0f; 
	m_bVisible				= true;
	m_bNeedUpdate			= true;
	m_pParentNode			= NULL;

	m_diffuse.set(1.0f, 1.0f, 1.0f);
	m_specular.set(0.0f, 0.0f, 0.0f);

	setName(strName);
}

// -----------------------------------------------------------------------------------------
CPepeEngineLight::~CPepeEngineLight()
{
	m_pParentNode = NULL;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setLightType(const LightType& lightType) 
{
	m_lightType = lightType;
}

// -----------------------------------------------------------------------------------------
const LightType& CPepeEngineLight::getLightType() const 
{
	return m_lightType;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setDiffuse(const CPepeEngineColor& diffuse)
{
	m_diffuse = diffuse;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setDiffuse(float r, float g, float b) 
{
	setDiffuse(CPepeEngineColor(r, g, b));
}

// -----------------------------------------------------------------------------------------
const CPepeEngineColor& CPepeEngineLight::getDiffuse() const 
{
	return m_diffuse;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setSpecular(const CPepeEngineColor& specular) 
{
	m_specular = specular;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setSpecular(float r, float g, float b) 
{
	setSpecular(CPepeEngineColor(r, g, b));
}

// -----------------------------------------------------------------------------------------
const CPepeEngineColor&	CPepeEngineLight::getSpecular() const 
{
	return m_specular;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setAmbient(const CPepeEngineColor& ambient) 
{
	m_ambient = ambient;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setAmbient(float r, float g, float b) 
{
	setAmbient(CPepeEngineColor(r, g, b));
}

// -----------------------------------------------------------------------------------------
const CPepeEngineColor&	CPepeEngineLight::getAmbient() const 
{
	return m_ambient;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setPosition(const CPepeEngineVector3& position) 
{
	m_position = position;
	m_bNeedUpdate = true;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setPosition(float x, float y, float z) 
{
	setPosition(CPepeEngineVector3(x, y, z));	
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVector3& CPepeEngineLight::getPosition() const 
{
	return m_position;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setDirection(const CPepeEngineVector3& direction) 
{
	m_direction = direction; 
	m_direction.normalise(); 
	m_bNeedUpdate = true;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setDirection(float x, float y, float z) 
{
	setDirection(CPepeEngineVector3(x, y, z));
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVector3& CPepeEngineLight::getDirection() const 
{
	return m_direction;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVector3& CPepeEngineLight::getDerivedPosition() const
{
	update();
	return m_derivedPosition;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVector3& CPepeEngineLight::getDerivedDirection() const
{
	update();
	return m_derivedDirection;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setRange(float fRange) 
{
	m_fRange = fRange;
}

// -----------------------------------------------------------------------------------------
float CPepeEngineLight::getRange() const 
{
	return m_fRange;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setFalloff(float fFalloff) 
{
	m_fFalloff = fFalloff;
}

// -----------------------------------------------------------------------------------------
float CPepeEngineLight::getFalloff() const
{
	return m_fFalloff;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setConstantAttenuation(float fConstantAttenuation) 
{
	m_fConstantAttenuation = fConstantAttenuation;
}

// -----------------------------------------------------------------------------------------
float CPepeEngineLight::getConstantAttenuation() const 
{
	return m_fConstantAttenuation;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setLinearAttenuation(float fLinearAttenuation) 
{
	m_fLinearAttenuation = fLinearAttenuation;
}

// -----------------------------------------------------------------------------------------
float CPepeEngineLight::getLinearAttenuation() const 
{
	return m_fLinearAttenuation;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setQuadricAttenuation(float fQuadricAttenuation) 
{
	m_fQuadricAttenuation = fQuadricAttenuation;
}

// -----------------------------------------------------------------------------------------
float CPepeEngineLight::getQuadricAttenuation() const 
{
	return m_fQuadricAttenuation;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setTheta(const Radian& theta) 
{
	m_theta = theta;
}

// -----------------------------------------------------------------------------------------
const Radian& CPepeEngineLight::getTheta() const 
{
	return m_theta;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setPhi(const Radian& phi) 
{
	m_phi = phi;
}

// -----------------------------------------------------------------------------------------
const Radian& CPepeEngineLight::getPhi() const 
{
	return m_phi;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineLight::isVisible() const 
{
	return m_bVisible;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setVisible(bool bVisible) 
{
	m_bVisible = bVisible;
}

// -----------------------------------------------------------------------------------------
const tstring& CPepeEngineLight::getName() 
{
	return m_strName;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setName(const tstring& strName) 
{
	m_strName = strName;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::setSceneNode(CPepeEngineSceneNode* pSceneNode)
{
	m_pParentNode = pSceneNode;
}

// -----------------------------------------------------------------------------------------
CPepeEngineSceneNode* CPepeEngineLight::getSceneNode() const
{
	return m_pParentNode;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineLight::update(bool bForceUpdate) const
{
	if (m_bNeedUpdate || bForceUpdate)
	{
		if (m_pParentNode)
		{
			const CPepeEngineQuaternion&	parentOrientation	= m_pParentNode->getDerivedOrientation();
			const CPepeEngineVector3&		parentPosition		= m_pParentNode->getDerivedTranslation();

			m_derivedDirection	= parentOrientation * m_direction;
			m_derivedPosition	= (parentOrientation * m_position) + parentPosition;
		} else
		{
			m_derivedPosition	= m_position;
			m_derivedDirection	= m_direction;
		}
		m_bNeedUpdate = false;
	}
}

_PEPE_ENGINE_END