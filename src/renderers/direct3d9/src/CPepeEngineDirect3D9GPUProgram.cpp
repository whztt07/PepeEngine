#include "CPepeEngineConverter.h"
#include "CPepeEngineCore.h"
#include "CPepeEngineDirect3D9GPUProgram.h"
#include "CPepeEngineException.h"
#include "CPepeEngineLogManager.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9GPUProgram::setVariable(	
	const tstring& strName,
	const float* pfValues,
	unsigned int nCount
)
{
	if (!isLoaded() || m_pConstantTable == NULL)
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_INVALID_PARAMETERS, 
			_T("Couldn't set variable for nonexisting program"), 
			_T("CPepeEngineDirect3D9GPUProgram::setVariable")
		);
	}
	
	D3DXHANDLE handle;

	char *strVName = NULL;

	CPepeEngineConverter::getChars(strName, &strVName);

	if (handle = m_pConstantTable->GetConstantByName(NULL, strVName))
	{
		HRESULT	hr;		

		delete strVName;

		if (FAILED(hr = m_pConstantTable->SetFloatArray(m_pDevice, handle, pfValues, nCount)))
		{
			tstring msg = DXGetErrorDescription9(hr);
				
			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
				_T("Couldn't set variable: ") + strName + _T(" - ") + msg, 
				_T("CPepeEngineDirect3D9GPUProgram::setVariable")
			);
		}
	} else
	{
		delete strVName;

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_INVALID_PARAMETERS, 
			_T("Variable ") + strName + _T(" doesn't exist"), 
			_T("CPepeEngineDirect3D9GPUProgram::setVariable")
		);
	}

}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9GPUProgram::setTexture(		
	const tstring& strName,
	TexturePtr pTexture
)
{
	if (!isLoaded() || m_pConstantTable == NULL)	
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_INVALID_PARAMETERS, 
			_T("Couldn't set variable for nonexisting program"), 
			_T("CPepeEngineDirect3D9GPUProgram::setTexture")
		);
	}

	D3DXHANDLE				handle;

	char *strVName = NULL;

	CPepeEngineConverter::getChars(strName, &strVName);

	if (handle = m_pConstantTable->GetConstantByName(NULL, strVName))
	{
		delete strVName;
		
		D3DXCONSTANT_DESC	constDesc;
		unsigned int		count;

		m_pConstantTable->GetConstantDesc(handle, &constDesc, &count);
		
		if (constDesc.RegisterSet == D3DXRS_SAMPLER)
		{
			CPepeEngineCore::getSingleton().getRenderer()->setTexture(pTexture, constDesc.RegisterIndex);
		} else
		{				
			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_INVALID_PARAMETERS, 
				_T("Variable ") + strName + _T(" is not a sampler."), 
				_T("CPepeEngineDirect3D9GPUProgram::setTexture")
			);
		}
		
	} else
	{
		delete strVName;

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_INVALID_PARAMETERS, 
			_T("Sampler ") + strName + _T(" doesn't exist"), 
			_T("CPepeEngineDirect3D9GPUProgram::setTexture")
		);
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9GPUProgram::buildConstantDefinitions()
{		
	assert(m_pConstantTable && "Program not loaded!");
	
	D3DXCONSTANTTABLE_DESC	desc;
	HRESULT					hr = m_pConstantTable->GetDesc(&desc);

	m_floatLogicalToPhysical.m_nBufferSize	= 0;
	m_intLogicalToPhysical.m_nBufferSize	= 0;
	m_namedConstants.m_nFloatBufferSize		= 0;
	m_namedConstants.m_nIntBufferSize		= 0;

	if (FAILED(hr))
	{
		tstring msg = DXGetErrorDescription9(hr);

		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Cannot retrieve constant descriptions from HLSL program: ") + msg, 
			_T("CPepeEngineDirect3D9GPUProgram::buildConstantDefinitions")
		);		
	}

	for (unsigned int i = 0; i < desc.Constants; ++i)
	{
		processParamElement(NULL, _T(""), i);
	}

	
	m_parameters.setNamedConstants(&m_namedConstants);
	m_parameters.setLogicalIndexes(&m_floatLogicalToPhysical, &m_intLogicalToPhysical);
	m_parameters.setSamplers(&m_samplerList);

	m_parameters.generateAutoConstantList();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9GPUProgram::processParamElement(
	D3DXHANDLE parent, 
	tstring prefix, 
    unsigned int nIndex) const
{
	D3DXHANDLE hConstant = m_pConstantTable->GetConstant(parent, nIndex);

	/*
		Since D3D HLSL doesn't deal with naming of array and struct parameters
		automatically, we have to do it by hand
	*/

	D3DXCONSTANT_DESC	desc;
	unsigned int		numParams	= 1;
	HRESULT				hr			= m_pConstantTable->GetConstantDesc(hConstant, &desc, &numParams);
	if (FAILED(hr))
	{
		tstring msg = DXGetErrorDescription9(hr);
		
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
			_T("Cannot retrieve constant descriptions from HLSL program: ") + msg, 
			_T("CPepeEngineDirect3D9GPUProgram::processParamElement")
		);				
	}

	tstring paramName = _TSTR(desc.Name);
	
	// trim the odd '$' which appears at the start of the names in HLSL
	if (paramName.at(0) == '$')
	{
		paramName.erase(paramName.begin());
	}

	// Also trim the '[0]' suffix if it exists, we will add our own indexing later
	if (_TSTR(paramName.at(paramName.length() - 1)) == _T("[0]"))
	{
		paramName.erase(paramName.size() - 3);
	}


	if (desc.Class == D3DXPC_STRUCT)
	{
		// work out a new prefix for nested members, if it's an array, we need an index
		prefix = prefix + paramName + _T(".");
		// Cascade into struct
		for (unsigned int i = 0; i < desc.StructMembers; ++i)
		{
			processParamElement(hConstant, prefix, i);
		}
	}
	else
	{
		// Process params
		if (desc.Type == D3DXPT_FLOAT || desc.Type == D3DXPT_INT || desc.Type == D3DXPT_BOOL)
		{
			size_t	paramIndex	= desc.RegisterIndex;
			tstring name		= prefix + paramName;

			GpuConstantDefinition def;
			// populate type, array size & element size
			populateDef(desc, def);
			if (def.isFloat())
			{
				def.m_nPhysicalIndex = m_floatLogicalToPhysical.m_nBufferSize;
				
				m_floatLogicalToPhysical.map.insert(
					GpuLogicalIndexUseMap::value_type(paramIndex, 
					GpuLogicalIndexUse(def.m_nPhysicalIndex, def.m_nArraySize * def.m_nElementSize))
				);
				m_floatLogicalToPhysical.m_nBufferSize	+= def.m_nArraySize * def.m_nElementSize;
				m_namedConstants.m_nFloatBufferSize		= m_floatLogicalToPhysical.m_nBufferSize;
			}
			else
			{
				def.m_nPhysicalIndex = m_intLogicalToPhysical.m_nBufferSize;				
				m_intLogicalToPhysical.map.insert(
					GpuLogicalIndexUseMap::value_type(paramIndex, 
					GpuLogicalIndexUse(def.m_nPhysicalIndex, def.m_nArraySize * def.m_nElementSize))
				);
				m_intLogicalToPhysical.m_nBufferSize	+= def.m_nArraySize * def.m_nElementSize;
				m_namedConstants.m_nIntBufferSize		= m_intLogicalToPhysical.m_nBufferSize;
			}

			m_namedConstants.map.insert(GpuConstantDefinitionMap::value_type(name, def));

			// Now deal with arrays
			m_namedConstants.generateConstantDefinitionArrayEntries(name, def);
		} else if (desc.RegisterSet  == D3DXRS_SAMPLER)
		{
			tstring					name = prefix + paramName;
			GpuSamplerDefinition	def;

			switch(desc.Type)
			{
			case D3DXPT_SAMPLER:
			case D3DXPT_SAMPLER2D:
				def.m_constType = GCT_SAMPLER2D;
				break;
			case D3DXPT_SAMPLER1D:
				def.m_constType = GCT_SAMPLER1D;
				break;
			case D3DXPT_SAMPLER3D:
				def.m_constType = GCT_SAMPLER3D;
				break;
			case D3DXPT_SAMPLERCUBE:
				def.m_constType = GCT_SAMPLERCUBE;
				break;
			}		
			def.m_nLogicalIndex = desc.RegisterIndex;

			m_samplerList.insert(GpuSamplerDefinitionMap::value_type(name, def));
		}
	}

}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9GPUProgram::populateDef(D3DXCONSTANT_DESC& d3dDesc, GpuConstantDefinition& def) const
{
	def.m_nArraySize = d3dDesc.Elements;
	switch(d3dDesc.Type)
	{
	case D3DXPT_INT:
		switch(d3dDesc.Columns)
		{
		case 1:
			def.m_constType		= GCT_INT1;
			def.m_nElementSize	= 4; // HLSL always packs
			break;
		case 2:
			def.m_constType		= GCT_INT2;
			def.m_nElementSize	= 4; // HLSL always packs
			break;
		case 3:
			def.m_constType		= GCT_INT3;
			def.m_nElementSize	= 4; // HLSL always packs
			break;
		case 4:
			def.m_constType		= GCT_INT4;
			def.m_nElementSize	= 4; 
			break;
		} // columns
		break;
	case D3DXPT_FLOAT:
		switch(d3dDesc.Rows)
		{
		case 1:
			switch(d3dDesc.Columns)
			{
			case 1:
				def.m_constType		= GCT_FLOAT1;
				def.m_nElementSize	= 4; // HLSL always packs
				break;
			case 2:
				def.m_constType		= GCT_FLOAT2;
				def.m_nElementSize	= 4; // HLSL always packs
				break;
			case 3:
				def.m_constType		= GCT_FLOAT3;
				def.m_nElementSize	= 4; // HLSL always packs
				break;
			case 4:
				def.m_constType		= GCT_FLOAT4;
				def.m_nElementSize	= 4; 
				break;
			} // columns
			break;
		case 2:
			switch(d3dDesc.Columns)
			{
			case 2:
				def.m_constType		= GCT_MATRIX_2X2;
				def.m_nElementSize	= 8; // HLSL always packs
				break;
			case 3:
				def.m_constType		= GCT_MATRIX_2X3;
				def.m_nElementSize	= 8; // HLSL always packs
				break;
			case 4:
				def.m_constType		= GCT_MATRIX_2X4;
				def.m_nElementSize	= 8; 
				break;
			} // columns
			break;
		case 3:
			switch(d3dDesc.Columns)
			{
			case 2:
				def.m_constType		= GCT_MATRIX_3X2;
				def.m_nElementSize	= 12; // HLSL always packs
				break;
			case 3:
				def.m_constType		= GCT_MATRIX_3X3;
				def.m_nElementSize	= 12; // HLSL always packs
				break;
			case 4:
				def.m_constType		= GCT_MATRIX_3X4;
				def.m_nElementSize	= 12; 
				break;
			} // columns
			break;
		case 4:
			switch(d3dDesc.Columns)
			{
			case 2:
				def.m_constType		= GCT_MATRIX_4X2;
				def.m_nElementSize	= 16; // HLSL always packs
				break;
			case 3:
				def.m_constType		= GCT_MATRIX_4X3;
				def.m_nElementSize	= 16; // HLSL always packs
				break;
			case 4:
				def.m_constType		= GCT_MATRIX_4X4;
				def.m_nElementSize	= 16; 
				break;
			} // columns
			break;

		} // rows
		break;

	default:
		// not mapping samplers, don't need to take the space 
		break;
	};

}

// -----------------------------------------------------------------------------------------
CPepeEngineDirect3D9VertexShader::~CPepeEngineDirect3D9VertexShader()
{
	if (isLoaded())
	{
		unload();
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9VertexShader::load()
{
	LOG("Loading vertex program: " + getName());
	tstring strFilePath = IPepeEngineGPUProgramManager::getSingleton().getFilePath() + getName();
	
	create(strFilePath, m_strEntryPoint, m_strShaderModel);
	LOG("Vertex program successfully loaded");
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9VertexShader::unload()
{
	LOG("Unloading vertex program: " + getName());
	IPepeEngineGPUProgram::unload();

	SAFE_RELEASE(m_pVSShader);	
	LOG("Vertex program successfully unloaded");
}
	
// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9VertexShader::create(
	const tstring& strShaderProgram, 
	const tstring& strEntryPoint, 
	const tstring& strShaderVersion
)
{
	if (isLoaded()) return;
	
	HRESULT				hr;	
	LPD3DXBUFFER		pVSShaderBuffer;
	LPD3DXBUFFER		pVSShaderError;
	
	if (
		strShaderProgram.length() > 0 &&
		strEntryPoint.length() > 0 &&
		strShaderVersion.length() > 0
	)
	{
		char *strEntry	= NULL;
		char *strTarget	= NULL;

		CPepeEngineConverter::getChars(strEntryPoint, &strEntry);
		CPepeEngineConverter::getChars(strShaderVersion, &strTarget);

		if (FAILED(hr = D3DXCompileShaderFromFile(
			strShaderProgram.c_str(), NULL, NULL, strEntry, strTarget,
			NULL, &pVSShaderBuffer, &pVSShaderError, &m_pConstantTable)))
		{
			tstring msg = DXGetErrorDescription9(hr);
		
			delete[] strEntry;
			delete[] strTarget;

			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
				_T("Create vertex shader from file error: ") + msg, 
				_T("CPepeEngineDirect3D9VertexShader::create")
			);
		} else
		{
			if (FAILED(hr = m_pDevice->CreateVertexShader(
				(DWORD*)pVSShaderBuffer->GetBufferPointer(), &m_pVSShader)))
			{
				tstring msg = DXGetErrorDescription9(hr);				
				
				delete[] strEntry;
				delete[] strTarget;

				PEPE_ENGINE_EXCEPT(
					CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
					_T("Create vertex shader from file error: ") + msg, 
					_T("CPepeEngineDirect3D9VertexShader::create")
				);
				
			}

			m_bIsLoaded			= true;
			
			delete[] strEntry;
			delete[] strTarget;

			buildConstantDefinitions();

			SAFE_RELEASE(pVSShaderBuffer);
		}

	} else
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_INVALID_PARAMETERS, 
			_T("All parameters are required"), 
			_T("CPepeEngineDirect3D9VertexShader::create")
		);
	}
}

// -----------------------------------------------------------------------------------------
CPepeEngineDirect3D9PixelShader::~CPepeEngineDirect3D9PixelShader()
{
	if (isLoaded())
	{
		unload();
	}
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9PixelShader::load()
{
	LOG("Loading pixel program: " + getName());
	tstring strFilePath = IPepeEngineGPUProgramManager::getSingleton().getFilePath() + getName();
	
	create(strFilePath, m_strEntryPoint, m_strShaderModel);
	LOG("Pixel program successfully loaded");
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9PixelShader::unload()
{
	LOG("Unloading pixel program: " + getName());
	IPepeEngineGPUProgram::unload();

	SAFE_RELEASE(m_pPSShader);
	LOG("Pixel program successfully unloaded");
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9PixelShader::create(
	const tstring& strShaderProgram, 
	const tstring& strEntryPoint, 
	const tstring& strShaderVersion
)
{
	if (isLoaded()) return;	
	HRESULT				hr;	
	LPD3DXBUFFER		pPSShaderBuffer = NULL;

	if (
		strShaderProgram.length() > 0 &&
		strEntryPoint.length() > 0 &&
		strShaderVersion.length() > 0
	)
	{

		char *strEntry	= NULL;
		char *strTarget	= NULL;

		CPepeEngineConverter::getChars(strEntryPoint, &strEntry);
		CPepeEngineConverter::getChars(strShaderVersion, &strTarget);

		if (FAILED(hr = D3DXCompileShaderFromFile(
			strShaderProgram.c_str(), NULL, NULL, strEntry, strTarget,
			NULL, &pPSShaderBuffer, NULL, &m_pConstantTable)))
		{
			tstring msg = DXGetErrorDescription9(hr);			
			
			delete[] strEntry;
			delete[] strTarget;

			PEPE_ENGINE_EXCEPT(
				CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
				_T("Create pixel shader from file error: ") + msg, 
				_T("CPepeEngineDirect3D9PixelShader::create")
			);
		} else
		{
			if (FAILED(hr = m_pDevice->CreatePixelShader(
				(DWORD*)pPSShaderBuffer->GetBufferPointer(), &m_pPSShader)))
			{
				tstring msg = DXGetErrorDescription9(hr);
				
				delete[] strEntry;
				delete[] strTarget;

				PEPE_ENGINE_EXCEPT(
					CPepeEngineException::ERR_RENDERINGAPI_ERROR, 
					_T("Create pixel shader from file error: ") + msg, 
					_T("CPepeEngineDirect3D9PixelShader::create")
				);
				
			}

			m_bIsLoaded			= true;
			
			delete[] strEntry;
			delete[] strTarget;

			buildConstantDefinitions();

			SAFE_RELEASE(pPSShaderBuffer);
		}
	} else
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_INVALID_PARAMETERS, 
			_T("All parameters are required"), 
			_T("CPepeEngineDirect3D9PixelShader::create")
		);
	}
}

// -----------------------------------------------------------------------------------------
Direct3D9GPUProgramPtr::Direct3D9GPUProgramPtr() : CPepeEngineSharedPtr<CPepeEngineDirect3D9GPUProgram>()
{
}

// -----------------------------------------------------------------------------------------
Direct3D9GPUProgramPtr::Direct3D9GPUProgramPtr(CPepeEngineDirect3D9GPUProgram* r) 
	: CPepeEngineSharedPtr<CPepeEngineDirect3D9GPUProgram>(r)
{
}

// -----------------------------------------------------------------------------------------
Direct3D9GPUProgramPtr::Direct3D9GPUProgramPtr(const Direct3D9GPUProgramPtr& r) 
	: CPepeEngineSharedPtr<CPepeEngineDirect3D9GPUProgram>(r)
{
}

// -----------------------------------------------------------------------------------------
Direct3D9GPUProgramPtr::Direct3D9GPUProgramPtr(const ResourcePtr& r) 
	: CPepeEngineSharedPtr<CPepeEngineDirect3D9GPUProgram>()
{
	ptr = (CPepeEngineDirect3D9GPUProgram*)r.data();
	if (ptr) 
	{
		ptr->ref();
	}
}

// -----------------------------------------------------------------------------------------
Direct3D9GPUProgramPtr& Direct3D9GPUProgramPtr::operator=(const ResourcePtr& r)
{
	if (ptr == (CPepeEngineDirect3D9GPUProgram*)r.data()) 
	{
		return *this;
	}
	if (ptr)
	{
		ptr->deref();
	}
	ptr = (CPepeEngineDirect3D9GPUProgram*)r.data();
	if (ptr) 
	{
		ptr->ref();
	}
	return *this;
}

_PEPE_ENGINE_END