/**
 * Project: PepeEngine
 * Tier:	Backend 
 * File:	CPepeEngineDirect3D9VideoModeList.h 
 *
 * @brief	Declaration of CPepeEngineDirect3D9VideoModeList class. This class use Direct3D 9 library. 		
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-11-10
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEDIRECT3D9VIDEOMODELIST_H
#define CPEPEENGINEDIRECT3D9VIDEOMODELIST_H

#include "PepeEngineDirect3D9Init.h"

#include "CPepeEngineDirect3D9Renderer.h"
#include "IPepeEngineVideoModeList.h"

_PEPE_ENGINE_START

class CPepeEngineDirect3D9VideoMode : public IPepeEngineVideoMode
{
	D3DDISPLAYMODE	m_displayMode;	
public:
	CPepeEngineDirect3D9VideoMode() : IPepeEngineVideoMode()
	{ 		
		ZeroMemory(&m_displayMode, sizeof(D3DDISPLAYMODE)); 
	}

	CPepeEngineDirect3D9VideoMode(const CPepeEngineDirect3D9VideoMode &ob) : IPepeEngineVideoMode()
	{ 	
		m_displayMode	= ob.m_displayMode; 
	}

	CPepeEngineDirect3D9VideoMode(D3DDISPLAYMODE d3ddm) : IPepeEngineVideoMode()
	{ 		
		m_displayMode	= d3ddm; 
	}

	unsigned int getWidth() const {return m_displayMode.Width;}
	unsigned int getHeight() const {return m_displayMode.Height;}
	D3DFORMAT getFormat() const {return m_displayMode.Format;}
	unsigned int getRefreshRate() const {return m_displayMode.RefreshRate;}
	unsigned int getColourDepth() const;
	D3DDISPLAYMODE getDisplayMode() const {return m_displayMode;}
	void increaseRefreshRate(unsigned int rr) {m_displayMode.RefreshRate = rr;} 
	tstring getDescription() const;
};

class CPepeEngineDirect3D9VideoModeList : public IPepeEngineVideoModeList
{
	typedef std::vector<CPepeEngineDirect3D9VideoMode> ModeList;

	ModeList	m_modeList;
	LPDIRECT3D9	m_pD3D;
public:

	CPepeEngineDirect3D9VideoModeList(LPDIRECT3D9 pD3D);
	~CPepeEngineDirect3D9VideoModeList();

	bool enumerate();

	IPepeEngineVideoMode* item(size_t index);
	size_t count();

	IPepeEngineVideoMode* item(const tstring &name);
};

_PEPE_ENGINE_END

#endif