#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineImage.h"
#include "FreeImage/FreeImage.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineImage::CPepeEngineImage(const tstring& strName) : IPepeEngineResource(strName)
{
    m_pBitMap           = NULL;
    m_nWidth            = 0;
    m_nHeight           = 0;
    m_nBPP              = 0;
    m_nSizeInBytes      = 0;
    m_pImageBuffer      = NULL;
    m_format            = PF_UNKNOWN;
}

// -----------------------------------------------------------------------------------------
CPepeEngineImage::~CPepeEngineImage()
{
    if (isLoaded()) {
        unload();
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineImage::load()
{
    if (!isLoaded()) {
        LOG("Loading image: " + getName());
        char *strTextName   = NULL;

        CPepeEngineConverter::getChars(m_strName, &strTextName);

        FREE_IMAGE_FORMAT imageFormat   = FreeImage_GetFileType(strTextName);
        m_pBitMap                       = FreeImage_Load(imageFormat, strTextName);

        m_nWidth    = FreeImage_GetWidth(m_pBitMap);
        m_nHeight   = FreeImage_GetHeight(m_pBitMap);
        m_nBPP      = FreeImage_GetBPP(m_pBitMap);

        FREE_IMAGE_TYPE         imageType   = FreeImage_GetImageType(m_pBitMap);
        FREE_IMAGE_COLOR_TYPE   colourType  = FreeImage_GetColorType(m_pBitMap);

        switch (imageType) {
        case FIT_UNKNOWN:
        case FIT_COMPLEX:
        case FIT_UINT32:
        case FIT_INT32:
        case FIT_DOUBLE:
        default:
            PEPE_ENGINE_EXCEPT(
                CPepeEngineException::ERR_ITEM_NOT_FOUND_ERROR,
                _T("Unknown or unsupported image format"),
                _T("CPepeEngineImage::load")
            );

            break;

        case FIT_BITMAP:

            // Standard image type
            // Perform any colour conversions for greyscale
            if (colourType == FIC_MINISWHITE || colourType == FIC_MINISBLACK) {
                FIBITMAP* newBitmap = FreeImage_ConvertToGreyscale(m_pBitMap);
                // free old bitmap and replace
                FreeImage_Unload(m_pBitMap);

                // get new formats
                m_pBitMap   = newBitmap;
                m_nBPP      = FreeImage_GetBPP(m_pBitMap);
                colourType  = FreeImage_GetColorType(m_pBitMap);
            }
            // Perform any colour conversions for RGB
            else if (m_nBPP < 8 || colourType == FIC_PALETTE || colourType == FIC_CMYK) {
                FIBITMAP* newBitmap = FreeImage_ConvertTo24Bits(m_pBitMap);
                // free old bitmap and replace
                FreeImage_Unload(m_pBitMap);

                // get new formats
                m_pBitMap   = newBitmap;
                m_nBPP      = FreeImage_GetBPP(m_pBitMap);
                colourType  = FreeImage_GetColorType(m_pBitMap);
            }

            switch(m_nBPP) {
            case 8:
                m_format = PF_L8;
                break;

            case 16:

                // Determine 555 or 565 from green mask
                // cannot be 16-bit greyscale since that's FIT_UINT16
                if(FreeImage_GetGreenMask(m_pBitMap) == FI16_565_GREEN_MASK) {
                    m_format = PF_R5G6B5;
                } else {
                    // FreeImage doesn't support 4444 format so must be 1555
                    m_format = PF_A1R5G5B5;
                }

                break;

            case 24:
                m_format = PF_BYTE_RGB;
                break;

            case 32:
                m_format = PF_BYTE_RGBA;
                break;
            };

            break;

        case FIT_UINT16:
        case FIT_INT16:
            // 16-bit greyscale
            m_format = PF_L16;
            break;

        case FIT_FLOAT:
            // Single-component floating point data
            m_format = PF_FLOAT32_R;
            break;

        case FIT_RGB16:
            m_format = PF_SHORT_RGB;
            break;

        case FIT_RGBA16:
            m_format = PF_SHORT_RGBA;
            break;

        case FIT_RGBF:
            m_format = PF_FLOAT32_RGB;
            break;

        case FIT_RGBAF:
            m_format = PF_FLOAT32_RGBA;
            break;
        };

        unsigned char* srcData  = FreeImage_GetBits(m_pBitMap);

        unsigned srcPitch       = FreeImage_GetPitch(m_pBitMap);

        // Final data - invert image and trim pitch at the same time
        size_t dstPitch = m_nWidth * m_nBPP / 8;

        m_nSizeInBytes  = dstPitch * m_nHeight;

        unsigned char* pSrc;

        m_pImageBuffer = new unsigned char[m_nSizeInBytes];

        for (size_t y = 0; y < m_nHeight; ++y) {
            pSrc = srcData + (m_nHeight - y - 1) * srcPitch;
            memcpy(m_pImageBuffer + (y * dstPitch), pSrc, dstPitch);
        }

        delete strTextName;

        m_bIsLoaded = true;
        LOG("Image successfully loaded");
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineImage::unload()
{
    if (isLoaded()) {
        if (m_pBitMap) {
            FreeImage_Unload(m_pBitMap);
            m_pBitMap = NULL;
        }

        delete m_pImageBuffer;
        m_pImageBuffer  = NULL;
        m_bIsLoaded     = false;
    }
}

// -----------------------------------------------------------------------------------------
void* CPepeEngineImage::ptr()
{
    if (isLoaded()) {
        return (void*)m_pImageBuffer;
    } else {
        return NULL;
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineImage::convert(const PixelFormat& pf)
{
    if (pf != PF_UNKNOWN && pf != m_format) {
        PixelFormatDescription desc  = CPepeEnginePixelFormat::getPixelFormatDescription(pf);

        switch (desc.elemBytes) {
        case 1:
            if (m_nBPP != 8) {
                FIBITMAP* newBitmap = FreeImage_ConvertTo8Bits(m_pBitMap);
                // free old bitmap and replace
                FreeImage_Unload(m_pBitMap);

                // get new formats
                m_pBitMap   = newBitmap;
                m_nBPP      = FreeImage_GetBPP(m_pBitMap);
                m_format    = pf;
            }

            break;

        case 2:
            if (m_nBPP != 16) {
                FIBITMAP* newBitmap = NULL;

                if (desc.gbits == 6) {
                    newBitmap = FreeImage_ConvertTo16Bits565(m_pBitMap);
                } else {
                    newBitmap = FreeImage_ConvertTo16Bits555(m_pBitMap);
                }

                // free old bitmap and replace
                FreeImage_Unload(m_pBitMap);

                // get new formats
                m_pBitMap   = newBitmap;
                m_nBPP      = FreeImage_GetBPP(m_pBitMap);
                m_format    = pf;
            }

            break;

        case 3:
            if (m_nBPP != 24) {
                FIBITMAP* newBitmap = FreeImage_ConvertTo24Bits(m_pBitMap);
                // free old bitmap and replace
                FreeImage_Unload(m_pBitMap);

                // get new formats
                m_pBitMap   = newBitmap;
                m_nBPP      = FreeImage_GetBPP(m_pBitMap);
                m_format    = pf;
            }

            break;

        case 4:
            if (m_nBPP != 32) {
                FIBITMAP* newBitmap = FreeImage_ConvertTo32Bits(m_pBitMap);
                // free old bitmap and replace
                FreeImage_Unload(m_pBitMap);

                // get new formats
                m_pBitMap   = newBitmap;
                m_nBPP      = FreeImage_GetBPP(m_pBitMap);
                m_format    = pf;
            }

            break;

        default:
            PEPE_ENGINE_EXCEPT(
                CPepeEngineException::ERR_INVALID_PARAMETERS,
                _T("Cannot convert image: ") + m_strName + _T(" to requested format."),
                _T("CPepeEngineImage::load")
            );
        }

        unsigned char* srcData  = FreeImage_GetBits(m_pBitMap);
        unsigned srcPitch       = FreeImage_GetPitch(m_pBitMap);

        // Final data - invert image and trim pitch at the same time
        size_t dstPitch = m_nWidth * m_nBPP / 8;
        m_nSizeInBytes  = dstPitch * m_nHeight;

        delete m_pImageBuffer;

        unsigned char* pSrc;
        m_pImageBuffer = new unsigned char[m_nSizeInBytes];

        for (size_t y = 0; y < m_nHeight; ++y) {
            pSrc = srcData + (m_nHeight - y - 1) * srcPitch;
            memcpy(m_pImageBuffer + (y * dstPitch), pSrc, dstPitch);
        }
    }
}

// -----------------------------------------------------------------------------------------
ImagePtr::ImagePtr() : CPepeEngineSharedPtr<CPepeEngineImage>()
{
}

// -----------------------------------------------------------------------------------------
ImagePtr::ImagePtr(CPepeEngineImage* r)
    : CPepeEngineSharedPtr<CPepeEngineImage>(r)
{
}

// -----------------------------------------------------------------------------------------
ImagePtr::ImagePtr(const ImagePtr& r)
    : CPepeEngineSharedPtr<CPepeEngineImage>(r)
{
}

// -----------------------------------------------------------------------------------------
ImagePtr::ImagePtr(const ResourcePtr& r) : CPepeEngineSharedPtr<CPepeEngineImage>()
{
    ptr = (CPepeEngineImage*)r.data();

    if (ptr) {
        ptr->ref();
    }
}

// -----------------------------------------------------------------------------------------
ImagePtr& ImagePtr::operator=(const ResourcePtr& r)
{
    if (ptr == (CPepeEngineImage*)r.data()) {
        return *this;
    }

    if (ptr) {
        ptr->deref();
    }

    ptr = (CPepeEngineImage*)r.data();

    if (ptr) {
        ptr->ref();
    }

    return *this;
}

_PEPE_ENGINE_END