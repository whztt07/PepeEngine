#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineRendererCapabilities.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineRendererCapabilities::CPepeEngineRendererCapabilities(): 
	m_nNumWorldMatrices(0), m_nNumTextureUnits(0), m_nStencilBufferBitDepth(0),
	m_nNumVertexBlendMatrices(0), m_nCapabilities(0), m_nNumMultiRenderTargets(1), 
	m_bNonPOW2TexturesLimited(false)
{
}

// -----------------------------------------------------------------------------------------
CPepeEngineRendererCapabilities::~CPepeEngineRendererCapabilities()
{
}

// -----------------------------------------------------------------------------------------
void CPepeEngineRendererCapabilities::log()
{
	LOG("Renderer capabilities");
	LOG("-------------------------");
	LOG("RenderSystem Name: "					+ CPepeEngineCore::getSingleton().getRenderer()->getName());
	//LOG("GPU Vendor: "							+ vendorToString(getVendor()));
	//LOG("Device Name: "							+ getDeviceName());
	//LOG("Driver Version: "						+ getDriverVersion().parseString());	
	LOG(" * Hardware generation of mipmaps: "	+ CPepeEngineConverter::parseString(hasCapability(RC_AUTOMIPMAP)));
	LOG(" * Texture blending: "					+ CPepeEngineConverter::parseString(hasCapability(RC_BLENDING)));
	LOG(" * Anisotropic texture filtering: "	+ CPepeEngineConverter::parseString(hasCapability(RC_ANISOTROPY)));
	LOG(" * Dot product texture operation: "	+ CPepeEngineConverter::parseString(hasCapability(RC_DOT3)));
	LOG(" * Cube mapping: "						+ CPepeEngineConverter::parseString(hasCapability(RC_CUBEMAPPING)));
	LOG(" * Hardware stencil buffer: "			+ CPepeEngineConverter::parseString(hasCapability(RC_HWSTENCIL)));

	if (hasCapability(RC_HWSTENCIL))
	{
		LOG("   - Stencil depth: "				+ CPepeEngineConverter::parseString((float)getStencilBufferBitDepth()));
		LOG("   - Two sided stencil support: "	+ CPepeEngineConverter::parseString(hasCapability(RC_TWO_SIDED_STENCIL)));
		LOG("   - Wrap stencil values: "		+ CPepeEngineConverter::parseString(hasCapability(RC_STENCIL_WRAP)));
	}

	LOG(" * Hardware vertex / index buffers: "	+ CPepeEngineConverter::parseString(hasCapability(RC_VBO)));
	LOG(" * Vertex programs: "					+ CPepeEngineConverter::parseString(hasCapability(RC_VERTEX_PROGRAM)));
	LOG("   - Max vertex program version: "		+ getMaxVertexProgramVersion());
	LOG(" * Pixel programs: "					+ CPepeEngineConverter::parseString(hasCapability(RC_PIXEL_PROGRAM)));
	LOG("   - Max pixel program version: "		+ getMaxPixelProgramVersion());
	

	LOG(" * Texture Compression: "		+ CPepeEngineConverter::parseString(hasCapability(RC_TEXTURE_COMPRESSION)));

	if (hasCapability(RC_TEXTURE_COMPRESSION))
	{
		LOG("   - DXT: "	+ CPepeEngineConverter::parseString(hasCapability(RC_TEXTURE_COMPRESSION_DXT)));
		LOG("   - VTC: "	+ CPepeEngineConverter::parseString(hasCapability(RC_TEXTURE_COMPRESSION_VTC)));
	}

	LOG(" * Scissor Rectangle: "				+ CPepeEngineConverter::parseString(hasCapability(RC_SCISSOR_TEST)));
	LOG(" * Hardware Occlusion Query: "			+ CPepeEngineConverter::parseString(hasCapability(RC_HWOCCLUSION)));
	LOG(" * User clip planes: "					+ CPepeEngineConverter::parseString(hasCapability(RC_USER_CLIP_PLANES)));
	LOG(" * VET_UBYTE4 vertex element type: "	+ CPepeEngineConverter::parseString(hasCapability(RC_VERTEX_FORMAT_UBYTE4)));
	LOG(" * Infinite far plane projection: "	+ CPepeEngineConverter::parseString(hasCapability(RC_INFINITE_FAR_PLANE)));
	LOG(" * Hardware render-to-texture: "		+ CPepeEngineConverter::parseString(hasCapability(RC_HWRENDER_TO_TEXTURE)));
	LOG(" * Floating point textures: "			+ CPepeEngineConverter::parseString(hasCapability(RC_TEXTURE_FLOAT)));
	LOG(" * Non-power-of-two textures: "		+ CPepeEngineConverter::parseString(hasCapability(RC_NON_POWER_OF_2_TEXTURES)) + (m_bNonPOW2TexturesLimited ? _T(" (limited)") : _T("")));
	LOG(" * Volume textures: "					+ CPepeEngineConverter::parseString(hasCapability(RC_TEXTURE_3D)));
	LOG(" * Multiple Render Targets: "			+ CPepeEngineConverter::parseString((float)m_nNumMultiRenderTargets));
	LOG(" * Point Sprites: "					+ CPepeEngineConverter::parseString(hasCapability(RC_POINT_SPRITES)));
	LOG(" * Extended point parameters: "		+ CPepeEngineConverter::parseString(hasCapability(RC_POINT_EXTENDED_PARAMETERS)));
	LOG(" * Max Point Size: "					+ CPepeEngineConverter::parseString(m_fMaxPointSize));
	LOG(" * Vertex texture fetch: "				+ CPepeEngineConverter::parseString(hasCapability(RC_VERTEX_TEXTURE_FETCH)));
	if (hasCapability(RC_VERTEX_TEXTURE_FETCH))
	{
		LOG("   - Max vertex textures: "		+ CPepeEngineConverter::parseString((float)m_nNumVertexTextureUnits));
		LOG("   - Vertex textures shared: "		+ CPepeEngineConverter::parseString(m_bVertexTextureUnitsShared));
	}
}

_PEPE_ENGINE_END