#include "CPepeEngineDirect3D9VideoModeList.h"
#include "CPepeEngineException.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
unsigned int CPepeEngineDirect3D9VideoMode::getColourDepth() const
{
	unsigned int colourDepth = 16;
	if (m_displayMode.Format == D3DFMT_X8R8G8B8 ||
		m_displayMode.Format == D3DFMT_A8R8G8B8 ||
		m_displayMode.Format == D3DFMT_R8G8B8)
	{
		colourDepth = 32;
	}

	return colourDepth;
}

// -----------------------------------------------------------------------------------------
tstring CPepeEngineDirect3D9VideoMode::getDescription() const
{
	unsigned int	colourDepth = 16;
	if (m_displayMode.Format == D3DFMT_X8R8G8B8 ||
		m_displayMode.Format == D3DFMT_A8R8G8B8 ||
		m_displayMode.Format == D3DFMT_R8G8B8)
	{
		colourDepth = 32;
	}
	
	tstringstream str;

	str << m_displayMode.Width << _T(" x ") << m_displayMode.Height 
		<< _T(" @ ") << colourDepth << _T("-bit colour");
	
	return str.str();
}

// -----------------------------------------------------------------------------------------
CPepeEngineDirect3D9VideoModeList::CPepeEngineDirect3D9VideoModeList(LPDIRECT3D9 pD3D) : IPepeEngineVideoModeList()
{	
	if (pD3D == NULL)
	{
		PEPE_ENGINE_EXCEPT(
			CPepeEngineException::ERR_INVALID_PARAMETERS, 
			_T("Direct3D9 object couldn't be NULL."), 
			_T("CPepeEngineDirect3D9VideoModeList::CPepeEngineDirect3D9VideoModeList")
		);
	}

	m_pD3D = pD3D;
	m_modeList.clear();
	enumerate();
}

// -----------------------------------------------------------------------------------------
CPepeEngineDirect3D9VideoModeList::~CPepeEngineDirect3D9VideoModeList()
{
	m_pD3D = NULL;
	m_modeList.clear();
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineDirect3D9VideoModeList::enumerate()
{
	unsigned int iMode;	
	unsigned int adapter = D3DADAPTER_DEFAULT;

	for (iMode = 0; iMode < m_pD3D->GetAdapterModeCount(adapter, D3DFMT_R5G6B5); iMode++)
	{
		D3DDISPLAYMODE displayMode;
		m_pD3D->EnumAdapterModes(adapter, D3DFMT_R5G6B5, iMode, &displayMode);

		// Filter out low-resolutions
		if (displayMode.Width < 640 || displayMode.Height < 400)
		{
			continue;
		}

		// Check to see if it is already in the list (to filter out refresh rates)
		bool found = false;
		std::vector<CPepeEngineDirect3D9VideoMode>::iterator it;
		for (it = m_modeList.begin(); it != m_modeList.end(); it++)
		{
			D3DDISPLAYMODE oldDisp = it->getDisplayMode();
			if (oldDisp.Width == displayMode.Width &&
				oldDisp.Height == displayMode.Height &&
				oldDisp.Format == displayMode.Format)
			{
				// Check refresh rate and favour higher if poss
				if (oldDisp.RefreshRate < displayMode.RefreshRate)
				{
					it->increaseRefreshRate(displayMode.RefreshRate);
				}
				found = true;
				break;
			}
		}

		if (!found)
		{
			m_modeList.push_back(CPepeEngineDirect3D9VideoMode(displayMode));
		}
	}

	for (iMode = 0; iMode < m_pD3D->GetAdapterModeCount(adapter, D3DFMT_X8R8G8B8); iMode++)
	{
		D3DDISPLAYMODE displayMode;
		m_pD3D->EnumAdapterModes(adapter, D3DFMT_X8R8G8B8, iMode, &displayMode);

		// Filter out low-resolutions
		if( displayMode.Width < 640 || displayMode.Height < 400 )
			continue;

		// Check to see if it is already in the list (to filter out refresh rates)
		bool found = false;
		std::vector<CPepeEngineDirect3D9VideoMode>::iterator it;
		for (it = m_modeList.begin(); it != m_modeList.end(); it++)
		{
			D3DDISPLAYMODE oldDisp = it->getDisplayMode();
			if (oldDisp.Width == displayMode.Width &&
				oldDisp.Height == displayMode.Height &&
				oldDisp.Format == displayMode.Format )
			{
				// Check refresh rate and favour higher if poss
				if (oldDisp.RefreshRate < displayMode.RefreshRate)
				{
					it->increaseRefreshRate(displayMode.RefreshRate);
				}
				found = true;
				break;
			}
		}

		if (!found)
		{
			m_modeList.push_back(CPepeEngineDirect3D9VideoMode(displayMode));
		}
	}

	return true;
}

// -----------------------------------------------------------------------------------------
IPepeEngineVideoMode* CPepeEngineDirect3D9VideoModeList::item(size_t index)
{
	std::vector<CPepeEngineDirect3D9VideoMode>::iterator p = m_modeList.begin();

	return &p[index];
}

// -----------------------------------------------------------------------------------------
size_t CPepeEngineDirect3D9VideoModeList::count()
{
	return m_modeList.size();
}

// -----------------------------------------------------------------------------------------
IPepeEngineVideoMode* CPepeEngineDirect3D9VideoModeList::item(const tstring &name)
{
	std::vector<CPepeEngineDirect3D9VideoMode>::iterator it = m_modeList.begin();
	if (it == m_modeList.end())
	{
		return NULL;
	}

	for (;it != m_modeList.end(); ++it)
	{
		if (it->getDescription() == name)
		{
			return &(*it);
		}
	}

	return NULL;
}

_PEPE_ENGINE_END