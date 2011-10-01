/**
 * Project:     PepeEngine
 * Tier:        Frontend
 * File:        CPepeEnginePixelFormat.h
 *
 * @brief       Declaration of CPepeEnginePixelFormat class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-11-19
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version     1.0
 */

#ifndef CPEPEENGINEPIXELFORMAT_H
#define CPEPEENGINEPIXELFORMAT_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

/**
 *  The pixel format used for images, textures, and render surfaces
 */
enum PixelFormat {
    PF_UNKNOWN      = 0,            /**<    Unknown pixel format.                                                   */
    PF_L8           = 1,            /**<    8-bit pixel format, all bits luminace.                                  */
    PF_BYTE_L       = PF_L8,        /**<    8-bit pixel format, all bits luminace.                                  */
    PF_L16          = 2,            /**<    16-bit pixel format, all bits luminace.                                 */
    PF_SHORT_L      = PF_L16,       /**<    16-bit pixel format, all bits luminace.                                 */
    PF_A8           = 3,            /**<    8-bit pixel format, all bits alpha.                                     */
    PF_BYTE_A       = PF_A8,        /**<    8-bit pixel format, all bits alpha.                                     */
    PF_A4L4         = 4,            /**<    8-bit pixel format, 4 bits alpha, 4 bits luminance.                     */
    PF_BYTE_LA      = 5,            /**<    2 byte pixel format, 1 byte luminance, 1 byte alpha                     */
    PF_R5G6B5       = 6,            /**<    16-bit pixel format, 5 bits red, 6 bits green, 5 bits blue.             */
    PF_B5G6R5       = 7,            /**<    16-bit pixel format, 5 bits red, 6 bits green, 5 bits blue.             */
    PF_R3G3B2       = 31,           /**<    8-bit pixel format, 2 bits blue, 3 bits green, 3 bits red.              */
    PF_A4R4G4B4     = 8,            /**<    16-bit pixel format, 4 bits for alpha, red, green and blue.             */
    PF_A1R5G5B5     = 9,            /**<    16-bit pixel format, 5 bits for blue, green, red and 1 for alpha.       */
    PF_R8G8B8       = 10,           /**<    24-bit pixel format, 8 bits for red, green and blue.                    */
    PF_B8G8R8       = 11,           /**<    24-bit pixel format, 8 bits for blue, green and red.                    */
    PF_A8R8G8B8     = 12,           /**<    32-bit pixel format, 8 bits for alpha, red, green and blue.             */
    PF_A8B8G8R8     = 13,           /**<    32-bit pixel format, 8 bits for blue, green, red and alpha.             */
    PF_B8G8R8A8     = 14,           /**<    32-bit pixel format, 8 bits for blue, green, red and alpha.             */
    PF_R8G8B8A8     = 28,           /**<    32-bit pixel format, 8 bits for red, green, blue and alpha.             */

    /**
     *  32-bit pixel format, 8 bits for red, 8 bits for green, 8 bits for blue
     *  like PF_A8R8G8B8, but alpha will get discarded
     */
    PF_X8R8G8B8     = 26,

    /**
     *  32-bit pixel format, 8 bits for blue, 8 bits for green, 8 bits for red
     *  like PF_A8B8G8R8, but alpha will get discarded
     */
    PF_X8B8G8R8     = 27,           /**<    3 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue  */
    PF_BYTE_RGB     = PF_R8G8B8,    /**<    3 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue  */
    PF_BYTE_BGR     = PF_B8G8R8,    /**<    3 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red  */

    /**
     *  4 byte pixel format, 1 byte for blue, 1 byte for green, 1 byte for red and one byte for alpha
     */
    PF_BYTE_BGRA    = PF_B8G8R8A8,

    /**
     *  4 byte pixel format, 1 byte for red, 1 byte for green, 1 byte for blue, and one byte for alpha
     */
    PF_BYTE_RGBA    = PF_R8G8B8A8,

    PF_A2R10G10B10  = 15,           /**<    32-bit pixel format, 2 bits for alpha, 10 bits for red, green and blue. */
    PF_A2B10G10R10  = 16,           /**<    32-bit pixel format, 10 bits for blue, green and red, 2 bits for alpha. */
    PF_DXT1         = 17,           /**<    DDS (DirectDraw Surface) DXT1 format                                    */
    PF_DXT2         = 18,           /**<    DDS (DirectDraw Surface) DXT2 format                                    */
    PF_DXT3         = 19,           /**<    DDS (DirectDraw Surface) DXT3 format                                    */
    PF_DXT4         = 20,           /**<    DDS (DirectDraw Surface) DXT4 format                                    */
    PF_DXT5         = 21,           /**<    DDS (DirectDraw Surface) DXT5 format                                    */
    PF_FLOAT16_R    = 32,           /**<    16-bit pixel format, 16 bits (float) for red                            */

    /**
     *  48-bit pixel format, 16 bits (float) for red, 16 bits (float) for green, 16 bits (float) for blue
     */
    PF_FLOAT16_RGB  = 22,
    /**
     *  64-bit pixel format, 16 bits (float) for red, 16 bits (float) for green,
     *  16 bits (float) for blue, 16 bits (float) for alpha
     */
    PF_FLOAT16_RGBA = 23,
    PF_FLOAT32_R    = 33,           /**<    16-bit pixel format, 16 bits (float) for red                            */
    /**
     *  96-bit pixel format, 32 bits (float) for red, 32 bits (float) for green, 32 bits (float) for blue
     */
    PF_FLOAT32_RGB  = 24,
    /**
     *  128-bit pixel format, 32 bits (float) for red, 32 bits (float) for green,
     *  32 bits (float) for blue, 32 bits (float) for alpha
     */
    PF_FLOAT32_RGBA = 25,
    PF_FLOAT16_GR   = 35,           /**<    32-bit, 2-channel floating point pixel format, 16-bit green, 16-bit red */
    PF_FLOAT32_GR   = 36,           /**<    64-bit, 2-channel floating point pixel format, 32-bit green, 32-bit red */
    PF_DEPTH        = 29,           /**<    Depth texture format                                                    */
    PF_SHORT_RGBA   = 30,           /**<    64-bit pixel format, 16 bits for red, green, blue and alpha             */
    PF_SHORT_GR     = 34,           /**<    32-bit pixel format, 16-bit green, 16-bit red                           */
    PF_SHORT_RGB    = 37,           /**<    48-bit pixel format, 16 bits for red, green and blue                    */
    PF_COUNT        = 38            /**<    Number of pixel formats currently defined                               */
};

typedef std::vector<PixelFormat> PixelFormatList;

/**
 *  Flags defining some on/off properties of pixel formats
 */
enum PixelFormatFlags {
    PFF_HASALPHA        = 0x00000001,   /**<    This format has an alpha channel            */

    /**
     *  This format is compressed. This invalidates the values in elemBytes,
     *  elemBits and the bit counts as these might not be fixed in a compressed format.
     */
    PFF_COMPRESSED      = 0x00000002,
    PFF_FLOAT           = 0x00000004,   /**<    This is a floating point format             */
    PFF_DEPTH           = 0x00000008,   /**<    This is a depth format (for depth textures) */

    /**
     *  Format is in native endian. Generally true for the 16, 24 and 32 bits
     *  formats which can be represented as machine integers.
     */
    PFF_NATIVEENDIAN    = 0x00000010,

    /**
     *  This is an intensity format instead of a RGB one. The luminance
     *  replaces R,G and B. (but not A)
     */
    PFF_LUMINANCE       = 0x00000020
};

/**
 *  Pixel component format
 */
enum PixelComponentType {
    PCT_BYTE    = 0,    /**<    Byte per component (8 bit fixed 0.0..1.0)       */
    PCT_SHORT   = 1,    /**<    Short per component (16 bit fixed 0.0..1.0))    */
    PCT_FLOAT16 = 2,    /**<    16 bit float per component                      */
    PCT_FLOAT32 = 3,    /**<    32 bit float per component                      */
    PCT_COUNT   = 4     /**<    Number of pixel types                           */
};

/**
 *  A record that describes a pixel format in detail.
 */
struct PixelFormatDescription {
    /**
     *  Name of the format, as in the enum
     */
    const char *name;

    /**
     *  Number of bytes one element (colour value) takes.
     */
    unsigned char elemBytes;

    /**
     *  Pixel format flags, see enum PixelFormatFlags for the bit field
     *  definitions
     */
    unsigned int flags;

    /**
     *  Component type
     */
    PixelComponentType componentType;

    /**
     *  Component count
     */
    unsigned char componentCount;

    /**
     *  Number of bits for red(or luminance), green, blue, alpha
     */
    unsigned char rbits, gbits, bbits, abits;

    /**
     *  Masks and shifts as used by packers/unpackers
     */
    unsigned int rmask, gmask, bmask, amask;
    unsigned char rshift, gshift, bshift, ashift;
};

class _PepeEngineExport CPepeEnginePixelFormat
{
public:
    static const PixelFormatDescription& getPixelFormatDescription(const PixelFormat& pf);
};

_PEPE_ENGINE_END

#endif