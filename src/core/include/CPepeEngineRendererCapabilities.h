/**
 * Project: PepeEngine
 * Tier:	Frontend 
 * File:	CPepeEngineRendererCapabilities.h 
 *
 * @brief	Declaration of CPepeEngineRendererCapabilities class.  
 *			
 * @author		Piotr 'pepe' Picheta
 * @date		2008-10-13
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINERENDERERCAPABILITIES_H
#define CPEPEENGINERENDERERCAPABILITIES_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

/**
 *	Enum describing the different hardware capabilities we want to check for
 */
enum Capabilities
{	
	RC_AUTOMIPMAP				    = 0x00000002,	/**<	Supports generating mipmaps in hardware								*/
	RC_BLENDING						= 0x00000004,	/**<	Supports blending between stages									*/
	RC_ANISOTROPY					= 0x00000008,	/**<	Supports anisotropic texture filtering								*/	
	RC_DOT3							= 0x00000010,	/**<	Supports fixed-function DOT3 texture blend							*/	
	RC_CUBEMAPPING					= 0x00000020,	/**<	Supports cube mapping												*/	
	RC_HWSTENCIL					= 0x00000040,	/**<	Supports hardware stencil buffer									*/	
	RC_VBO							= 0x00000080,	/**<	Supports hardware vertex and index buffers							*/	
	RC_VERTEX_PROGRAM				= 0x00000200,	/**<	Supports vertex programs (vertex shaders)							*/	
	RC_PIXEL_PROGRAM				= 0x00000400,	/**<	Supports pixel programs (pixel shaders)								*/
	RC_TEXTURE_COMPRESSION			= 0x00000800,	/**<	Supports compressed textures										*/	
	RC_TEXTURE_COMPRESSION_DXT		= 0x00001000,	/**<	Supports compressed textures in the DXT/ST3C formats				*/
	RC_TEXTURE_COMPRESSION_VTC		= 0x00002000,	/**<	Supports compressed textures in the VTC format						*/
	RC_SCISSOR_TEST					= 0x00004000,	/**<	Supports performing a scissor test to exclude areas of the screen	*/
	RC_TWO_SIDED_STENCIL			= 0x00008000,	/**<	Supports separate stencil updates for both front and back faces		*/	
	RC_STENCIL_WRAP					= 0x00010000,	/**<	Supports wrapping the stencil value at the range extremeties		*/
	RC_HWOCCLUSION					= 0x00020000,	/**<	Supports hardware occlusion queries									*/	
	RC_USER_CLIP_PLANES				= 0x00040000,	/**<	Supports user clipping planes										*/	
	RC_VERTEX_FORMAT_UBYTE4			= 0x00080000,	/**<	Supports the VET_UBYTE4 vertex element type							*/
	RC_INFINITE_FAR_PLANE			= 0x00100000,	/**<	Supports infinite far plane projection								*/			
	RC_HWRENDER_TO_TEXTURE			= 0x00200000,	/**<	Supports hardware render-to-texture (bigger than framebuffer)		*/
	RC_TEXTURE_FLOAT				= 0x00400000,	/**<	Supports float textures and render targets							*/
	RC_NON_POWER_OF_2_TEXTURES		= 0x00800000,	/**<	Supports non-power of two textures									*/	
	RC_TEXTURE_3D					= 0x01000000,	/**<	Supports 3d (volume) textures										*/
	RC_POINT_SPRITES				= 0x02000000,	/**<	Supports basic point sprite rendering								*/	
	RC_POINT_EXTENDED_PARAMETERS	= 0x04000000,	/**<	Supports extra point parameters (minsize, maxsize, attenuation)		*/
	RC_VERTEX_TEXTURE_FETCH			= 0x08000000,	/**<	Supports vertex texture fetch										*/	 	
	RC_MIPMAP_LOD_BIAS				= 0x10000000	/**<	Supports mipmap LOD biasing											*/
};

/** 
 *	Class for storing the capabilities of the graphics card. 
 *    
 *	This class _PepeEngineExport stores the capabilities of the graphics card. This
 *	information is set by the individual render systems.
 */
class _PepeEngineExport CPepeEngineRendererCapabilities
{		
	unsigned short	m_nNumWorldMatrices;					/**<	The number of world matricies available							*/	
	unsigned short	m_nNumTextureUnits;						/**<	The number of texture units available							*/	
	unsigned short	m_nStencilBufferBitDepth;				/**<	The stencil buffer bit depth									*/	
	unsigned short	m_nNumVertexBlendMatrices;				/**<	The number of matrices available for hardware blending			*/
	int				m_nCapabilities;						/**<	Stores the capabilities flags.									*/
	tstring			m_strMaxVertexProgramVersion;			/**<	The best vertex program that this card / rendersystem supports	*/
	tstring			m_strMaxPixelProgramVersion;			/**<	The best pixel program that this card / rendersystem supports	*/	
	unsigned short	m_nVertexProgramConstantFloatCount;		/**<	The number of floating-point constants vertex programs support	*/
	unsigned short	m_nVertexProgramConstantIntCount;		/**<	The number of integer constants vertex programs support			*/
	unsigned short	m_nVertexProgramConstantBoolCount;		/**<	The number of boolean constants vertex programs support			*/
	unsigned short	m_nPixelProgramConstantFloatCount;		/**<	The number of floating-point constants pixel programs support	*/
	unsigned short	m_nPixelProgramConstantIntCount;		/**<	The number of integer constants pixel programs support			*/
	unsigned short	m_nPixelProgramConstantBoolCount;		/**<	The number of boolean constants pixel programs support			*/
	unsigned short	m_nNumMultiRenderTargets;				/**<	The number of simultaneous render targets supported				*/
	float			m_fMaxPointSize;						/**<	The maximum point size											*/
	bool			m_bNonPOW2TexturesLimited;				/**<	Are non-POW2 textures feature-limited?							*/	
	unsigned short	m_nNumVertexTextureUnits;				/**<	The number of vertex texture units supported					*/
	bool			m_bVertexTextureUnitsShared;			/**<	Are vertex texture units shared with pixel processor?			*/
public:
	CPepeEngineRendererCapabilities();
	~CPepeEngineRendererCapabilities();
	
	void setNumWorldMatricies(unsigned short num)
	{
		m_nNumWorldMatrices = num;
	}

	void setNumTextureUnits(unsigned short num)
	{
		m_nNumTextureUnits = num;
	}

	void setStencilBufferBitDepth(unsigned short num)
	{
		m_nStencilBufferBitDepth = num;
	}

	void setNumVertexBlendMatrices(unsigned short num)
	{
		m_nNumVertexBlendMatrices = num;
	}

	/**
	 *	The number of simultaneous render targets supported
	 */
	void setNumMultiRenderTargets(unsigned short num)
	{
		m_nNumMultiRenderTargets = num;
	}

	unsigned short getNumWorldMatricies(void) const
	{ 
		return m_nNumWorldMatrices;
	}

	/** 
	 *	Returns the number of texture units the current output hardware
	 *	supports.
	 *
	 *	For use in rendering, this determines how many texture units the
 	 *	are available for multitexturing (i.e. rendering multiple  
	 *	textures in a single pass). Where a Material has multiple 
     *	texture layers, it will try to use multitexturing where 
	 *	available, and where it is not available, will perform multipass
	 *	rendering to achieve the same effect. This property only applies
	 *	to the fixed-function pipeline, the number available to the 
	 *	programmable pipeline depends on the shader model in use.
	 */
	unsigned short getNumTextureUnits(void) const
	{
		return m_nNumTextureUnits;
	}

	/** 
	 *	Determines the bit depth of the hardware accelerated stencil 
	 *	buffer, if supported.
	 *
	 *	If hardware stencilling is not supported, the software will
	 *	provide an 8-bit software stencil.
	 */
	unsigned short getStencilBufferBitDepth(void) const
	{
		return m_nStencilBufferBitDepth;
	}

	/** 
	 *	Returns the number of matrices available to hardware vertex 
	 *	blending for this rendering system. 
	 */
	unsigned short numVertexBlendMatrices(void) const
	{
		return m_nNumVertexBlendMatrices;
	}

	/**
	 *	The number of simultaneous render targets supported
	 */
	unsigned short numMultiRenderTargets(void) const
	{
		return m_nNumMultiRenderTargets;
	}

	/** 
	 *	Adds a capability flag to mCapabilities
	*/
	void setCapability(const Capabilities c) 
	{ 
		m_nCapabilities |= c;
	}

	/** 
	 *	Checks for a capability
	 */
	bool hasCapability(const Capabilities c) const
	{
		if (m_nCapabilities & c)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 *	Gets the best low-level vertex program version supported
	 */
	const tstring& getMaxVertexProgramVersion(void) const
	{
		return m_strMaxVertexProgramVersion;
	}

	/**
	 *	Gets the best pixel program that this card / rendersystem supports
	 */
	const tstring& getMaxPixelProgramVersion(void) const
	{
		return m_strMaxPixelProgramVersion;
	}

	/**
	 *	The number of floating-point constants vertex programs support
	 */
	unsigned short getVertexProgramConstantFloatCount(void) const
	{
		return m_nVertexProgramConstantFloatCount;           
	}

	/**
	 *	The number of integer constants vertex programs support
	 */
	unsigned short getVertexProgramConstantIntCount(void) const
	{
		return m_nVertexProgramConstantIntCount;           
	}

	/**
	 *	The number of boolean constants vertex programs support
	 */
	unsigned short getVertexProgramConstantBoolCount(void) const
	{
		return m_nVertexProgramConstantBoolCount;           
	}

	/**
	 *	The number of floating-point constants pixel programs support
	 */
	unsigned short getPixelProgramConstantFloatCount(void) const
	{
		return m_nPixelProgramConstantFloatCount;           
	}

	/**
	 *	The number of integer constants pixel programs support
	 */
	unsigned short getPixelProgramConstantIntCount(void) const
	{
		return m_nPixelProgramConstantIntCount;           
	}

	/**
	 *	The number of boolean constants pixel programs support
	 */
	unsigned short getPixelProgramConstantBoolCount(void) const
	{
		return m_nPixelProgramConstantBoolCount;           
	}

	/**
	 *	Sets the best low-level vertex program version supported
	 */
	void setMaxVertexProgramVersion(const tstring& ver)
	{
		m_strMaxVertexProgramVersion = ver;
	}

	/**
	 *	Sets the best pixel program that this card / rendersystem supports
	 */
	void setMaxPixelProgramVersion(const tstring& ver)
	{
		m_strMaxPixelProgramVersion = ver;
	}

	/**
	 *	The number of floating-point constants vertex programs support
	 */
	void setVertexProgramConstantFloatCount(unsigned short c)
	{
		m_nVertexProgramConstantFloatCount = c;           
	}

	/**
	 *	The number of integer constants vertex programs support
	 */
	void setVertexProgramConstantIntCount(unsigned short c)
	{
		m_nVertexProgramConstantIntCount = c;           
	}

	/**
	 *	The number of boolean constants vertex programs support
	 */
	void setVertexProgramConstantBoolCount(unsigned short c)
	{
		m_nVertexProgramConstantBoolCount = c;           
	}

	/**
	 *	The number of floating-point constants pixel programs support
	 */
	void setPixelProgramConstantFloatCount(unsigned short c)
	{
		m_nPixelProgramConstantFloatCount = c;           
	}

	/**
	 *	The number of integer constants pixel programs support
	 */
	void setPixelProgramConstantIntCount(unsigned short c)
	{
		m_nPixelProgramConstantIntCount = c;           
	}

	/**
	 *	The number of boolean constants pixel programs support
	 */
	void setPixelProgramConstantBoolCount(unsigned short c)
	{
		m_nPixelProgramConstantBoolCount = c;           
	}

	/**
	 *	Maximum point screen size in pixels
	 */
	void setMaxPointSize(float s)
	{
		m_fMaxPointSize = s;
	}

	/**
	 *	Maximum point screen size in pixels
	 */
	float getMaxPointSize(void) const
	{
		return m_fMaxPointSize;
	}

	/**
	 *	Non-POW2 textures limited
	 */
	void setNonPOW2TexturesLimited(bool l)
	{
		m_bNonPOW2TexturesLimited = l;
	}

	/** 
	 *	Are non-power of two textures limited in features?
	 *
	 *	If the RSC_NON_POWER_OF_2_TEXTURES capability is set, but this
	 *	method returns true, you can use non power of 2 textures only if:
	 *	<ul><li>You load them explicitly with no mip maps</li>
	 *	<li>You don't use DXT texture compression</li>
	 *	<li>You use clamp texture addressing</li></ul>
	 */
	bool getNonPOW2TexturesLimited(void) const
	{
		return m_bNonPOW2TexturesLimited;
	}

	/**
	 *	Set the number of vertex texture units supported
	 */
	void setNumVertexTextureUnits(unsigned short n)
	{
		m_nNumVertexTextureUnits = n;
	}

	/**
	 *	Get the number of vertex texture units supported
	 */
	unsigned short getNumVertexTextureUnits(void) const
	{
		return m_nNumVertexTextureUnits;
	}

	/**
	 *	Set whether the vertex texture units are shared with the pixel processor
	 */
	void setVertexTextureUnitsShared(bool shared)
	{
		m_bVertexTextureUnitsShared = shared;
	}

	/**
	 *	Get whether the vertex texture units are shared with the pixel processor
	 */
	bool getVertexTextureUnitsShared(void) const
	{
		return m_bVertexTextureUnitsShared;
	}

	void log();
};

_PEPE_ENGINE_END

#endif