#include "CPepeEngineDirect3D9Mapper.h"
#include "CPepeEngineDirect3D9VertexDeclaration.h"
#include "CPepeEngineException.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineDirect3D9VertexDeclaration::CPepeEngineDirect3D9VertexDeclaration(LPDIRECT3DDEVICE9 pDevice) : m_bNeedRebuild(false)
{
    m_pDevice   = pDevice;
    m_lpD3DDecl = NULL;
}

// -----------------------------------------------------------------------------------------
CPepeEngineDirect3D9VertexDeclaration::~CPepeEngineDirect3D9VertexDeclaration()
{
    SAFE_RELEASE(m_lpD3DDecl);
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVertexElement& CPepeEngineDirect3D9VertexDeclaration::addElement(
    unsigned short nSource, size_t nOffset, VertexElementType theType,
    VertexElementSemantic semantic, unsigned short nIndex)
{
    m_bNeedRebuild = true;
    return CPepeEngineVertexDeclaration::addElement(nSource, nOffset, theType, semantic, nIndex);
}

// -----------------------------------------------------------------------------------------
const CPepeEngineVertexElement& CPepeEngineDirect3D9VertexDeclaration::insertElement(
    unsigned short nAtPosition, unsigned short nSource, size_t nOffset, VertexElementType theType,
    VertexElementSemantic semantic, unsigned short nIndex)
{
    m_bNeedRebuild = true;
    return CPepeEngineVertexDeclaration::insertElement(nAtPosition, nSource, nOffset, theType, semantic, nIndex);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9VertexDeclaration::removeElement(unsigned short nElementIndex)
{
    m_bNeedRebuild = true;
    CPepeEngineVertexDeclaration::removeElement(nElementIndex);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineDirect3D9VertexDeclaration::removeAllElements(void)
{
    m_bNeedRebuild = true;
    CPepeEngineVertexDeclaration::removeAllElements();
}

// -----------------------------------------------------------------------------------------
LPDIRECT3DVERTEXDECLARATION9 CPepeEngineDirect3D9VertexDeclaration::getD3DVertexDeclaration(void)
{
    if (m_bNeedRebuild) {
        SAFE_RELEASE(m_lpD3DDecl);

        // Create D3D elements
        D3DVERTEXELEMENT9* d3delems = new D3DVERTEXELEMENT9[m_elementList.size() + 1];
        ZeroMemory(d3delems, sizeof(d3delems));

        VertexElementList::const_iterator i, iend;
        unsigned int idx;
        iend = m_elementList.end();

        for (idx = 0, i = m_elementList.begin(); i != iend; ++i, ++idx) {
            d3delems[idx].Method    = D3DDECLMETHOD_DEFAULT;
            d3delems[idx].Offset    = static_cast<unsigned short>(i->getOffset());
            d3delems[idx].Stream    = i->getSource();
            d3delems[idx].Type      = CPepeEngineDirect3D9Mapper::get(i->getType());
            d3delems[idx].Usage     = CPepeEngineDirect3D9Mapper::get(i->getSemantic());

            /**
             *  NB force index if colours since D3D uses the same usage for diffuse & specular
             */
            if (i->getSemantic() == VES_SPECULAR) {
                d3delems[idx].UsageIndex = 1;
            } else if (i->getSemantic() == VES_DIFFUSE) {
                d3delems[idx].UsageIndex = 0;
            } else {
                d3delems[idx].UsageIndex = (BYTE)i->getIndex();
            }
        }

        // Add terminator
        d3delems[idx].Stream        = 0xff;
        d3delems[idx].Offset        = 0;
        d3delems[idx].Type          = D3DDECLTYPE_UNUSED;
        d3delems[idx].Method        = 0;
        d3delems[idx].Usage         = 0;
        d3delems[idx].UsageIndex    = 0;

        HRESULT hr = m_pDevice->CreateVertexDeclaration(d3delems, &m_lpD3DDecl);

        if (FAILED(hr)) {
            tstring msg = DXGetErrorDescription9(hr);

            PEPE_ENGINE_EXCEPT(
                CPepeEngineException::ERR_RENDERINGAPI_ERROR,
                _T("Cannot create D3D9 vertex declaration: ") + msg,
                _T("CPepeEngineDirect3D9VertexDeclaration::getD3DVertexDeclaration")
            );
        }

        delete[] d3delems;

        m_bNeedRebuild = false;

    }

    return m_lpD3DDecl;
}

_PEPE_ENGINE_END