#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineColor.h"
#include "CPepeEngineVertexDeclaration.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
VertexElementType CPepeEngineVertexElement::getBestColourVertexElementType()
{
    return CPepeEngineCore::getSingleton().getRenderer()->getColourVertexElementType();
}

// -----------------------------------------------------------------------------------------
size_t CPepeEngineVertexElement::getTypeSize(VertexElementType type)
{
    switch(type) {
    case VET_COLOUR:
    case VET_COLOUR_ABGR:
    case VET_COLOUR_ARGB:
        return sizeof(RGBA);

    case VET_FLOAT1:
        return sizeof(float);

    case VET_FLOAT2:
        return sizeof(float) * 2;

    case VET_FLOAT3:
        return sizeof(float) * 3;

    case VET_FLOAT4:
        return sizeof(float) * 4;

    case VET_SHORT1:
        return sizeof(short);

    case VET_SHORT2:
        return sizeof(short) * 2;

    case VET_SHORT3:
        return sizeof(short) * 3;

    case VET_SHORT4:
        return sizeof(short) * 4;

    case VET_UBYTE4:
        return sizeof(unsigned char) * 4;
    }

    return 0;
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineVertexDeclaration::vertexElementLess(
    const CPepeEngineVertexElement& e1, const CPepeEngineVertexElement& e2
)
{
    // Sort by source first
    if (e1.getSource() < e2.getSource()) {
        return true;
    } else if (e1.getSource() == e2.getSource()) {
        // Use ordering of semantics to sort
        if (e1.getSemantic() < e2.getSemantic()) {
            return true;
        } else if (e1.getSemantic() == e2.getSemantic()) {
            // Use index to sort
            if (e1.getIndex() < e2.getIndex()) {
                return true;
            }
        }
    }

    return false;
}

// -----------------------------------------------------------------------------------------
CPepeEngineVertexDeclaration::CPepeEngineVertexDeclaration()
{
}

// -----------------------------------------------------------------------------------------
CPepeEngineVertexDeclaration::~CPepeEngineVertexDeclaration()
{
    removeAllElements();
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVertexElement* CPepeEngineVertexDeclaration::getElement(unsigned short nIndex)
{
    assert(nIndex < m_elementList.size() && "Index out of bounds");

    VertexElementList::iterator i = m_elementList.begin();

    for (unsigned short n = 0; n < nIndex; ++n) {
        ++i;
    }

    return &(*i);
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVertexElement* CPepeEngineVertexDeclaration::getElementBySemantic(
    const VertexElementSemantic& semantic) const
{
    VertexElementList::const_iterator i = m_elementList.begin();

    for (unsigned short n = 0; n < m_elementList.size(); ++n) {
        if (i->getSemantic() == semantic) {
            return &(*i);
        }
    }

    return 0;
}

// -----------------------------------------------------------------------------------------
unsigned short CPepeEngineVertexDeclaration::getMaxSource(void) const
{
    VertexElementList::const_iterator i, iend;
    iend                = m_elementList.end();
    unsigned short ret  = 0;

    for (i = m_elementList.begin(); i != iend; ++i) {
        if (i->getSource() > ret) {
            ret = i->getSource();
        }

    }

    return ret;
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVertexElement& CPepeEngineVertexDeclaration::addElement(
    unsigned short nSource, size_t nOffset, VertexElementType theType,
    VertexElementSemantic semantic, unsigned short nIndex)
{
    // Refine colour type to a specific type
    if (theType == VET_COLOUR) {
        theType = CPepeEngineVertexElement::getBestColourVertexElementType();
    }

    m_elementList.push_back(CPepeEngineVertexElement(nSource, nOffset, theType, semantic, nIndex));

    return m_elementList.back();
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVertexElement& CPepeEngineVertexDeclaration::insertElement(
    unsigned short nAtPosition, unsigned short nSource, size_t nOffset, VertexElementType theType,
    VertexElementSemantic semantic, unsigned short nIndex)
{
    if (nAtPosition >= m_elementList.size()) {
        return addElement(nSource, nOffset, theType, semantic, nIndex);
    }

    VertexElementList::iterator i = m_elementList.begin();

    for (unsigned short n = 0; n < nAtPosition; ++n) {
        ++i;
    }

    i = m_elementList.insert(i, CPepeEngineVertexElement(nSource, nOffset, theType, semantic, nIndex));
    return *i;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineVertexDeclaration::removeElement(unsigned short nElementIndex)
{
    assert(nElementIndex < m_elementList.size() && "Index out of bounds");

    VertexElementList::iterator i = m_elementList.begin();

    for (unsigned short n = 0; n < nElementIndex; ++n) {
        ++i;
    }

    m_elementList.erase(i);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineVertexDeclaration::removeAllElements(void)
{
    m_elementList.clear();
}

// -----------------------------------------------------------------------------------------
size_t CPepeEngineVertexDeclaration::getVertexSize(unsigned short nSource)
{
    VertexElementList::const_iterator i, iend;
    size_t sz   = 0;
    iend        = m_elementList.end();

    for (i = m_elementList.begin(); i != iend; ++i) {
        if (i->getSource() == nSource) {
            sz += i->getSize();

        }
    }

    return sz;
}

_PEPE_ENGINE_END