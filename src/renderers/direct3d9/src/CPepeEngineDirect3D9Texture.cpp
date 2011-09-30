#include "CPepeEngineConverter.h"
#include "CPepeEngineDirect3D9Mapper.h"
#include "CPepeEngineDirect3D9Texture.h"
#include "CPepeEngineException.h"
#include "CPepeEngineImageManager.h"
#include "CPepeEngineLogManager.h"
#include "IPepeEngineTextureManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Texture::createInternalResources()
{
	unload();
	switch (getTextureType())
	{
	case TEX_TYPE_1D:
	case TEX_TYPE_2D:
		createNormTexture();
		break;
	case TEX_TYPE_3D:
		createVolumeTexture();
		break;
	case TEX_TYPE_CUBE_MAP:
		createCubeMapTexture();
		break;
	default:		
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_INVALID_PARAMETERS, 
			_T("Unknown texture format"), 
			_T("CPepeEngineDirect3D9Texture::createInternalResources")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Texture::createNormTexture()
{
	assert(m_type == TEX_TYPE_1D || m_type == TEX_TYPE_2D);
	assert(m_nSrcWidth > 0 && m_nSrcHeight > 0);
	
	HRESULT hr;	

	if (useDefaultPool())
	{
		m_D3DPool = D3DPOOL_DEFAULT;
	} else
	{
		m_D3DPool = D3DPOOL_MANAGED;
	}

	// determine wich D3D9 pixel format we'll use	
	D3DFORMAT d3dPF = this->chooseD3DFormat();
	DWORD usage		= (m_usage & TU_RENDERTARGET) ? D3DUSAGE_RENDERTARGET : 0;
	hr				= D3DXCheckTextureRequirements(m_pDevice, NULL, NULL, NULL, 0, &d3dPF, m_D3DPool);

	// Check dynamic textures
	if (m_usage & TU_DYNAMIC)
	{		
		if (canUseDynamicTextures(usage, D3DRTYPE_TEXTURE, d3dPF))
		{
			usage |= D3DUSAGE_DYNAMIC;
		}
	}

	unsigned int numMips = (m_nMipMapsRequested == MIP_UNLIMITED) ? D3DX_DEFAULT : m_nMipMapsRequested + 1;

	// check if mip maps are supported on hardware
	m_bMipMapsHardwareGenerated = false;
	if (m_caps.TextureCaps & D3DPTEXTURECAPS_MIPMAP)
	{
		if (m_usage & TU_AUTOMIPMAP && m_nMipMapsRequested != 0)
		{
			// use auto.gen. if available, and if desired
			m_bMipMapsHardwareGenerated = this->canAutoGenMipmaps(usage, D3DRTYPE_TEXTURE, d3dPF);
			if (m_bMipMapsHardwareGenerated)
			{
				usage	|= D3DUSAGE_AUTOGENMIPMAP;
				numMips = 0;
			}
		}
	} else
	{
		// no mip map support for this kind of textures :(
		m_nMipMaps	= 0;
		numMips		= 1;
	}

	hr = D3DXCreateTexture(
		m_pDevice,
		m_nSrcWidth,
		m_nSrcHeight,
		numMips,
		usage,
		d3dPF,
		m_D3DPool,
		&m_pNormTexture
	);

	if (FAILED(hr))
	{
		tstring msg = DXGetErrorDescription9(hr);					

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Create normal texture error: ") + msg, 
			_T("CPepeEngineDirect3D9Texture::createNormTexture")
		);
	} 

	tstring strFilePath	= IPepeEngineTextureManager::getSingleton().getFilePath() + m_strName;
	ImagePtr pImage		= CPepeEngineImageManager::getSingleton().getByName(strFilePath);

	if (pImage)
	{		
		pImage->convert(CPepeEngineDirect3D9Mapper::get(d3dPF));

		LPDIRECT3DSURFACE9 d3dsurf;
		hr = m_pNormTexture->GetSurfaceLevel(0, &d3dsurf);

		if (FAILED(hr))
		{
			tstring msg = DXGetErrorDescription9(hr);					

			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
				_T("Can't get texture surface level: ") + msg, 
				_T("CPepeEngineDirect3D9Texture::createNormTexture")
			);
		} 

		RECT srcRect;

		srcRect.left	= 0;
		srcRect.right	= m_nSrcWidth;
		srcRect.top		= 0;
		srcRect.bottom	= m_nSrcHeight;

		hr = D3DXLoadSurfaceFromMemory(
			d3dsurf, 
			NULL, 
			NULL, 
			pImage->ptr(), 
			d3dPF,
			m_nSrcWidth * pImage->getBPP() / 8,
			NULL, 
			&srcRect, 
			getBestFilterMethod(), 
			0
		);

		if (FAILED(hr))
		{
			tstring msg = DXGetErrorDescription9(hr);					

			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
				_T("Can't load surface from memory: ") + msg, 
				_T("CPepeEngineDirect3D9Texture::createNormTexture")
			);
		} 
	
		m_bIsLoaded = true;
		pImage->unload();
	}		

	// Set best filter type
	if (m_bMipMapsHardwareGenerated)
	{
		hr = m_pNormTexture->SetAutoGenFilterType(getBestFilterMethod());
		if (FAILED(hr))
		{			
			tstring msg = DXGetErrorDescription9(hr);			

			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
				_T("Could not set best autogen filter type: ") + msg, 
				_T("CPepeEngineDirect3D9Texture::createNormTexture")
			);		
		}
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Texture::createVolumeTexture()
{
	assert(m_type == TEX_TYPE_3D);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Texture::createCubeMapTexture()
{
	assert(m_type == TEX_TYPE_CUBE_MAP);
	assert(m_nSrcWidth > 0 && m_nSrcHeight > 0);
	
	HRESULT hr;	

	if (useDefaultPool())
	{
		m_D3DPool = D3DPOOL_DEFAULT;
	} else
	{
		m_D3DPool = D3DPOOL_MANAGED;
	}

	// determine wich D3D9 pixel format we'll use	
	D3DFORMAT d3dPF = this->chooseD3DFormat();
	DWORD usage		= (m_usage & TU_RENDERTARGET) ? D3DUSAGE_RENDERTARGET : 0;
	hr				= D3DXCheckTextureRequirements(m_pDevice, NULL, NULL, NULL, 0, &d3dPF, m_D3DPool);

	// Check dynamic textures
	if (m_usage & TU_DYNAMIC)
	{		
		if (canUseDynamicTextures(usage, D3DRTYPE_TEXTURE, d3dPF))
		{
			usage |= D3DUSAGE_DYNAMIC;
		}
	}

	unsigned int numMips = (m_nMipMapsRequested == MIP_UNLIMITED) ? D3DX_DEFAULT : m_nMipMapsRequested + 1;

	// check if mip maps are supported on hardware
	m_bMipMapsHardwareGenerated = false;
	if (m_caps.TextureCaps & D3DPTEXTURECAPS_MIPMAP)
	{
		if (m_usage & TU_AUTOMIPMAP && m_nMipMapsRequested != 0)
		{
			// use auto.gen. if available, and if desired
			m_bMipMapsHardwareGenerated = this->canAutoGenMipmaps(usage, D3DRTYPE_CUBETEXTURE, d3dPF);
			if (m_bMipMapsHardwareGenerated)
			{
				usage	|= D3DUSAGE_AUTOGENMIPMAP;
				numMips = 0;
			}
		}
	} else
	{
		// no mip map support for this kind of textures :(
		m_nMipMaps	= 0;
		numMips		= 1;
	}

	hr = D3DXCreateCubeTexture(
		m_pDevice,
		m_nSrcWidth,
		numMips,		
		usage,
		d3dPF,
		m_D3DPool,
		&m_pCubeTexture
	);

	if (FAILED(hr))
	{
		tstring msg = DXGetErrorDescription9(hr);					

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Create cube texture error: ") + msg, 
			_T("CPepeEngineDirect3D9Texture::createCubeMapTexture")
		);
	} 

	for (int i = 0; i < 6; i++)
	{	
	
		tstring fileName	= m_strName;		
		tstring	strFilePath	= IPepeEngineTextureManager::getSingleton().getFilePath() + fileName.insert(m_strName.rfind(_T(".")), CubeMapNames[i]);
		ImagePtr pImage		= CPepeEngineImageManager::getSingleton().getByName(strFilePath);

		if (pImage)
		{
			pImage->convert(CPepeEngineDirect3D9Mapper::get(d3dPF));

			LPDIRECT3DSURFACE9 d3dsurf;
			hr = m_pCubeTexture->GetCubeMapSurface(
				CPepeEngineDirect3D9Mapper::get(CubeMapNames[i]), 
				0,
				&d3dsurf
			);

			if (FAILED(hr))
			{
				tstring msg = DXGetErrorDescription9(hr);					

				PEPE_ENGINE_EXCEPT(
					CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
					_T("Can't get texture surface level: ") + msg, 
					_T("CPepeEngineDirect3D9Texture::createNormTexture")
				);
			} 

			RECT srcRect;

			srcRect.left	= 0;
			srcRect.right	= m_nSrcWidth;
			srcRect.top		= 0;
			srcRect.bottom	= m_nSrcHeight;

			hr = D3DXLoadSurfaceFromMemory(
				d3dsurf, 
				NULL, 
				NULL, 
				pImage->ptr(), 
				d3dPF,
				m_nSrcWidth * pImage->getBPP() / 8,
				NULL, 
				&srcRect, 
				getBestFilterMethod(), 
				0
			);

			if (FAILED(hr))
			{
				tstring msg = DXGetErrorDescription9(hr);					

				PEPE_ENGINE_EXCEPT(
					CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
					_T("Can't load surface from memory: ") + msg, 
					_T("CPepeEngineDirect3D9Texture::createNormTexture")
				);
			}
			m_bIsLoaded = true;
			pImage->unload();
		} else
		{
			m_bIsLoaded = false;
		}		
	}	

	// Set best filter type
	if (m_bMipMapsHardwareGenerated)
	{
		hr = m_pCubeTexture->SetAutoGenFilterType(getBestFilterMethod());
		if (FAILED(hr))
		{			
			tstring msg = DXGetErrorDescription9(hr);			

			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
				_T("Could not set best autogen filter type: ") + msg, 
				_T("CPepeEngineDirect3D9Texture::createNormTexture")
			);		
		}
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Texture::loadNormTexture()
{
	assert(m_type == TEX_TYPE_1D || m_type == TEX_TYPE_2D);

	tstring		strFilePath = IPepeEngineTextureManager::getSingleton().getFilePath() + m_strName;		
	ImagePtr	pImage		= CPepeEngineImageManager::getSingleton().create(strFilePath);	
	pImage->load();

	m_nSrcWidth		= pImage->getWidth();
	m_nSrcHeight	= pImage->getHeight();
	m_format		= pImage->getFormat();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Texture::loadVolumeTexture()
{
	assert(m_type == TEX_TYPE_3D);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Texture::loadCubeMapTexture()
{
	assert(m_type == TEX_TYPE_CUBE_MAP);

	ImagePtr pImage = NULL;
	for (int i = 0; i < 6; i++)
	{
		tstring fileName	= m_strName;
		tstring	strFilePath = IPepeEngineTextureManager::getSingleton().getFilePath() + fileName.insert(m_strName.rfind(_T(".")), CubeMapNames[i]);		
		pImage				= CPepeEngineImageManager::getSingleton().create(strFilePath);		
		pImage->load();
	}

	m_nSrcWidth		= pImage->getWidth();
	m_nSrcHeight	= pImage->getHeight();
	m_format		= pImage->getFormat();
}

// -----------------------------------------------------------------------------------------
D3DTEXTUREFILTERTYPE CPepeEngineDirect3D9Texture::getBestFilterMethod()
{	
	assert(m_pDevice);
	assert(m_pD3D);	

	DWORD filterCaps = 0;
	
	switch (this->getTextureType())
	{
	case TEX_TYPE_1D:		// Same as 2D
	case TEX_TYPE_2D:		filterCaps = m_caps.TextureFilterCaps;			break;
	case TEX_TYPE_3D:		filterCaps = m_caps.VolumeTextureFilterCaps;	break;
	case TEX_TYPE_CUBE_MAP:	filterCaps = m_caps.CubeTextureFilterCaps;		break;
	}

	if (filterCaps & D3DPTFILTERCAPS_MINFGAUSSIANQUAD)
	{
		return D3DTEXF_GAUSSIANQUAD;
	}

	if (filterCaps & D3DPTFILTERCAPS_MINFPYRAMIDALQUAD)
	{
		return D3DTEXF_PYRAMIDALQUAD;
	}

	if (filterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC)
	{
		return D3DTEXF_ANISOTROPIC;
	}

	if (filterCaps & D3DPTFILTERCAPS_MINFLINEAR)
	{
		return D3DTEXF_LINEAR;
	}

	if (filterCaps & D3DPTFILTERCAPS_MINFPOINT)
	{
		return D3DTEXF_POINT;
	}

	return D3DTEXF_POINT;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineDirect3D9Texture::canUseDynamicTextures(DWORD srcUsage, D3DRESOURCETYPE srcType, D3DFORMAT srcFormat)
{	
	assert(m_pDevice);
	assert(m_pD3D);
	
	HRESULT hr;

	hr = m_pD3D->CheckDeviceFormat(
		m_devCreationParams.AdapterOrdinal, 
		m_devCreationParams.DeviceType, 
		m_BBPixelFormat, 
		srcUsage | D3DUSAGE_DYNAMIC,
		srcType,
		srcFormat);
	if (hr == D3D_OK)
	{
		return true;
	} else
	{
		return false;
	}
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineDirect3D9Texture::canAutoGenMipmaps(
	unsigned int srcUsage, D3DRESOURCETYPE srcType, D3DFORMAT srcFormat
)
{	
	assert(m_pDevice);
	assert(m_pD3D);

	// || (mDepth & mDepth-1)
	if ((m_nSrcWidth & m_nSrcWidth - 1) || (m_nSrcHeight & m_nSrcHeight - 1))
	{
		return false;
	}

	if (m_caps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP)
	{
		HRESULT hr;
		// check for auto gen. mip maps support
		hr = m_pD3D->CheckDeviceFormat(
			m_devCreationParams.AdapterOrdinal, 
			m_devCreationParams.DeviceType, 
			m_BBPixelFormat, 
			srcUsage | D3DUSAGE_AUTOGENMIPMAP,
			srcType,
			m_BBPixelFormat
		);
		
		if (hr == D3D_OK)
		{
			return true;
		} else
		{
			return false;
		}
	} else
	{
		return false;
	}
}

// -----------------------------------------------------------------------------------------
D3DFORMAT CPepeEngineDirect3D9Texture::chooseD3DFormat()
{
	// Choose frame buffer pixel format in case PF_UNKNOWN was requested
	if (m_format == PF_UNKNOWN)
	{
		return m_BBPixelFormat;
	}
	// Choose closest supported D3D format as a D3D format
	return CPepeEngineDirect3D9Mapper::get(CPepeEngineDirect3D9Mapper::getClosestSupportedPF(m_format));
}

// -----------------------------------------------------------------------------------------
CPepeEngineDirect3D9Texture::CPepeEngineDirect3D9Texture(LPDIRECT3DDEVICE9 pDevice, const tstring &strName) 
	: IPepeEngineTexture(strName)
{
	m_pDevice			= pDevice;
	m_pNormTexture		= NULL;	
	m_pCubeTexture		= NULL;
	m_pVolumeTexture	= NULL;
	if (useDefaultPool())
	{
		m_D3DPool = D3DPOOL_DEFAULT;
	} else
	{
		m_D3DPool = D3DPOOL_MANAGED;
	}
	m_format = PF_UNKNOWN;

	assert(m_pDevice);
	HRESULT hr;

	// get device caps
	hr = m_pDevice->GetDeviceCaps(&m_caps);
	if (FAILED(hr))
	{
		tstring msg = DXGetErrorDescription9(hr);					

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_INTERNAL_ERROR, 
			_T("Can't get device description:  ") + msg, 
			_T("CPepeEngineDirect3D9Texture::CPepeEngineDirect3D9Texture")
		);
	}

	// get D3D pointer
	hr = m_pDevice->GetDirect3D(&m_pD3D);
	// decrement reference count
	m_pD3D->Release();
	if (FAILED(hr))
	{
		tstring msg = DXGetErrorDescription9(hr);					

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Failed to get D3D9 pointer:  ") + msg, 
			_T("CPepeEngineDirect3D9Texture::CPepeEngineDirect3D9Texture")
		);	
	}

	// get our device creation parameters
	hr = m_pDevice->GetCreationParameters(&m_devCreationParams);
	if (FAILED(hr))
	{
		tstring msg = DXGetErrorDescription9(hr);					

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Failed to get D3D9 device creation parameters:   ") + msg, 
			_T("CPepeEngineDirect3D9Texture::CPepeEngineDirect3D9Texture")
		);		
	}

	// get our back buffer pixel format
	IDirect3DSurface9	*pSrf;
	D3DSURFACE_DESC		srfDesc;
	hr = m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSrf);
	// decrement reference count
	pSrf->Release();
	if (FAILED(hr))
	{
		tstring msg = DXGetErrorDescription9(hr);					

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Failed to get D3D9 device pixel format: ") + msg, 
			_T("CPepeEngineDirect3D9Texture::CPepeEngineDirect3D9Texture")
		);			
	}

	hr = pSrf->GetDesc(&srfDesc);
	if (FAILED(hr))
	{
		tstring msg = DXGetErrorDescription9(hr);					

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Failed to get D3D9 device pixel format:  ") + msg, 
			_T("CPepeEngineDirect3D9Texture::CPepeEngineDirect3D9Texture")
		);			
	}

	m_BBPixelFormat = srfDesc.Format;
}

// -----------------------------------------------------------------------------------------
CPepeEngineDirect3D9Texture::~CPepeEngineDirect3D9Texture()
{
	if (isLoaded())
	{
		unload();
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Texture::load()
{	
	if (!isLoaded())
	{	
		LOG("Loading texture: " + getName());
		switch (getTextureType())
		{
		case TEX_TYPE_1D:
		case TEX_TYPE_2D:
			loadNormTexture();
			break;
		case TEX_TYPE_3D:
			loadVolumeTexture();
			break;
		case TEX_TYPE_CUBE_MAP:
			loadCubeMapTexture();
			break;
		default:		
			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_INVALID_PARAMETERS, 
				_T("Unknown texture format"), 
				_T("CPepeEngineDirect3D9Texture::load")
			);
		}		
		createInternalResources();
		LOG("Texture successfully loaded");
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Texture::unload()
{
	LOG("Unloading texture: " + getName());
	SAFE_RELEASE(m_pNormTexture);
	SAFE_RELEASE(m_pVolumeTexture);
	SAFE_RELEASE(m_pCubeTexture);
	m_bIsLoaded = false;	
	LOG("Texture successfully unloaded");
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineDirect3D9Texture::useDefaultPool() const
{
	return (m_usage & TU_RENDERTARGET) || (m_usage & TU_DYNAMIC);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Texture::releaseIfDefaultPool()
{
	if (m_D3DPool == D3DPOOL_DEFAULT)
	{
		if (isLoaded())
		{
			unload();
		}
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9Texture::recreateIfDefaultPool()
{
	
	if (m_D3DPool == D3DPOOL_DEFAULT)
	{
		if (!isLoaded())
		{
			load();
		}
	}
}

// -----------------------------------------------------------------------------------------
Direct3D9TexturePtr::Direct3D9TexturePtr() : CPepeEngineSharedPtr<CPepeEngineDirect3D9Texture>()
{
}

// -----------------------------------------------------------------------------------------
Direct3D9TexturePtr::Direct3D9TexturePtr(CPepeEngineDirect3D9Texture* r) 
	: CPepeEngineSharedPtr<CPepeEngineDirect3D9Texture>(r)
{
}

// -----------------------------------------------------------------------------------------
Direct3D9TexturePtr::Direct3D9TexturePtr(const Direct3D9TexturePtr& r) 
	: CPepeEngineSharedPtr<CPepeEngineDirect3D9Texture>(r)
{
}

// -----------------------------------------------------------------------------------------
Direct3D9TexturePtr::Direct3D9TexturePtr(const ResourcePtr& r) 
	: CPepeEngineSharedPtr<CPepeEngineDirect3D9Texture>()
{
	ptr = (CPepeEngineDirect3D9Texture*)r.data();
	if (ptr) 
	{
		ptr->ref();
	}
}

// -----------------------------------------------------------------------------------------
Direct3D9TexturePtr& Direct3D9TexturePtr::operator=(const ResourcePtr& r)
{
	if (ptr == (CPepeEngineDirect3D9Texture*)r.data()) 
	{
		return *this;
	}
	if (ptr)
	{
		ptr->deref();
	}
	ptr = (CPepeEngineDirect3D9Texture*)r.data();
	if (ptr) 
	{
		ptr->ref();
	}
	return *this;
}

_PEPE_ENGINE_END