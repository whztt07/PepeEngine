/**
 * Project:		PepeEngine
 * Tier:		Frontend 
 * File:		CPepeEngineMaterial.h 
 *
 * @brief		Declaration of CPepeEngineMaterial class.  
 *			
 * @author		Piotr 'pepe' Picheta
 * @date		2008-02-28
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef CPEPEENGINEMATERIAL_H
#define CPEPEENGINEMATERIAL_H

#include "PepeEngineInit.h"

#include "CPepeEngineColor.h"
#include "CPepeEngineTextureUnitState.h"
#include "IPepeEngineGPUProgram.h"
#include "IPepeEngineResource.h"

_PEPE_ENGINE_START

/** 
 *	Types of blending that you can specify between an object and the existing contents of the scene.
 */
enum SceneBlendType
{
	SBT_TRANSPARENT_ALPHA,		/**<	Make the object transparent based on the final alpha values in the texture						*/
	SBT_TRANSPARENT_COLOUR,		/**<	Make the object transparent based on the colour values in the texture (brighter = more opaque)	*/	
	SBT_ADD,					/**<	Add the texture values to the existing scene content											*/
	SBT_MODULATE,				/**<	Multiply the 2 colours together																	*/
	SBT_REPLACE					/**<	The default blend mode where source replaces destination										*/
};

/** 
 *	Blending factors for manually blending objects with the scene. 
 */
enum SceneBlendFactor
{
	SBF_ONE,
	SBF_ZERO,
	SBF_DEST_COLOUR,
	SBF_SOURCE_COLOUR,
	SBF_ONE_MINUS_DEST_COLOUR,
	SBF_ONE_MINUS_SOURCE_COLOUR,
	SBF_DEST_ALPHA,
	SBF_SOURCE_ALPHA,
	SBF_ONE_MINUS_DEST_ALPHA,
	SBF_ONE_MINUS_SOURCE_ALPHA
};

enum CullingMode
{
	CULL_NONE			= 1,	/**<	Hardware never culls triangles and renders everything it receives.					*/	
	CULL_CLOCKWISE		= 2,	/**<	Hardware culls triangles whose vertices are listed clockwise in the view (default).	*/
	CULL_ANTICLOCKWISE	= 3		/**<	Hardware culls triangles whose vertices are listed anticlockwise in the view.		*/
};

enum ShadeMode
{
	SM_FLAT,
	SM_GOURAUD,
	SM_PHONG
};

class _PepeEngineExport CPepeEngineMaterial : public IPepeEngineResource
{	
	typedef struct
	{
		CPepeEngineColor	diffuse;        /**< Diffuse color RGBA					*/
		CPepeEngineColor	ambient;        /**< Ambient color RGB					*/
		CPepeEngineColor	specular;       /**< Specular 'shininess'				*/
		CPepeEngineColor	emissive;       /**< Emissive color RGB					*/
		float				power;          /**< Sharpness if specular highlight	*/
	} Material;

	Material m_material;

	tstring m_strPSName;
	tstring m_strVSName;
	tstring m_strTextureName;

	GPUProgramPtr	m_pPSGPUProgram;
	GPUProgramPtr	m_pVSGPUProgram;
	TexturePtr		m_pTexture;
	
	CPepeEngineTextureUnitState m_texUnitState;

	bool m_bHasGPUProgram;

	/** @{	Render States	*/
	CullingMode			m_cullingMode;
	ShadeMode			m_shadeMode;	
	bool				m_bLightEnabled;	
	bool				m_bSpecularEnabled;		
	SceneBlendFactor	m_srcBlendFactor;
	SceneBlendFactor	m_dstBlendFactor;
	/** @}					*/
public:
	CPepeEngineMaterial(const tstring& strName);
	~CPepeEngineMaterial();

	void load();	
	void unload();

	CPepeEngineColor getDiffuse() const;
	CPepeEngineColor getAmbient() const;
	CPepeEngineColor getSpecular() const;
	CPepeEngineColor getEmissive() const;
	float getPower() const;
	
	void setDiffuse(const CPepeEngineColor& diffuse);
	void setAmbient(const CPepeEngineColor& ambient);
	void setSpecular(const CPepeEngineColor& specular);
	void setEmissive(const CPepeEngineColor& emissive);
	void setPower(float fPower);
	void setAlpha(float fAlpha);

	void setPixelShader(const tstring& strName);
	void setVertexShader(const tstring& strName);
	void setTexture(const tstring& strName);
	
	GPUProgramPtr getPixelShader() const {return m_pPSGPUProgram;}
	GPUProgramPtr getVertexShader() const {return m_pVSGPUProgram;}	
	TexturePtr getTexture() const {return m_pTexture;}

	bool hasGPUProgram() const {return m_bHasGPUProgram;}
	bool hasVertexShader() const {return m_pVSGPUProgram != NULL;}
	bool hasPixelShader() const {return m_pPSGPUProgram != NULL;}

	inline CPepeEngineTextureUnitState& getTextureUnitState();

	void setLightingEnabled(bool bEnabled);
	void setCullingMode(const CullingMode& mode);
	void setShadeMode(const ShadeMode& mode);	
	void setSpecularEnabled(bool bEnabled);
	void setSceneBlending(const SceneBlendType& blendType);
	void setSceneBlending(const SceneBlendFactor& srcFactor, const SceneBlendFactor& destFactor);

	void setRenderState();
};

struct _PepeEngineExport MaterialPtr : public CPepeEngineSharedPtr<CPepeEngineMaterial>
{
public:
	inline				MaterialPtr();
	inline explicit		MaterialPtr(CPepeEngineMaterial* r);
	inline				MaterialPtr(const MaterialPtr& r);
	inline				MaterialPtr(const ResourcePtr& r); 
	inline MaterialPtr&	operator=(const ResourcePtr& r);
};

_PEPE_ENGINE_END

#endif