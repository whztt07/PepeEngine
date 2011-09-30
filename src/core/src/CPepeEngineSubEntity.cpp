#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineMaterialManager.h"
#include "CPepeEngineSubEntity.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineSubEntity::CPepeEngineSubEntity(CPepeEngineEntity* pParentEntity, CPepeEngineSubMesh* pSubMesh)
{
	m_pParentEntity = pParentEntity;
	m_pSubMesh		= pSubMesh;

	tstring		strNewMatName		= CPepeEngineMaterialManager::getSingleton().generateGUID();
	MaterialPtr pMaterial			= CPepeEngineMaterialManager::getSingleton().create(
		strNewMatName
	);
	MaterialPtr pCurrentMaterial	= CPepeEngineMaterialManager::getSingleton().getByName(
		pSubMesh->getMaterialName()
	);

	*pMaterial = *pCurrentMaterial;

	setMaterialName(strNewMatName);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineSubEntity::render() const
{	
	IPepeEngineRenderer* pRenderer	= CPepeEngineCore::getSingleton().getRenderer();
	MaterialPtr pCurrentMaterial	= CPepeEngineMaterialManager::getSingleton().getByName(
		getMaterialName()
	);
		
	pCurrentMaterial->load();

	pCurrentMaterial->setRenderState();
	CPepeEngineCore::getSingleton().getSceneManager()->setCurrentMaterial(pCurrentMaterial);
			
	if (!pCurrentMaterial->hasGPUProgram())
	{
		if (pCurrentMaterial->getTexture())
		{			
			pRenderer->setTextureUnitState(0, pCurrentMaterial->getTextureUnitState());
			pRenderer->setTexture(pCurrentMaterial->getTexture());			
		} else
		{
			pRenderer->setTexture(TexturePtr(NULL));
		}

		pRenderer->setMaterial(pCurrentMaterial);
		pRenderer->unbindGPUPrograms();
	} else
	{		
		if (pCurrentMaterial->hasVertexShader())
		{
			CPepeEngineGPUProgramParameters* params = pCurrentMaterial->getVertexShader()->getParameters();
			params->updateAutoConstantParameters(
				CPepeEngineCore::getSingleton().getSceneManager()->getAutoParamDataSource()
			);
			params->updateAutoConstantParametersLights(
				CPepeEngineCore::getSingleton().getSceneManager()->getAutoParamDataSource()
			);				
			
			if (CPepeEngineCore::getSingleton().getSceneManager()->hasSkyBox() &&
				params->useSkyBox())
			{
				params->bindTexture(
					_T("skyBox"),
					CPepeEngineCore::getSingleton().getSceneManager()->getSkyBox()->getTexture()
				);
			}

			pCurrentMaterial->getVertexShader()->setProgramSamplers(pCurrentMaterial->getTextureUnitState());

			pRenderer->bindGPUProgramParameters(GPU_VERTEX_SHADER, params);
			pRenderer->bindGPUProgram(pCurrentMaterial->getVertexShader());
		}

		if (pCurrentMaterial->hasPixelShader())
		{
			CPepeEngineGPUProgramParameters* params = pCurrentMaterial->getPixelShader()->getParameters();

			params->updateAutoConstantParameters(
				CPepeEngineCore::getSingleton().getSceneManager()->getAutoParamDataSource()
			);
			params->updateAutoConstantParametersLights(
				CPepeEngineCore::getSingleton().getSceneManager()->getAutoParamDataSource()
			);
			
			if (CPepeEngineCore::getSingleton().getSceneManager()->hasSkyBox() &&
				params->useSkyBox())
			{
				params->bindTexture(
					_T("skyBox"), 
					CPepeEngineCore::getSingleton().getSceneManager()->getSkyBox()->getTexture()
				);
			}

			pCurrentMaterial->getPixelShader()->setProgramSamplers(pCurrentMaterial->getTextureUnitState());

			pRenderer->bindGPUProgramParameters(GPU_PIXEL_SHADER, params);
			pRenderer->bindGPUProgram(pCurrentMaterial->getPixelShader());			
		}
	}
							
	pRenderer->render(m_pSubMesh->getRenderOperation());	
}	

_PEPE_ENGINE_END