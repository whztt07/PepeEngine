/**
 * Project:     PepeEngine
 * File:        PepeEngineInit.h
 *
 * @brief       Declaration of constant values, macros etc. used by PepeEngine
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-02-16
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version     1.0
 */

#ifndef PEPEENGINEINIT_H
#define PEPEENGINEINIT_H


// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#pragma warning(disable : 4251)

// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs in VC7 for no justifiable reason on all
// #includes of CPepeEngineSingleton
#pragma warning(disable : 4661)

/** @{ PepeEngine namespace */
#define _PEPE_ENGINE_START  namespace pepeengine {
#define _PEPE_ENGINE_END    }
#define PEPE_ENGINE         using namespace pepeengine;
/** @} */

#include "PepeEnginePlatform.h"

#if PEPE_ENGINE_PLATFORM == PEPE_ENGINE_PLATFORM_WIN32
#   define PEPE_ENGINE_SHOW_MESSAGE(strMessage) ::MessageBox(NULL, (LPCTSTR)strMessage, _T("PepeEngine Error"), MB_ICONSTOP);
#else
#   error operating system not supported    
#endif

#if UNICODE
#define tstring         std::wstring
#define tstringstream   std::wstringstream
#define tofstream       std::wofstream
#define tifstream       std::wifstream
#else
#define tstring         std::string
#define tstringstream   std::stringstream
#define tofstream       std::ofstream
#define tifstream       std::ifstream
#endif

_PEPE_ENGINE_START


class CPepeEngine3DSFileMeshLoader;
class CPepeEngineAutoParamDataSource;
class CPepeEngineAxisAlignedBox;
class CPepeEngineCamera;
class CPepeEngineColor;
class CPepeEngineConfig;
class CPepeEngineConverter;
class CPepeEngineCore;
class CPepeEngineDynamicLibrary;
class CPepeEngineDynamicLibraryManager;
class CPepeEngineEntity;
class CPepeEngineEvent;
class CPepeEngineException;
class CPepeEngineFrustum;
class CPepeEngineGPUProgramParameters;
class CPepeEngineImage;
class CPepeEngineImageManager;
class CPepeEngineIndexData;
class CPepeEngineLight;
class CPepeEngineLog;
class CPepeEngineLogManager;
class CPepeEngineMaterial;
class CPepeEngineMaterialManager;
class CPepeEngineMath;
class CPepeEngineMatrix3;
class CPepeEngineMatrix4;
class CPepeEngineMesh;
class CPepeEngineMeshFactory;
class CPepeEngineMeshManager;
class CPepeEngineObservable;
class CPepeEnginePixelFormat;
class CPepeEnginePlane;
class CPepeEngineQuaternion;
class CPepeEngineRenderOperation;
class CPepeEngineRendererCapabilities;
class CPepeEngineSceneManager;
class CPepeEngineSceneNode;
class CPepeEngineShared;
template<class T> struct CPepeEngineSharedPtr;
template <typename T> class CPepeEngineSingleton;
class CPepeEngineSkyBox;
class CPepeEngineSphere;
class CPepeEngineStringUtil;
class CPepeEngineSubEntity;
class CPepeEngineSubMesh;
class CPepeEngineTextureUnitState;
class CPepeEngineVector3;
class CPepeEngineVector4;
class CPepeEngineVertexData;
class CPepeEngineVertexElement;
class CPepeEngineVertexDeclaration;
class CPepeEngineWireBoundingBox;
class CPepeEngineXFileMeshLoader;
class Degree;
class IPepeEngineBuffer;
class IPepeEngineBufferManager;
class IPepeEngineConfigDialog;
class IPepeEngineFrameListener;
class IPepeEngineGPUProgram;
class IPepeEngineGPUProgramManager;
class IPepeEngineIndexBuffer;
class IPepeEngineListener;
class IPepeEngineMeshLoader;
class IPepeEnginePlugin;
class IPepeEngineRenderable;
class IPepeEngineRenderer;
class IPepeEngineRenderWindow;
class IPepeEngineResource;
class IPepeEngineResourceManager;
class IPepeEngineSimpleRenderable;
class IPepeEngineTexture;
class IPepeEngineTextureManager;
class IPepeEngineTimer;
class IPepeEngineVertexBuffer;
class IPepeEngineVideoMode;
class IPepeEngineVideoModeList;
class IPepeEngineWindow;
class Radian;

// ---- shared ptr ------------------------------------------------------
struct DynamicLibraryPtr;
struct GPUProgramPtr;
struct ImagePtr;
struct MaterialPtr;
struct MeshPtr;
struct TexturePtr;
//end - shared ptr ------------------------------------------------------

// ---- standard macros -------------------------------------------------

#define SAFE_RELEASE(p) {if (p) {(p)->Release(); (p) = NULL;}}
#define LOG(msg) {CPepeEngineLogManager::getSingleton().logMessage(_T("pepe_engine.log"), L ## msg);}

//end - standard macros -------------------------------------------------

_PEPE_ENGINE_END

#include "PepeEngineStdHeaders.h"

#endif