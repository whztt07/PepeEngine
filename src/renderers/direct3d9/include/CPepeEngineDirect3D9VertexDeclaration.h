/**
 * Project: PepeEngine
 * Tier:	Backend 
 * File:	CPepeEngineDirect3D9VertexDeclaration.h 
 *
 * @brief	Declaration of CPepeEngineDirect3D9VertexDeclaration class. This class use Direct3D 9 library. 		
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-07-20
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEDIRECT3D9VERTEXDECLARATION_H
#define CPEPEENGINEDIRECT3D9VERTEXDECLARATION_H

#include "PepeEngineDirect3D9Init.h"

#include "CPepeEngineVertexDeclaration.h"

_PEPE_ENGINE_START

class CPepeEngineDirect3D9VertexDeclaration : public CPepeEngineVertexDeclaration
{
protected:   
	LPDIRECT3DVERTEXDECLARATION9	m_lpD3DDecl;    
	LPDIRECT3DDEVICE9				m_pDevice;
	bool							m_bNeedRebuild;
public:
	 CPepeEngineDirect3D9VertexDeclaration(LPDIRECT3DDEVICE9 pDevice);
	 ~CPepeEngineDirect3D9VertexDeclaration();
	
	const CPepeEngineVertexElement& addElement(
		unsigned short nSource, size_t nOffset, VertexElementType theType,
		VertexElementSemantic semantic, unsigned short nIndex = 0);
	
	const CPepeEngineVertexElement& insertElement(
		unsigned short nAtPosition, unsigned short nSource, size_t nOffset, VertexElementType theType,
		VertexElementSemantic semantic, unsigned short nIndex = 0);

	void removeElement(unsigned short nElementIndex);

	void removeAllElements(void);

	 /** 
	  *	Gets the D3D9-specific vertex declaration. 
	  */
	LPDIRECT3DVERTEXDECLARATION9 getD3DVertexDeclaration(void);
	
};

_PEPE_ENGINE_END

#endif