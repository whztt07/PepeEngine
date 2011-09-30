/**
 * Project:		PepeEngine
 * Tier:		Middleware
 * File:		IPepeEngineTexture.h 
 *
 * @brief		Declaration of IPepeEngineTexture interface.  
 *			
 * @author		Piotr 'pepe' Picheta
 * @date		2008-04-20
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef IPEPEENGINETEXTURE_H
#define IPEPEENGINETEXTURE_H

#include "PepeEngineInit.h"

#include "CPepeEngineImage.h"
#include "IPepeEngineBuffer.h"
#include "IPepeEngineResource.h"

_PEPE_ENGINE_START


/** 
 *	Enum identifying the texture type
 */
enum TextureType
{

	TEX_TYPE_1D			= 1,	/**<	1D texture, used in combination with 1D texture coordinates				*/        
	TEX_TYPE_2D			= 2,	/**<	2D texture, used in combination with 2D texture coordinates (default)	*/        
	TEX_TYPE_3D			= 3,	/**<	3D volume texture, used in combination with 3D texture coordinates		*/        
	TEX_TYPE_CUBE_MAP	= 4		/**<	3D cube map, used in combination with 3D texture coordinates			*/
};

/** 
 *	Enum identifying special mipmap numbers
 */
enum TextureMipmap
{		
	MIP_UNLIMITED	= 0x7FFFFFFF,	/**<	Generate mipmaps up to 1x1					*/		
	MIP_DEFAULT		= -1			/**<	Use CPepeEngineTextureManager default		*/
};

enum TextureUsage
{		
	TU_STATIC							= IPepeEngineBuffer::BU_STATIC,
	TU_DYNAMIC							= IPepeEngineBuffer::BU_DYNAMIC,
	TU_WRITE_ONLY						= IPepeEngineBuffer::BU_WRITE_ONLY,
	TU_STATIC_WRITE_ONLY				= IPepeEngineBuffer::BU_STATIC_WRITE_ONLY, 
	TU_DYNAMIC_WRITE_ONLY				= IPepeEngineBuffer::BU_DYNAMIC_WRITE_ONLY,
	TU_DYNAMIC_WRITE_ONLY_DISCARDABLE	= IPepeEngineBuffer::BU_DYNAMIC_WRITE_ONLY_DISCARDABLE,

	/** 
	 *	Mipmaps will be automatically generated for this texture 
	 */		
	TU_AUTOMIPMAP						= 0x100,

	/** 
	 *	This texture will be a render target, ie. used as a target for render to texture
	 *	setting this flag will ignore all other texture usages except TU_AUTOMIPMAP
	 */
	TU_RENDERTARGET						= 0x200,

	/** 
	 *	Default to automatic mipmap generation static textures 
	 */
	TU_DEFAULT							= TU_AUTOMIPMAP | TU_STATIC_WRITE_ONLY        
};

class _PepeEngineExport IPepeEngineTexture : public IPepeEngineResource
{
public:	
						IPepeEngineTexture(const tstring &strName) 
							:	IPepeEngineResource(strName), m_type(TEX_TYPE_2D), m_usage(TU_DEFAULT), 
								m_nSrcWidth(0), m_nSrcHeight(0), m_format(PF_UNKNOWN), m_nMipMaps(MIP_DEFAULT),
								m_nMipMapsRequested(MIP_DEFAULT), m_bMipMapsHardwareGenerated(false)
						{}

	virtual				~IPepeEngineTexture() {}
	
	void				setTextureType(const TextureType& type) {m_type = type;}
	const TextureType&	getTextureType() const {return m_type;}

	void				setTextureUsage(const TextureUsage& usage) {m_usage = usage;}
	const TextureUsage& getTextureUsage() const {return m_usage;}

	const PixelFormat&	getPixelFormat() const {return m_format;}

	inline void			setMipMapsNum(size_t nMipMaps);
	inline size_t		getMipMapsNum() const;
	inline bool			isMipMapsHardwareGenerated() const;

protected:
	TextureType			m_type;	
	TextureUsage		m_usage;	
	PixelFormat			m_format;

	size_t				m_nMipMapsRequested;
	size_t				m_nMipMaps;
	bool				m_bMipMapsHardwareGenerated;

	size_t				m_nSrcWidth;
	size_t				m_nSrcHeight;	

	static tstring		CubeMapNames[6];			
};

struct _PepeEngineExport TexturePtr : public CPepeEngineSharedPtr<IPepeEngineTexture>
{
public:
	inline				TexturePtr();
	inline explicit		TexturePtr(IPepeEngineTexture* r);
	inline				TexturePtr(const TexturePtr& r);
	inline				TexturePtr(const ResourcePtr& r); 
	inline TexturePtr&	operator=(const ResourcePtr& r);
};

_PEPE_ENGINE_END

#endif