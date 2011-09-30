#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineMaterial.h"
#include "IPepeEngineTexture.h"
#include "IPepeEngineGPUProgram.h"
#include "IPepeEngineGPUProgramManager.h"
#include "IPepeEngineTextureManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineMaterial::CPepeEngineMaterial(const tstring& strName) : IPepeEngineResource(strName) 
{
	m_pPSGPUProgram				= NULL;
	m_pVSGPUProgram				= NULL;
	m_pTexture					= NULL;

	m_bIsLoaded					= true;
	m_bHasGPUProgram			= false;
	
	m_cullingMode				= CULL_CLOCKWISE;
	m_shadeMode					= SM_GOURAUD;	
	m_bLightEnabled				= true;	
	m_bSpecularEnabled			= true;	
	m_material.ambient			= CPepeEngineColor::BLACK;
	m_material.specular			= CPepeEngineColor::BLACK;
	m_material.emissive			= CPepeEngineColor::BLACK;
	m_material.diffuse			= CPepeEngineColor::WHITE;
	setSceneBlending(SBT_REPLACE);
}

// -----------------------------------------------------------------------------------------
CPepeEngineMaterial::~CPepeEngineMaterial()
{
	if (isLoaded())
	{
		unload();
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::load()
{
	if (!isLoaded())
	{		
		LOG("Loading material: " + getName());
		if (m_pPSGPUProgram != NULL)
		{
			m_pPSGPUProgram->load();
			m_bHasGPUProgram = true;
		}

		if (m_pVSGPUProgram != NULL)
		{
			m_pVSGPUProgram->load();
			m_bHasGPUProgram = true;
		}

		if (m_pTexture != NULL)
		{
			m_pTexture->load();
		}

		m_bIsLoaded = true;
		LOG("Material successfully loaded");
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::unload()
{	
	m_bHasGPUProgram	= false;
	m_bIsLoaded			= false;	
}

// -----------------------------------------------------------------------------------------
CPepeEngineColor CPepeEngineMaterial::getDiffuse() const
{	
	return m_material.diffuse;	
}

// -----------------------------------------------------------------------------------------
CPepeEngineColor CPepeEngineMaterial::getAmbient() const
{	
	return m_material.ambient;	
}

// -----------------------------------------------------------------------------------------
CPepeEngineColor CPepeEngineMaterial::getSpecular() const
{	
	return m_material.specular;	
}

// -----------------------------------------------------------------------------------------
CPepeEngineColor CPepeEngineMaterial::getEmissive() const
{
	return m_material.emissive;
}

// -----------------------------------------------------------------------------------------
float CPepeEngineMaterial::getPower() const
{	
	return m_material.power;	
}
	
// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setDiffuse(const CPepeEngineColor& diffuse)
{	
	m_material.diffuse = diffuse.saturateCopy();	
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setAmbient(const CPepeEngineColor& ambient)
{	
	m_material.ambient = ambient.saturateCopy();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setSpecular(const CPepeEngineColor& specular)
{
	m_material.specular = specular.saturateCopy();	
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setEmissive(const CPepeEngineColor& emissive)
{	
	m_material.emissive = emissive.saturateCopy();	
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setPower(float fPower)
{	
	m_material.power = fPower;	
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setAlpha(float fAlpha)
{
	m_material.diffuse.a = fAlpha;
	m_material.diffuse.saturate();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setPixelShader(const tstring& strName)
{
	if (CPepeEngineCore::getSingleton().getRenderer()->getRendererCapabilities()->hasCapability(RC_PIXEL_PROGRAM))
	{
		m_bIsLoaded		= false;
		m_pPSGPUProgram	= IPepeEngineGPUProgramManager::getSingleton().create(strName);			
		load();

		if (getTexture() && m_pPSGPUProgram->getParameters()->useModelTexture())
		{
			m_pPSGPUProgram->getParameters()->bindTexture(_T("model_texture"), getTexture());
		}
	} else
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_INVALID_PARAMETERS, 
			_T("This device doesn't support pixel shaders."), 
			_T("CPepeEngineMaterial::setPixelShader")
		);	
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setVertexShader(const tstring& strName)
{
	if (CPepeEngineCore::getSingleton().getRenderer()->getRendererCapabilities()->hasCapability(RC_VERTEX_PROGRAM))
	{
		m_bIsLoaded		= false;
		m_pVSGPUProgram	= IPepeEngineGPUProgramManager::getSingleton().create(strName);			
		load();

		if (getTexture() && m_pVSGPUProgram->getParameters()->useModelTexture())
		{
			m_pVSGPUProgram->getParameters()->bindTexture(_T("model_texture"), getTexture());
		}	
	} else
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("This device doesn't support vertex shaders."), 
			_T("CPepeEngineMaterial::setVertexShader")
		);	
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setTexture(const tstring& strName)
{
	m_pTexture		= IPepeEngineTextureManager::getSingleton().create(strName);			
	m_bIsLoaded		= false;
	m_texUnitState.setTextureType(m_pTexture->getTextureType());
}

// -----------------------------------------------------------------------------------------
CPepeEngineTextureUnitState& CPepeEngineMaterial::getTextureUnitState()
{
	return m_texUnitState;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setLightingEnabled(bool bEnabled)
{
	m_bLightEnabled = bEnabled;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setCullingMode(const CullingMode& mode)
{
	m_cullingMode = mode;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setShadeMode(const ShadeMode& mode)
{
	m_shadeMode = mode;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setSpecularEnabled(bool bEnabled)
{
	m_bSpecularEnabled = bEnabled;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setSceneBlending(const SceneBlendType& blendType)
{
	switch (blendType)
	{
	case SBT_TRANSPARENT_ALPHA:
		m_srcBlendFactor	= SBF_SOURCE_ALPHA;
		m_dstBlendFactor	= SBF_ONE_MINUS_SOURCE_ALPHA;
		return;
	case SBT_TRANSPARENT_COLOUR:
		m_srcBlendFactor	= SBF_SOURCE_COLOUR;
		m_dstBlendFactor	= SBF_ONE_MINUS_SOURCE_COLOUR;
		return;
	case SBT_MODULATE:
		m_srcBlendFactor	= SBF_DEST_COLOUR;
		m_dstBlendFactor	= SBF_ZERO;
		return;
	case SBT_ADD:
		m_srcBlendFactor	= SBF_ONE;
		m_dstBlendFactor	= SBF_ONE;
		return;
	case SBT_REPLACE:
		m_srcBlendFactor	= SBF_ONE;
		m_dstBlendFactor	= SBF_ZERO;
		return;	
	}

	// Default to SBT_REPLACE
	m_srcBlendFactor	= SBF_ONE;
	m_dstBlendFactor	= SBF_ZERO;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setSceneBlending(const SceneBlendFactor& srcFactor, const SceneBlendFactor& destFactor)
{
	m_srcBlendFactor = srcFactor;
	m_dstBlendFactor = destFactor;	
}

// -----------------------------------------------------------------------------------------
void CPepeEngineMaterial::setRenderState()
{
	IPepeEngineRenderer* pRenderer = CPepeEngineCore::getSingleton().getRenderer();

	pRenderer->setCullingMode(m_cullingMode, false);
	pRenderer->setShadeMode(m_shadeMode, false);
	pRenderer->setSpecularEnabled(m_bSpecularEnabled, false);
	pRenderer->setLightingEnabled(m_bLightEnabled, false);
	pRenderer->setSceneBlending(m_srcBlendFactor, m_dstBlendFactor, false);
}

// -----------------------------------------------------------------------------------------
MaterialPtr::MaterialPtr() : CPepeEngineSharedPtr<CPepeEngineMaterial>()
{
}

// -----------------------------------------------------------------------------------------
MaterialPtr::MaterialPtr(CPepeEngineMaterial* r)
	: CPepeEngineSharedPtr<CPepeEngineMaterial>(r)
{
}

// -----------------------------------------------------------------------------------------
MaterialPtr::MaterialPtr(const MaterialPtr& r) 
	: CPepeEngineSharedPtr<CPepeEngineMaterial>(r)
{
}

// -----------------------------------------------------------------------------------------
MaterialPtr::MaterialPtr(const ResourcePtr& r) : CPepeEngineSharedPtr<CPepeEngineMaterial>()
{
	ptr = (CPepeEngineMaterial*)r.data();
	if (ptr) 
	{
		ptr->ref();
	}
}

// -----------------------------------------------------------------------------------------
MaterialPtr& MaterialPtr::operator=(const ResourcePtr& r)
{
	if (ptr == (CPepeEngineMaterial*)r.data()) 
	{
		return *this;
	}
	if (ptr)
	{
		ptr->deref();
	}
	ptr = (CPepeEngineMaterial*)r.data();
	if (ptr) 
	{
		ptr->ref();
	}
	return *this;
}

_PEPE_ENGINE_END