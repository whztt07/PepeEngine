/**
 * Project: PepeEngine
 * Tier:    Frontend
 * File:    CPepeEngineTextureUnitState.h
 *
 * @brief   Declaration of CPepeEngineTextureUnitState class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-11-17
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINETEXTUREUNITSTATE_H
#define CPEPEENGINETEXTUREUNITSTATE_H

#include "PepeEngineInit.h"

#include "IPepeEngineTexture.h"

_PEPE_ENGINE_START

enum TextureFilterOptions {
    TFO_NONE,           /**<    Equal to: min = FO_POINT, mag = FO_POINT, mip = FO_NONE                 */
    TFO_BILINEAR,       /**<    Equal to: min = FO_LINEAR, mag = FO_LINEAR, mip = FO_POINT              */
    TFO_TRILINEAR,      /**<    Equal to: min = FO_LINEAR, mag = FO_LINEAR, mip = FO_LINEAR             */
    TFO_ANISOTROPIC     /**<    Equal to: min = FO_ANISOTROPIC, max = FO_ANISOTROPIC, mip = FO_LINEAR   */
};

enum FilterType {
    FT_MIN, /**<    The filter used when shrinking a texture    */
    FT_MAG, /**<    The filter used when magnifying a texture   */
    FT_MIP  /**<    The filter used when determining the mipmap */
};

/**
 *  Filtering options for textures / mipmaps.
 */
enum FilterOptions {
    FO_NONE,            /**<    No filtering, used for FILT_MIP to turn off mipmapping                              */
    FO_POINT,           /**<    Use the closest pixel                                                               */
    FO_LINEAR,          /**<    Average of a 2x2 pixel area, denotes bilinear for MIN and MAG, trilinear for MIP    */
    FO_ANISOTROPIC      /**<    Similar to FO_LINEAR, but compensates for the angle of the texture plane            */
};

/**
 *  Texture addressing modes - default is TAM_WRAP.
 */
enum TextureAddressingMode {
    TAM_WRAP,   /**<    Texture wraps at values over 1.0                                                */
    TAM_MIRROR, /**<    Texture mirrors (flips) at joins over 1.0                                       */
    TAM_CLAMP,  /**<    Texture clamps at 1.0                                                           */
    TAM_BORDER  /**<    Texture coordinates outside the range [0.0, 1.0] are set to the border colour   */
};

typedef struct {
    TextureAddressingMode u, v, w;
} UVWTextureAddressingMode;

class _PepeEngineExport CPepeEngineTextureUnitState
{
public:

    inline                                  CPepeEngineTextureUnitState();
    inline void                             setTextureFilterOptions(const TextureFilterOptions& texFilOpt);
    inline const TextureFilterOptions&      getTextureFilterOptions() const;

    inline void                             setTextureAddressingMode(const TextureAddressingMode& texAddrMode);
    inline const TextureAddressingMode&     getTextureAddressingMode() const;

    inline void                             setTextureUAddressingMode(const TextureAddressingMode& texAddrMode);
    inline const TextureAddressingMode&     getTextureUAddressingMode() const;

    inline void                             setTextureVAddressingMode(const TextureAddressingMode& texAddrMode);
    inline const TextureAddressingMode&     getTextureVAddressingMode() const;

    inline void                             setTextureWAddressingMode(const TextureAddressingMode& texAddrMode);
    inline const TextureAddressingMode&     getTextureWAddressingMode() const;

    inline void                             setTextureUVWAddressingMode(const UVWTextureAddressingMode& texAddrMode);
    inline const UVWTextureAddressingMode&  getTextureUVWAddressingMode() const;

    inline void                             setFilterOptions(
        const FilterType& filterType,
        const FilterOptions& filterOpt
    );
    inline const FilterOptions&             getFilterOptions(const FilterType& filterType) const;


    inline void                             setTextureType(const TextureType& texType);
    inline const TextureType&               getTextureType() const;

protected:
    TextureFilterOptions                    m_textureFilter;
    TextureAddressingMode                   m_addressingMode;

    FilterOptions                           m_mipFilter;
    FilterOptions                           m_magFilter;
    FilterOptions                           m_minFilter;

    UVWTextureAddressingMode                m_uvwAddressingMode;

    TextureType                             m_textureType;
};

_PEPE_ENGINE_END

#endif