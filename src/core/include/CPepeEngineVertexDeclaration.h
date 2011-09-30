/**
* Project:	PepeEngine
* Tier:		Middleware
* File:		CPepeEngineVertexDeclaration.h   
*
* @brief	Declaration of CPepeEngineVertexDeclaration class.
*
* @author		Piotr 'pepe' Picheta
* @date			2008-07-10
* @copyright	Copyright (c) 2008 Piotr Picheta
*
* @version 1.0
*/

#ifndef CPEPEENGINEVERTEXDECLARATION_H
#define CPEPEENGINEVERTEXDECLARATION_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

/** 
 *	Vertex element semantics, used to identify the meaning of vertex buffer contents
 */
enum VertexElementSemantic 
{	
	VES_POSITION			= 1,	/**< Position, 3 reals per vertex		*/	
	VES_BLEND_WEIGHTS		= 2,	/**< Blending weights					*/	
	VES_BLEND_INDICES		= 3,	/**< Blending indices					*/	
	VES_NORMAL				= 4,	/**< Normal, 3 reals per vertex			*/	
	VES_DIFFUSE				= 5,	/**< Diffuse colours					*/	
	VES_SPECULAR			= 6,	/**< Specular colours					*/	
	VES_TEXTURE_COORDINATES	= 7,	/**< Texture coordinates				*/	
	VES_BINORMAL			= 8,	/**< Binormal (Y axis if normal is Z)	*/	
	VES_TANGENT				= 9		/**< Tangent (X axis if normal is Z)	*/
};

/**
 *	Vertex element type, used to identify the base types of the vertex contents
 */
enum VertexElementType
{
	VET_FLOAT1		= 0,
	VET_FLOAT2		= 1,
	VET_FLOAT3		= 2,
	VET_FLOAT4		= 3,	
	VET_COLOUR		= 4,
	VET_SHORT1		= 5,
	VET_SHORT2		= 6,
	VET_SHORT3		= 7,
	VET_SHORT4		= 8,
	VET_UBYTE4		= 9,	
	VET_COLOUR_ARGB = 10,	
	VET_COLOUR_ABGR = 11
};

class _PepeEngineExport CPepeEngineVertexElement
{
protected:
	
	/** The source vertex buffer, as bound to an index using VertexBufferBinding */
	unsigned short m_nSource;

	/** The offset in the buffer that this element starts at */
	size_t m_nOffset;

	/** The type of element */
	VertexElementType m_type;

	/** The meaning of the element */
	VertexElementSemantic m_semantic;

	/** Index of the item, only applicable for some elements like texture coords */
	unsigned short m_nIndex;
public:

	/**
	 * Constructor, should not be called directly, call CPepeEngineVertexDeclaration::addElement
	 */
	CPepeEngineVertexElement(
		unsigned short nSource, size_t nOffset, VertexElementType theType,
		VertexElementSemantic semantic, unsigned short nIndex = 0
		) : m_nSource(nSource), m_nOffset(nOffset), m_type(theType), m_semantic(semantic), m_nIndex(nIndex)
	{}

	unsigned short getSource() const {return m_nSource;}
	size_t getOffset() const {return m_nOffset;}
	VertexElementType getType() const {return m_type;}
	VertexElementSemantic getSemantic() const {return m_semantic;}
	unsigned short getIndex() const {return m_nIndex;}
	size_t getSize(void) const {return CPepeEngineVertexElement::getTypeSize(m_type);}

	static VertexElementType getBestColourVertexElementType();

	static size_t getTypeSize(VertexElementType type);

	inline bool operator==(const CPepeEngineVertexElement& rhs) const
	{
		if (m_type != rhs.m_type ||
			m_nIndex != rhs.m_nIndex ||
			m_nOffset != rhs.m_nOffset ||
			m_semantic != rhs.m_semantic ||
			m_nSource != rhs.m_nSource)
		{
			return false;
		} else
		{
			return true;
		}
	}
};

class _PepeEngineExport CPepeEngineVertexDeclaration
{	
	typedef std::list<CPepeEngineVertexElement> VertexElementList;

	static bool vertexElementLess(const CPepeEngineVertexElement& e1, const CPepeEngineVertexElement& e2);
protected:
	VertexElementList m_elementList;
public:	
	CPepeEngineVertexDeclaration();
	virtual ~CPepeEngineVertexDeclaration();
	
	void sort() {m_elementList.sort(CPepeEngineVertexDeclaration::vertexElementLess);}

	size_t getElementCount(void) {return m_elementList.size();}

	/** 
	 *	Gets read-only access to the list of vertex elements. 
	 */
	const VertexElementList& getElements(void) const {return m_elementList;}

	/**
	 *	Get a single element. 
	 */
	const CPepeEngineVertexElement* getElement(unsigned short nIndex);
	const CPepeEngineVertexElement* getElementBySemantic(const VertexElementSemantic& semantic) const;

	/** 
	 *	Gets the indeex of the highest source value referenced by this declaration. 
	 */
	unsigned short getMaxSource(void) const;

	virtual const CPepeEngineVertexElement& addElement(
		unsigned short nSource, size_t nOffset, VertexElementType theType,
		VertexElementSemantic semantic, unsigned short nIndex = 0);
	
	virtual const CPepeEngineVertexElement& insertElement(
		unsigned short nAtPosition, unsigned short nSource, size_t nOffset, VertexElementType theType,
		VertexElementSemantic semantic, unsigned short nIndex = 0);

	/** 
	 *	Remove the element at the given index from this declaration. 
	 */
	virtual void removeElement(unsigned short nElementIndex);

	/** 
	 *	Remove all elements. 
	 */
	virtual void removeAllElements(void);

	/** 
	 *	Gets the vertex size defined by this declaration for a given source. 
	 */
	virtual size_t getVertexSize(unsigned short nSource);

	inline bool operator==(const CPepeEngineVertexDeclaration& rhs) const
	{
		if (m_elementList.size() != rhs.m_elementList.size())
		{
			return false;
		}

		VertexElementList::const_iterator i, iend, rhsi, rhsiend;

		iend	= m_elementList.end();
		rhsiend = rhs.m_elementList.end();
		rhsi	= rhs.m_elementList.begin();

		for (i = m_elementList.begin(); i != iend && rhsi != rhsiend; ++i, ++rhsi)
		{
			if (!(*i == *rhsi))
			{
				return false;
			}
		}

		return true;
	}

	inline bool operator!=(const CPepeEngineVertexDeclaration& rhs) const
	{
		return !(*this == rhs);
	}
};

_PEPE_ENGINE_END

#endif