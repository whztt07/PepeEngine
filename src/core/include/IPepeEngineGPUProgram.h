/**
 * Project: PepeEngine
 * Tier:	Middleware
 * File:	IPepeEngineGPUProgram.h 
 *
 * @brief	Declaration of IPepeEngineGPUProgram interface.  
 *			
 * @author		Piotr 'pepe' Picheta
 * @date		2008-09-16
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef IPEPEENGINEGPUPROGRAM_H
#define IPEPEENGINEGPUPROGRAM_H

#include "PepeEngineInit.h"

#include "IPepeEngineResource.h"

_PEPE_ENGINE_START

enum ShaderType
{
	GPU_PIXEL_SHADER	= 1,
	GPU_VERTEX_SHADER	= 2
};

/** 
 *	Enumeration of the types of constant we may encounter in programs.  
 */
enum GpuConstantType
{
	GCT_FLOAT1,
	GCT_FLOAT2,
	GCT_FLOAT3,
	GCT_FLOAT4,
	GCT_SAMPLER1D,
	GCT_SAMPLER2D,
	GCT_SAMPLER3D,
	GCT_SAMPLERCUBE,
	GCT_SAMPLER1DSHADOW,
	GCT_SAMPLER2DSHADOW,
	GCT_MATRIX_2X2,
	GCT_MATRIX_2X3,
	GCT_MATRIX_2X4,
	GCT_MATRIX_3X2,
	GCT_MATRIX_3X3,
	GCT_MATRIX_3X4,
	GCT_MATRIX_4X2,
	GCT_MATRIX_4X3,
	GCT_MATRIX_4X4,
	GCT_INT1,
	GCT_INT2,
	GCT_INT3,
	GCT_INT4,
	GCT_UNKNOWN
};

/** 
 *	Information about predefined program constants. 
 */
struct _PepeEngineExport GpuConstantDefinition
{	
	GpuConstantType m_constType;		/**<	Data type														*/	
	size_t			m_nPhysicalIndex;	/**<	Physical start index in buffer (either float or int buffer)		*/	
	size_t			m_nElementSize;		/**<	Number of raw buffer slots per element 
												(some programs pack each array element to float4, some do not)	*/	
	size_t			m_nArraySize;		/**<	Length of array													*/

	bool isFloat() const
	{
		switch(m_constType)
		{
			case GCT_INT1:
			case GCT_INT2:
			case GCT_INT3:
			case GCT_INT4:
			case GCT_SAMPLER1D:
			case GCT_SAMPLER2D:
			case GCT_SAMPLER3D:
			case GCT_SAMPLERCUBE:
			case GCT_SAMPLER1DSHADOW:
			case GCT_SAMPLER2DSHADOW:
				return false;
			default:
				return true;
		};
	}

	bool isSampler() const
	{
		switch(m_constType)
		{
		case GCT_SAMPLER1D:
		case GCT_SAMPLER2D:
		case GCT_SAMPLER3D:
		case GCT_SAMPLERCUBE:
		case GCT_SAMPLER1DSHADOW:
		case GCT_SAMPLER2DSHADOW:
			return true;
		default:
			return false;
		};

	}

	GpuConstantDefinition()
		: m_constType(GCT_UNKNOWN)
		, m_nPhysicalIndex((std::numeric_limits<size_t>::max)())
		, m_nElementSize(0)
		, m_nArraySize(1) {}
};

struct _PepeEngineExport GpuSamplerDefinition
{
	GpuConstantType		m_constType;
	size_t				m_nLogicalIndex;
	TexturePtr			m_pTexture;

	GpuSamplerDefinition()
		: m_constType(GCT_UNKNOWN)
		, m_nLogicalIndex((std::numeric_limits<size_t>::max)())
		, m_pTexture(0)
		{}
};

typedef std::map<tstring, GpuSamplerDefinition>		GpuSamplerDefinitionMap;
typedef std::map<tstring, GpuConstantDefinition>	GpuConstantDefinitionMap;

struct _PepeEngineExport GpuNamedConstants
{	
	size_t	m_nFloatBufferSize;		/**<	Total size of the float buffer required			*/	
	size_t	m_nIntBufferSize;		/**<	Total size of the int buffer required			*/	
	GpuConstantDefinitionMap map;	/**<	Map of parameter names to GpuConstantDefinition	*/

	/** 
	 *	Generate additional constant entries for arrays based on a base definition.
	 *
	 *	Array uniforms will be added just with their base name with no array
	 *	suffix. This method will add named entries for array suffixes too
	 *	so individual array entries can be addressed. Note that we only 
	 *	individually index array elements if the array size is up to 16
	 *	entries in size. Anything larger than that only gets a [0] entry
	 *	as well as the main entry, to save cluttering up the name map. After
	 *	all, you can address the larger arrays in a bulk fashion much more
	 *	easily anyway. 
	 */
	void generateConstantDefinitionArrayEntries(
		const tstring& strParamName, 
		const GpuConstantDefinition& baseDef
	);
};

/** 
 *	Structure recording the use of a physical buffer by a logical parameter
 *	index. Only used for low-level programs.
 */
struct _PepeEngineExport GpuLogicalIndexUse
{	
	size_t m_nPhysicalIndex;	/**<	Physical buffer index				*/	
	size_t m_nCurrentSize;		/**<	Current physical size allocation	*/

	GpuLogicalIndexUse(size_t bufIdx, size_t curSz) 
		: m_nPhysicalIndex(bufIdx), m_nCurrentSize(curSz) {}
};

typedef std::map<size_t, GpuLogicalIndexUse> GpuLogicalIndexUseMap;

/** 
 *	Container struct to allow params to safely & update shared list of logical buffer assignments
 */
struct _PepeEngineExport GpuLogicalBufferStruct
{	
	GpuLogicalIndexUseMap	map;			/**<	Map from logical index to physical buffer location	*/	
	size_t					m_nBufferSize;	/**<	Shortcut to know the buffer size needs				*/
	GpuLogicalBufferStruct() : m_nBufferSize(0) {}
};

class _PepeEngineExport CPepeEngineGPUProgramParameters
{	
	friend IPepeEngineGPUProgram;
	
	enum AutoConstantType
	{
		ACT_WORLD_MATRIX,					/**<	The current world matrix					*/
		ACT_VIEW_MATRIX,					/**<	The current view matrix						*/
		ACT_PROJECTION_MATRIX,				/**<	The current projection matrix				*/

		/** @{ matrices combination */
		ACT_WORLD_VIEW_MATRIX,
		ACT_VIEW_PROJECTION_MATRIX,
		ACT_WORLD_VIEW_PROJECTION_MATRIX,
		/** @} */

		ACT_MATERIAL_AMBIENT_COLOUR,		/**<	Surface ambient colour						*/	
		ACT_MATERIAL_DIFFUSE_COLOUR,		/**<	Surface diffuse colour						*/	
		ACT_MATERIAL_SPECULAR_COLOUR,		/**<	Surface specular colour						*/	
		ACT_MATERIAL_EMISSIVE_COLOUR,		/**<	Surface emissive colour						*/	
		ACT_MATERIAL_POWER,					/**<	Surface shininess							*/
		
		ACT_AMBIENT_LIGHT_COLOUR,			/**<	The ambient light colour set in the scene	*/
		
		ACT_LIGHT_DIFFUSE_COLOUR,			/**<	Light diffuse colour						*/	
		ACT_LIGHT_SPECULAR_COLOUR,			/**<	Light specular colour						*/	
		ACT_LIGHT_ATTENUATION,				/**<	Light attenuation parameters, 
													CPepeEngineVector4(
														range, constant, linear, quadric)		*/

		/** 
		 *	Spotlight parameters, CPepeEngineVector4(innerFactor, outerFactor, falloff, isSpot)
		 *	innerFactor and outerFactor are cos(angle/2)
		 *	The isSpot parameter is 0.0f for non-spotlights, 1.0f for spotlights.
		 *	Also for non-spotlights the inner and outer factors are 1 and nearly 1 respectively
		 */ 
		ACT_SPOTLIGHT_PARAMS,
		
		ACT_LIGHT_POSITION,					/**<	A light position array in world space								*/
		ACT_LIGHT_DIRECTION,				/**<	A light direction array in world space								*/

		ACT_LIGHT_DIFFUSE_COLOUR_ARRAY,		/**<	Light diffuse colour array (count set by extra param)				*/	
		ACT_LIGHT_SPECULAR_COLOUR_ARRAY,	/**<	Light specular colour array (count set by extra param) 				*/	
		ACT_LIGHT_ATTENUATION_ARRAY,		/**<	Light attenuation parameters array (count set by extra param)		*/
		
		ACT_SPOTLIGHT_PARAMS_ARRAY,			/** 	Spotlight parameters array (count set by extra param)				*/ 
		
		ACT_LIGHT_POSITION_ARRAY,			/**<	A light position array in world space (count set by extra param)	*/
		ACT_LIGHT_DIRECTION_ARRAY,			/**<	A light direction array in world space (count set by extra param)	*/


		ACT_CAMERA_POSITION,				/**<	The current camera position in world space	*/	
		
		ACT_TIME,							/**<	Provides current elapsed time										*/

		/** 
		 *	Single float value, which repeats itself based on given as
		 *	parameter "cycle time". Equivalent to RenderMonkey's "Time0_X".
		 */
		ACT_TIME_0_X,		
		ACT_COSTIME_0_X,					/**<	Cosine of "Time0_X". Equivalent to RenderMonkey's "CosTime0_X".		*/		
		ACT_SINTIME_0_X,					/**<	Sine of "Time0_X". Equivalent to RenderMonkey's "SinTime0_X".		*/		
		ACT_TANTIME_0_X,					/**<	Tangent of "Time0_X". Equivalent to RenderMonkey's "TanTime0_X".	*/

		/** 
		 *	Vector of "Time0_X", "SinTime0_X", "CosTime0_X", 
		 *	"TanTime0_X". Equivalent to RenderMonkey's "Time0_X_Packed".
		 */
		ACT_TIME_0_X_PACKED,

		/** 
		 *	Single float value, which represents scaled time value [0..1],
		 *	which repeats itself based on given as parameter "cycle time".
		 *	Equivalent to RenderMonkey's "Time0_1".
		 */
		ACT_TIME_0_1,		
		ACT_COSTIME_0_1,					/**<	Cosine of "Time0_1". Equivalent to RenderMonkey's "CosTime0_1".		*/		
		ACT_SINTIME_0_1,					/**<	Sine of "Time0_1". Equivalent to RenderMonkey's "SinTime0_1".		*/
		ACT_TANTIME_0_1,					/**<	Tangent of "Time0_1". Equivalent to RenderMonkey's "TanTime0_1".	*/
	
		/** 
		 *	Vector of "Time0_1", "SinTime0_1", "CosTime0_1",
		 *	"TanTime0_1". Equivalent to RenderMonkey's "Time0_1_Packed".
		 */
		ACT_TIME_0_1_PACKED,

		/**	
		 *	Single float value, which represents scaled time value [0..2 * Pi],
		 *	which repeats itself based on given as parameter "cycle time".
		 *	Equivalent to RenderMonkey's "Time0_2PI".
		 */
		ACT_TIME_0_2PI,		
		ACT_COSTIME_0_2PI,					/**<	Cosine of "Time0_2PI". Equivalent to RenderMonkey's "CosTime0_2PI".		*/
		ACT_SINTIME_0_2PI,					/**<	Sine of "Time0_2PI". Equivalent to RenderMonkey's "SinTime0_2PI".		*/		
		ACT_TANTIME_0_2PI,					/**<	Tangent of "Time0_2PI". Equivalent to RenderMonkey's "TanTime0_2PI".	*/

		/** 
		 *	Vector of "Time0_2PI", "SinTime0_2PI", "CosTime0_2PI",
		 *	"TanTime0_2PI". Equivalent to RenderMonkey's "Time0_2PI_Packed".
		 */
		ACT_TIME_0_2PI_PACKED,		
		ACT_FRAME_TIME,						/**<	Provides the scaled frame time, returned as a floating point value.				*/		
		ACT_FPS,							/**<	Provides the calculated frames per second, returned as a floating point value.	*/
	};

	/** 
	 *	Defines the base element type of the auto constant
	 */
	enum ElementType 
	{
		ET_INT,
		ET_FLOAT
	};

	enum ACDataType 
	{	
		ACDT_NONE,		/**<	no data is required								*/		
		ACDT_INT,		/**<	the auto constant requires data of type int		*/		
		ACDT_FLOAT		/**<	the auto constant requires data of type float	*/
	};
	

	/** 
	 *	Structure defining an auto constant that's available for use in 
	 *	a parameters object.
	 */
	struct AutoConstantDefinition
	{
		AutoConstantType	m_acType;
		tstring				m_strName;
		size_t				m_nElementCount;		
		ElementType			m_elementType;		/**<	The type of the constant in the program	*/
		ACDataType			m_dataType;			/**<	The type of any extra data				*/

		
		AutoConstantDefinition(
			AutoConstantType acType, const tstring& strName, 
			size_t nElementCount, ElementType elementType, ACDataType dataType
		) : m_acType(acType), m_strName(strName), m_nElementCount(nElementCount), m_elementType(elementType),
			m_dataType(dataType)
		{
		}
	};	

	/** 
	 *	Structure recording the use of an automatic parameter. 
	 */
	class _PepeEngineExport AutoConstantEntry
	{
	public:		
		AutoConstantType m_paramType;	/**<	The type of parameter					*/		
		
		size_t m_nPhysicalIndex;		/**<	The target (physical) constant index	*/

		/** 
		 *	The number of elements per individual entry in this constant
		 *	Used in case people used packed elements smaller than 4 (e.g. GLSL)
		 *	and bind an auto which is 4-element packed to it 
		 */
		size_t m_nElementCount;

		/**
		 *	Additional information to go with the parameter
		 */
		union
		{
			size_t	nData;
			float	fData;
		};


		AutoConstantEntry(AutoConstantType theType, size_t theIndex, size_t theData, 
			size_t theElemCount = 4)
			: m_paramType(theType), m_nPhysicalIndex(theIndex), m_nElementCount(theElemCount), nData(theData) {}

		AutoConstantEntry(AutoConstantType theType, size_t theIndex, float theData, 
			size_t theElemCount = 4)
			: m_paramType(theType), m_nPhysicalIndex(theIndex), m_nElementCount(theElemCount), fData(theData) {}	
	};

	void writeRawConstants(size_t nPhysicalIndex, const float* pfVal, size_t nCount);
	void writeRawConstants(size_t nPhysicalIndex, const int* piVal, size_t nCount);
	void writeRawConstant(size_t nPhysicalIndex, const CPepeEngineMatrix4& m);
	void writeRawConstant(size_t nPhysicalIndex, const CPepeEngineColor& color, size_t nCount);
	void writeRawConstant(size_t nPhysicalIndex, const CPepeEngineVector4& vec, size_t nCount);
	void writeRawConstant(size_t nPhysicalIndex, float fVal);
	void writeRawConstant(size_t nPhysicalIndex, int nVal);
	void writeRawConstant(size_t nPhysicalIndex, const CPepeEngineVector3& vec);	
	void writeRawConstant(size_t physicalIndex, const CPepeEngineMatrix4* pMatrix, size_t numEntries);

	void setRawAutoConstant(
		size_t nPhysicalIndex, AutoConstantType acType, size_t nExtraInfo, size_t nElementSize = 4
	);

	void setRawAutoConstantFloat(
		size_t nPhysicalIndex, AutoConstantType acType, float fExtraInfo, size_t nElementSize = 4
	);

	size_t getFloatConstantPhysicalIndex(size_t nLogicalIndex, size_t nRequestedSize);
	size_t getIntConstantPhysicalIndex(size_t nLogicalIndex, size_t nRequestedSize);

protected:

	typedef std::vector<AutoConstantEntry>	AutoConstantList;
	typedef std::vector<float>				FloatConstantList;
	typedef std::vector<int>				IntConstantList;

	static AutoConstantDefinition	m_autoConstantDictionary[];
	AutoConstantList				m_autoConstantList;
	FloatConstantList				m_floatConstantList;
	IntConstantList					m_intConstantList;

	/** 
	 *	Logical index to physical index map - for low-level programs
	 *	or high-level programs which pass params this way. 
	 */
	GpuLogicalBufferStruct* m_pFloatLogicalToPhysical;
	
	/** 
	 *	Logical index to physical index map - for low-level programs
	 *	or high-level programs which pass params this way. 
	 */
	GpuLogicalBufferStruct* m_pIntLogicalToPhysical;
	
	/**
	 *	Mapping from parameter names to def - high-level programs are expected to populate this
	 */
	const GpuNamedConstants*	m_pNamedConstants;
	GpuSamplerDefinitionMap*	m_pSamplerList;

	bool m_bTransposeMatrices;
public:

	CPepeEngineGPUProgramParameters()
	{		
		m_pFloatLogicalToPhysical		= NULL;
		m_pIntLogicalToPhysical			= NULL;
		m_pNamedConstants				= NULL;
		m_pSamplerList					= NULL;
		m_bTransposeMatrices			= false;
	}

	void setTransposeMatrices(bool bTransposeMatrices) {m_bTransposeMatrices = bTransposeMatrices;}

	void setNamedConstants(const GpuNamedConstants* pNamedConstants);
	void setLogicalIndexes(GpuLogicalBufferStruct* pFloatIndexMap, GpuLogicalBufferStruct* pIntIndexMap);
	void setSamplers(GpuSamplerDefinitionMap* pSamplerList);

	const size_t getNumAutoConstantDefinitions(void) const;
	const AutoConstantDefinition* getAutoConstantDefinition(const tstring& strName) const;
	const AutoConstantDefinition* getAutoConstantDefinition(
		const CPepeEngineGPUProgramParameters::AutoConstantType& type
	) const;

	void setAutoConstant(size_t nIndex, AutoConstantType acType, size_t nExtraInfo);	
	void setAutoConstantFloat(size_t nIndex, AutoConstantType acType, float fExtraInfo);

	void setNamedConstant(const tstring& strName, float val);
	void setNamedConstant(const tstring& strName, int val);
	void setNamedConstant(const tstring& strName, const CPepeEngineVector4& vec);
	void setNamedConstant(const tstring& strName, const CPepeEngineVector3& vec);
	void setNamedConstant(const tstring& strName, const CPepeEngineMatrix4& m);
	void setNamedConstant(const tstring& strName, const CPepeEngineMatrix4* m, size_t numEntries);
	void setNamedConstant(const tstring& strName, const float *val, size_t count, size_t multiple);
	void setNamedConstant(const tstring& name, const CPepeEngineColor& colour);
	void setNamedConstant(const tstring& strName, const int *val, size_t count, size_t multiple);

	void setNamedAutoConstant(const tstring& strName, AutoConstantType acType, size_t nExtraInfo);
	void setNamedAutoConstantFloat(const tstring& name, AutoConstantType acType, float fData);

	const GpuConstantDefinition* findNamedConstantDefinition(const tstring& strName, bool bThrowException = true);
	GpuSamplerDefinition* findSamplerDefinition(const tstring& strName, bool bThrowException = true);

	bool useModelTexture();
	bool useSkyBox();

	void bindTexture(const tstring& strName, TexturePtr pTexture);

	void updateAutoConstantParameters(const CPepeEngineAutoParamDataSource& source);
	void updateAutoConstantParametersLights(const CPepeEngineAutoParamDataSource& source);
	bool hasAutoConstantParameters() const {return !m_autoConstantList.empty();}

	const GpuLogicalBufferStruct* getFloatLogicalBufferStruct() const {return m_pFloatLogicalToPhysical;}
	const GpuLogicalBufferStruct* getIntLogicalBufferStruct() const {return m_pIntLogicalToPhysical;}

	const float* getFloatPointer(size_t pos) const { return &m_floatConstantList[pos];}
	const int* getIntPointer(size_t pos) const { return &m_intConstantList[pos];}

	void generateAutoConstantList();
};



class _PepeEngineExport IPepeEngineGPUProgram : public IPepeEngineResource
{
protected:		
	tstring							m_strShaderModel;	
	tstring							m_strEntryPoint;	
	ShaderType						m_shaderType;
	CPepeEngineGPUProgramParameters	m_parameters;
	
	/** 
	 *	Logical index to physical index map - for low-level programs
	 *	or high-level programs which pass params this way. 
	 */
	mutable GpuLogicalBufferStruct m_floatLogicalToPhysical;
	
	/** 
	 *	Logical index to physical index map - for low-level programs
	 *	or high-level programs which pass params this way. 
	 */
	mutable GpuLogicalBufferStruct m_intLogicalToPhysical;
	
	/**
	 *	Mapping from parameter names to def - high-level programs are expected to populate this
	 */
	mutable GpuNamedConstants		m_namedConstants;
	mutable GpuSamplerDefinitionMap m_samplerList;

public:

	IPepeEngineGPUProgram(
		const ShaderType& shaderType,
		const tstring &strName, 		
		const tstring& strShaderModel,		
		const tstring& strEntryPoint		
	) : IPepeEngineResource(strName)
	{		
		m_strShaderModel				= strShaderModel;		
		m_strEntryPoint					= strEntryPoint;		
		m_shaderType					= shaderType;

		m_namedConstants.m_nFloatBufferSize = 0;
		m_namedConstants.m_nIntBufferSize	= 0;

		m_floatLogicalToPhysical.m_nBufferSize	= 0;
		m_intLogicalToPhysical.m_nBufferSize	= 0;
	}

	virtual ~IPepeEngineGPUProgram() {}

	virtual void unload()
	{	
		m_namedConstants.map.clear();
		m_namedConstants.m_nFloatBufferSize = 0;
		m_namedConstants.m_nIntBufferSize	= 0;

		m_floatLogicalToPhysical.map.clear();
		m_floatLogicalToPhysical.m_nBufferSize = 0;

		m_intLogicalToPhysical.map.clear();
		m_intLogicalToPhysical.m_nBufferSize = 0;	
	}

	virtual void create(
		const tstring& strShaderProgram, 
		const tstring& strEntryPoint, 
		const tstring& strShaderVersion
	) = 0;

	virtual void setVariable(		
		const tstring& strName,
		const float* pfValues,
		unsigned int nCount
	) = 0;

	virtual void setTexture(			
		const tstring& strName,
		TexturePtr pTexture
	) = 0;

	virtual void buildConstantDefinitions() = 0;	

	const ShaderType& getShaderType() const {return m_shaderType;}	

	CPepeEngineGPUProgramParameters* getParameters() {return &m_parameters;}
	
	void setProgramSamplers(const CPepeEngineTextureUnitState& texUnitState);
};

struct _PepeEngineExport GPUProgramPtr : public CPepeEngineSharedPtr<IPepeEngineGPUProgram>
{
public:
	inline					GPUProgramPtr();
	inline explicit			GPUProgramPtr(IPepeEngineGPUProgram* r);
	inline					GPUProgramPtr(const GPUProgramPtr& r);
	inline					GPUProgramPtr(const ResourcePtr& r); 
	inline GPUProgramPtr&	operator=(const ResourcePtr& r);
};

_PEPE_ENGINE_END

#endif