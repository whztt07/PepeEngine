#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineTextureUnitState.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineTextureUnitState::CPepeEngineTextureUnitState()
{
	if (CPepeEngineCore::getSingleton().getRenderer()->getRendererCapabilities()->hasCapability(RC_ANISOTROPY))
	{
		setTextureFilterOptions(TFO_ANISOTROPIC);
	} else
	{
		setTextureFilterOptions(TFO_TRILINEAR);
	}

	setTextureAddressingMode(TAM_WRAP);
	setTextureType(TEX_TYPE_2D);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineTextureUnitState::setTextureFilterOptions(const TextureFilterOptions& texFilOpt)
{
	switch (texFilOpt)
	{
	case TFO_NONE:
		setFilterOptions(FT_MIN, FO_POINT);
		setFilterOptions(FT_MAG, FO_POINT);
		setFilterOptions(FT_MIP, FO_NONE);
		break;
	case TFO_BILINEAR:
		setFilterOptions(FT_MIN, FO_LINEAR);
		setFilterOptions(FT_MAG, FO_LINEAR);
		setFilterOptions(FT_MIP, FO_POINT);
		break;
	case TFO_TRILINEAR:
		setFilterOptions(FT_MIN, FO_LINEAR);
		setFilterOptions(FT_MAG, FO_LINEAR);
		setFilterOptions(FT_MIP, FO_LINEAR);
		break;
	case TFO_ANISOTROPIC:
		setFilterOptions(FT_MIN, FO_ANISOTROPIC);
		setFilterOptions(FT_MAG, FO_ANISOTROPIC);
		setFilterOptions(FT_MIP, FO_LINEAR);
		break;
	default:
		setFilterOptions(FT_MIN, FO_LINEAR);
		setFilterOptions(FT_MAG, FO_LINEAR);
		setFilterOptions(FT_MIP, FO_LINEAR);
	}
}

// -----------------------------------------------------------------------------------------
const TextureFilterOptions& CPepeEngineTextureUnitState::getTextureFilterOptions() const
{
	return m_textureFilter;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineTextureUnitState::setTextureAddressingMode(const TextureAddressingMode& texAddrMode)
{
	m_addressingMode		= texAddrMode;
	m_uvwAddressingMode.u	= texAddrMode;
	m_uvwAddressingMode.v	= texAddrMode;
	m_uvwAddressingMode.w	= texAddrMode;
}

// -----------------------------------------------------------------------------------------
const TextureAddressingMode& CPepeEngineTextureUnitState::getTextureAddressingMode() const
{
	return m_addressingMode;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineTextureUnitState::setTextureUAddressingMode(const TextureAddressingMode& texAddrMode)
{
	m_uvwAddressingMode.u = texAddrMode;
}

// -----------------------------------------------------------------------------------------
const TextureAddressingMode& CPepeEngineTextureUnitState::getTextureUAddressingMode() const
{
	return m_uvwAddressingMode.u;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineTextureUnitState::setTextureVAddressingMode(const TextureAddressingMode& texAddrMode)
{
	m_uvwAddressingMode.v = texAddrMode;
}

// -----------------------------------------------------------------------------------------
const TextureAddressingMode& CPepeEngineTextureUnitState::getTextureVAddressingMode() const
{
	return m_uvwAddressingMode.v;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineTextureUnitState::setTextureWAddressingMode(const TextureAddressingMode& texAddrMode)
{
	m_uvwAddressingMode.w = texAddrMode;
}

// -----------------------------------------------------------------------------------------
const TextureAddressingMode& CPepeEngineTextureUnitState::getTextureWAddressingMode() const
{
	return m_uvwAddressingMode.w;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineTextureUnitState::setTextureUVWAddressingMode(const UVWTextureAddressingMode& texAddrMode)
{
	m_uvwAddressingMode = texAddrMode;
}

// -----------------------------------------------------------------------------------------
const UVWTextureAddressingMode& CPepeEngineTextureUnitState::getTextureUVWAddressingMode() const
{
	return m_uvwAddressingMode;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineTextureUnitState::setFilterOptions(const FilterType& filterType, const FilterOptions& filterOpt)
{
	switch (filterType)
	{
	case FT_MIP:
		m_mipFilter = filterOpt;
		break;
	case FT_MAG:
		m_magFilter = filterOpt;
		break;
	case FT_MIN:
		m_minFilter = filterOpt;
		break;
	}
}

// -----------------------------------------------------------------------------------------
const FilterOptions& CPepeEngineTextureUnitState::getFilterOptions(const FilterType& filterType) const
{
	switch (filterType)
	{
	case FT_MIP:
		return m_mipFilter;		
	case FT_MAG:
		return m_magFilter;		
	case FT_MIN:
		return m_minFilter;	
	}

	return m_minFilter;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineTextureUnitState::setTextureType(const TextureType& texType)
{
	m_textureType = texType;
}

// -----------------------------------------------------------------------------------------
const TextureType& CPepeEngineTextureUnitState::getTextureType() const
{
	return m_textureType;
}

_PEPE_ENGINE_END