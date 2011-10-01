#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineException.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
const tstring& CPepeEngineException::getFullDescription() const
{
    if (m_strFullDescription.empty()) {
        tstringstream desc;

        desc    << _T("PEPE ENGINE EXCEPTION(") << m_iError << _T(":") << m_strType << _T("): ")
                << m_strDescription
                << _T(" in ") << m_strSource;

        if (m_lLine > 0) {
            desc << _T(" at ") << m_strFile << _T(" (line ") << m_lLine << _T(")");
        }

        m_strFullDescription = desc.str();
    }

    return m_strFullDescription;
}

_PEPE_ENGINE_END