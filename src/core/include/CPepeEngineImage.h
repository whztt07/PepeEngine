/**
 * Project:		PepeEngine
 * Tier:		Frontend
 * File:		CPepeEngineImage.h   
 *
 * @brief		Declaration of CPepeEngineImage class.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-11-12
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef CPEPEENGINEIMAGE_H
#define CPEPEENGINEIMAGE_H

#include "PepeEngineInit.h"

#include "CPepeEnginePixelFormat.h"
#include "IPepeEngineResource.h"

struct FIBITMAP;

_PEPE_ENGINE_START
  
class _PepeEngineExport CPepeEngineImage : public IPepeEngineResource
{	
	FIBITMAP*				m_pBitMap;
	unsigned char*			m_pImageBuffer;
	size_t					m_nWidth;
	size_t					m_nHeight;	
	size_t					m_nBPP;
	size_t					m_nSizeInBytes;
	PixelFormat				m_format;	
public:
	CPepeEngineImage(const tstring& strName);
	~CPepeEngineImage();

	void load();
	void unload();

	void* ptr();

	size_t size() const {return m_nSizeInBytes;}
	size_t getWidth() const {return m_nWidth;}
	size_t getHeight() const {return m_nHeight;}
	size_t getBPP() const {return m_nBPP;}
	const PixelFormat& getFormat() const {return m_format;}	
	
	void convert(const PixelFormat& pf);
};

struct _PepeEngineExport ImagePtr : public CPepeEngineSharedPtr<CPepeEngineImage>
{
public:
	inline				ImagePtr();
	inline explicit		ImagePtr(CPepeEngineImage* r);
	inline				ImagePtr(const ImagePtr& r);
	inline				ImagePtr(const ResourcePtr& r); 
	inline ImagePtr&	operator=(const ResourcePtr& r);
};

_PEPE_ENGINE_END

#endif