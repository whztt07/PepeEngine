/**
 * Project: PepeEngine
 * Tier:	backend/video/direct3d9
 * File:	PepeEngineDirect3D9Init.h 
 *
 * @brief	Declaration of constant values, macros etc. used by PepeEngine in Direct3D9 mode.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-05-23
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef PEPEENGINEDIRECT3D9INIT_H
#define PEPEENGINEDIRECT3D9INIT_H

#include "PepeEngineStdHeaders.h"
#include <windows.h>

using namespace std;

#include "PepeEngineInit.h"

// Direct3D9 includes

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

_PEPE_ENGINE_START

/**
 * Should we ask D3D to manage vertex/index buffers automatically?
 * Doing so avoids lost devices, but also has a performance impact
 * which is unacceptably bad when using very large buffers
 */
#define PEPE_ENGINE_D3D_MANAGE_BUFFERS	1

class CPepeEngineDirect3D9BufferManager;
class CPepeEngineDirect3D9GPUProgram;
class CPepeEngineDirect3D9GPUProgramManager;
class CPepeEngineDirect3D9IndexBuffer;
class CPepeEngineDirect3D9Mapper;
class CPepeEngineDirect3D9PixelShader;
class CPepeEngineDirect3D9Plugin;
class CPepeEngineDirect3D9Renderer;
class CPepeEngineDirect3D9RenderWindow;
class CPepeEngineDirect3D9VertexBuffer;
class CPepeEngineDirect3D9VertexDeclaration;
class CPepeEngineDirect3D9VideoMode;
class CPepeEngineDirect3D9VideoModeList;
class CPepeEngineDirect3D9VertexShader;
class CPepeEngineDirect3D9Texture;
class CPepeEngineDirect3D9TextureManager;


#if PEPE_ENGINE_PLATFORM == PEPE_ENGINE_PLATFORM_WIN32
#	if defined(PEPE_ENGINE_D3D9_STATIC_LIB)
#		define _PepeEngineWin32Export
#	else
#		if defined(PEPE_ENGINE_D3D9_NONCLIENT_BUILD)
#			define _PepeEngineD3D9Export __declspec(dllexport)
#		else
# 			define _PepeEngineD3D9Export __declspec(dllimport)
#		endif
#	endif
#else
#	error operating system not supported	
#endif


_PEPE_ENGINE_END

#endif