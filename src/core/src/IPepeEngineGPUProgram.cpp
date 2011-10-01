#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineAutoParamDataSource.h"
#include "IPepeEngineGPUProgram.h"

_PEPE_ENGINE_START

CPepeEngineGPUProgramParameters::AutoConstantDefinition CPepeEngineGPUProgramParameters::m_autoConstantDictionary[] = {
    AutoConstantDefinition(ACT_WORLD_MATRIX,                    _T("world_matrix"),                 16,     ET_FLOAT,   ACDT_NONE),
    AutoConstantDefinition(ACT_VIEW_MATRIX,                     _T("view_matrix"),                  16,     ET_FLOAT,   ACDT_NONE),
    AutoConstantDefinition(ACT_PROJECTION_MATRIX,               _T("projection_matrix"),            16,     ET_FLOAT,   ACDT_NONE),
    AutoConstantDefinition(ACT_WORLD_VIEW_PROJECTION_MATRIX,    _T("world_view_projection_matrix"), 16,     ET_FLOAT,   ACDT_NONE),
    AutoConstantDefinition(ACT_VIEW_PROJECTION_MATRIX,          _T("view_projection_matrix"),       16,     ET_FLOAT,   ACDT_NONE),
    AutoConstantDefinition(ACT_WORLD_VIEW_MATRIX,               _T("world_view_matrix"),            16,     ET_FLOAT,   ACDT_NONE),
    AutoConstantDefinition(ACT_MATERIAL_AMBIENT_COLOUR,         _T("material_ambient_color"),       4,      ET_FLOAT,   ACDT_NONE),
    AutoConstantDefinition(ACT_MATERIAL_DIFFUSE_COLOUR,         _T("material_diffuse_color"),       4,      ET_FLOAT,   ACDT_NONE),
    AutoConstantDefinition(ACT_MATERIAL_SPECULAR_COLOUR,        _T("material_specular_color"),      4,      ET_FLOAT,   ACDT_NONE),
    AutoConstantDefinition(ACT_MATERIAL_EMISSIVE_COLOUR,        _T("material_emissive_color"),      4,      ET_FLOAT,   ACDT_NONE),
    AutoConstantDefinition(ACT_MATERIAL_POWER,                  _T("material_power"),               1,      ET_FLOAT,   ACDT_NONE),
    AutoConstantDefinition(ACT_AMBIENT_LIGHT_COLOUR,            _T("ambient_light_color"),          4,      ET_FLOAT,   ACDT_NONE),
    AutoConstantDefinition(ACT_LIGHT_DIFFUSE_COLOUR,            _T("light_diffuse_color"),          4,      ET_FLOAT,   ACDT_INT),
    AutoConstantDefinition(ACT_LIGHT_SPECULAR_COLOUR,           _T("light_specular_color"),         4,      ET_FLOAT,   ACDT_INT),
    AutoConstantDefinition(ACT_LIGHT_ATTENUATION,               _T("light_attenaution"),            4,      ET_FLOAT,   ACDT_INT),
    AutoConstantDefinition(ACT_SPOTLIGHT_PARAMS,                _T("spotlight_params"),             4,      ET_FLOAT,   ACDT_INT),
    AutoConstantDefinition(ACT_LIGHT_POSITION,                  _T("light_position"),               4,      ET_FLOAT,   ACDT_INT),
    AutoConstantDefinition(ACT_LIGHT_DIRECTION,                 _T("light_direction"),              4,      ET_FLOAT,   ACDT_INT),

    AutoConstantDefinition(ACT_LIGHT_DIFFUSE_COLOUR_ARRAY,      _T("light_diffuse_color_array"),    4,      ET_FLOAT,   ACDT_INT),
    AutoConstantDefinition(ACT_LIGHT_SPECULAR_COLOUR_ARRAY,     _T("light_specular_color_array"),   4,      ET_FLOAT,   ACDT_INT),
    AutoConstantDefinition(ACT_LIGHT_ATTENUATION_ARRAY,         _T("light_attenaution_array"),      4,      ET_FLOAT,   ACDT_INT),
    AutoConstantDefinition(ACT_SPOTLIGHT_PARAMS_ARRAY,          _T("spotlight_params_array"),       4,      ET_FLOAT,   ACDT_INT),
    AutoConstantDefinition(ACT_LIGHT_POSITION_ARRAY,            _T("light_position_array"),         4,      ET_FLOAT,   ACDT_INT),
    AutoConstantDefinition(ACT_LIGHT_DIRECTION_ARRAY,           _T("light_direction_array"),        4,      ET_FLOAT,   ACDT_INT),

    AutoConstantDefinition(ACT_CAMERA_POSITION,                 _T("camera_position"),              4,      ET_FLOAT,   ACDT_NONE),

    AutoConstantDefinition(ACT_TIME,                            _T("time"),                         1,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_TIME_0_X,                        _T("time_0_x"),                     4,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_COSTIME_0_X,                     _T("costime_0_x"),                  4,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_SINTIME_0_X,                     _T("sintime_0_x"),                  4,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_TANTIME_0_X,                     _T("tantime_0_x"),                  4,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_TIME_0_X_PACKED,                 _T("time_0_x_packed"),              4,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_TIME_0_1,                        _T("time_0_1"),                     4,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_COSTIME_0_1,                     _T("costime_0_1"),                  4,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_SINTIME_0_1,                     _T("sintime_0_1"),                  4,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_TANTIME_0_1,                     _T("tantime_0_1"),                  4,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_TIME_0_1_PACKED,                 _T("time_0_1_packed"),              4,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_TIME_0_2PI,                      _T("time_0_2pi"),                   4,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_COSTIME_0_2PI,                   _T("costime_0_2pi"),                4,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_SINTIME_0_2PI,                   _T("sintime_0_2pi"),                4,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_TANTIME_0_2PI,                   _T("tantime_0_2pi"),                4,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_TIME_0_2PI_PACKED,               _T("time_0_2pi_packed"),            4,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_FRAME_TIME,                      _T("frame_time"),                   1,      ET_FLOAT,   ACDT_FLOAT),
    AutoConstantDefinition(ACT_FPS,                             _T("fps"),                          1,      ET_FLOAT,   ACDT_NONE)
};

// -----------------------------------------------------------------------------------------
void GpuNamedConstants::generateConstantDefinitionArrayEntries(
    const tstring& strParamName,
    const GpuConstantDefinition& baseDef
)
{
    // Copy definition for use with arrays
    GpuConstantDefinition   arrayDef    = baseDef;
    tstring                 arrayName;

    arrayDef.m_nArraySize = 1;

    /*
        Add parameters for array accessors
        [0] will refer to the same location, [1+] will increment
        only populate others individually up to 16 array slots so as not to get out of hand
        paramName[0] version will always exist
    */

    size_t maxArrayIndex = 1;

    if (baseDef.m_nArraySize <= 16) {
        maxArrayIndex = baseDef.m_nArraySize;
    }

    for (size_t i = 0; i < maxArrayIndex; i++) {
        arrayName = strParamName + _T("[") + _TSTR(i) + _T("]");
        map.insert(GpuConstantDefinitionMap::value_type(arrayName, arrayDef));

        arrayDef.m_nPhysicalIndex += arrayDef.m_nElementSize;
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::writeRawConstants(size_t nPhysicalIndex, const float* pfVal, size_t nCount)
{
    assert(nPhysicalIndex + nCount <= m_floatConstantList.size());
    memcpy(&m_floatConstantList[nPhysicalIndex], pfVal, sizeof(float) * nCount);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::writeRawConstants(size_t nPhysicalIndex, const int* piVal, size_t nCount)
{
    assert(nPhysicalIndex + nCount <= m_floatConstantList.size());
    memcpy(&m_intConstantList[nPhysicalIndex], piVal, sizeof(int) * nCount);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::writeRawConstant(size_t nPhysicalIndex, const CPepeEngineMatrix4& m)
{
    if (m_bTransposeMatrices) {
        CPepeEngineMatrix4 t = m.transpose();
        writeRawConstants(nPhysicalIndex, t[0], 16);
    } else {
        writeRawConstants(nPhysicalIndex, m[0], 16);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::writeRawConstant(
    size_t nPhysicalIndex, const CPepeEngineColor& color, size_t nCount)
{
    // write either the number requested (for packed types) or up to 4
    writeRawConstants(nPhysicalIndex, color.ptr(), std::min(nCount, (size_t)4));
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::writeRawConstant(
    size_t nPhysicalIndex, const CPepeEngineVector4& vec, size_t nCount)
{
    /*
        remember, raw content access uses raw float count rather than float4
        write either the number requested (for packed types) or up to 4
    */
    writeRawConstants(nPhysicalIndex, vec.ptr(), std::min(nCount, (size_t)4));
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::writeRawConstant(size_t nPhysicalIndex, float fVal)
{
    writeRawConstants(nPhysicalIndex, &fVal, 1);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::writeRawConstant(size_t nPhysicalIndex, int nVal)
{
    writeRawConstants(nPhysicalIndex, &nVal, 1);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::writeRawConstant(size_t nPhysicalIndex, const CPepeEngineVector3& vec)
{
    writeRawConstants(nPhysicalIndex, vec.ptr(), 3);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::writeRawConstant(
    size_t physicalIndex, const CPepeEngineMatrix4* pMatrix, size_t numEntries
)
{
    if (m_bTransposeMatrices) {
        for (size_t i = 0; i < numEntries; ++i) {
            CPepeEngineMatrix4 t = pMatrix[i].transpose();
            writeRawConstants(physicalIndex, t[0], 16);
            physicalIndex += 16;
        }
    } else {
        writeRawConstants(physicalIndex, pMatrix[0][0], 16 * numEntries);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setRawAutoConstant(
    size_t nPhysicalIndex, AutoConstantType acType, size_t nExtraInfo, size_t nElementSize)
{
    // update existing index if it exists
    bool found = false;

    for (AutoConstantList::iterator i = m_autoConstantList.begin(); i != m_autoConstantList.end(); ++i) {
        if (i->m_nPhysicalIndex == nPhysicalIndex) {
            i->m_paramType      = acType;
            i->nData            = nExtraInfo;
            i->m_nElementCount  = nElementSize;

            found = true;

            break;
        }
    }

    if (!found) {
        m_autoConstantList.push_back(AutoConstantEntry(acType, nPhysicalIndex, nExtraInfo, nElementSize));
    }
}

// -----------------------------------------------------------------------------------------
void  CPepeEngineGPUProgramParameters::setRawAutoConstantFloat(
    size_t nPhysicalIndex, AutoConstantType acType, float fExtraInfo, size_t nElementSize)
{
    // update existing index if it exists
    bool found = false;

    for (AutoConstantList::iterator i = m_autoConstantList.begin(); i != m_autoConstantList.end(); ++i) {
        if (i->m_nPhysicalIndex == nPhysicalIndex) {
            i->m_paramType      = acType;
            i->fData            = fExtraInfo;
            i->m_nElementCount  = nElementSize;

            found = true;

            break;
        }
    }

    if (!found) {
        m_autoConstantList.push_back(AutoConstantEntry(acType, nPhysicalIndex, fExtraInfo, nElementSize));
    }
}

// -----------------------------------------------------------------------------------------
size_t CPepeEngineGPUProgramParameters::getFloatConstantPhysicalIndex(size_t nLogicalIndex, size_t nRequestedSize)
{
    if (m_pFloatLogicalToPhysical == NULL) {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_INVALID_PARAMETERS,
            _T("This is not a low-level parameter parameter object"),
            _T("CPepeEngineGPUProgramParameters::getFloatConstantPhysicalIndex")
        );
    }

    size_t physicalIndex;

    GpuLogicalIndexUseMap::iterator logi = m_pFloatLogicalToPhysical->map.find(nLogicalIndex);

    if (logi == m_pFloatLogicalToPhysical->map.end()) {
        if (nRequestedSize) {
            physicalIndex = m_floatConstantList.size();

            // Expand at buffer end
            m_floatConstantList.insert(m_floatConstantList.end(), nRequestedSize, 0.0f);

            // Record extended size for future GPU params re-using this information
            m_pFloatLogicalToPhysical->m_nBufferSize = m_floatConstantList.size();

            /*
                Low-level programs will not know about mapping ahead of time, so
                populate it. Other params objects will be able to just use this
                accepted mapping since the constant structure will be the same
            */

            // Set up a mapping for all items in the count
            size_t currPhys = physicalIndex;
            size_t count    = nRequestedSize / 4;

            for (size_t logicalNum = 0; logicalNum < count; ++logicalNum) {
                m_pFloatLogicalToPhysical->map.insert(
                    GpuLogicalIndexUseMap::value_type(
                        nLogicalIndex + logicalNum,
                        GpuLogicalIndexUse(currPhys, nRequestedSize)
                    )
                );
                currPhys += 4;
            }
        } else {
            // no match & ignore
            return std::numeric_limits<size_t>::max();
        }

    } else {
        physicalIndex = logi->second.m_nPhysicalIndex;

        // check size
        if (logi->second.m_nCurrentSize < nRequestedSize) {
            /*
                Init buffer entry wasn't big enough; could be a mistake on the part
                of the original use, or perhaps a variable length we can't predict
                until first actual runtime use
            */
            size_t                      insertCount = nRequestedSize - logi->second.m_nCurrentSize;
            FloatConstantList::iterator insertPos   = m_floatConstantList.begin();
            std::advance(insertPos, physicalIndex);
            m_floatConstantList.insert(insertPos, insertCount, 0.0f);

            // shift all physical positions after this one
            for (GpuLogicalIndexUseMap::iterator i = m_pFloatLogicalToPhysical->map.begin();
                 i != m_pFloatLogicalToPhysical->map.end(); ++i) {
                if (i->second.m_nPhysicalIndex > physicalIndex) {
                    i->second.m_nPhysicalIndex += insertCount;
                }
            }

            for (AutoConstantList::iterator i = m_autoConstantList.begin(); i != m_autoConstantList.end(); ++i) {
                if (i->m_nPhysicalIndex > physicalIndex) {
                    i->m_nPhysicalIndex += insertCount;
                }
            }
        }
    }

    return physicalIndex;
}

// -----------------------------------------------------------------------------------------
size_t CPepeEngineGPUProgramParameters::getIntConstantPhysicalIndex(size_t nLogicalIndex, size_t nRequestedSize)
{
    if (!m_pIntLogicalToPhysical) {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_INVALID_PARAMETERS,
            _T("This is not a low-level parameter parameter object"),
            _T("CPepeEngineGPUProgramParameters::getIntConstantPhysicalIndex")
        );
    }

    size_t physicalIndex;

    GpuLogicalIndexUseMap::iterator logi = m_pIntLogicalToPhysical->map.find(nLogicalIndex);

    if (logi == m_pIntLogicalToPhysical->map.end()) {
        if (nRequestedSize) {
            physicalIndex = m_intConstantList.size();

            // Expand at buffer end
            m_intConstantList.insert(m_intConstantList.end(), nRequestedSize, 0);

            // Record extended size for future GPU params re-using this information
            m_pIntLogicalToPhysical->m_nBufferSize = m_intConstantList.size();

            /*
                Low-level programs will not know about mapping ahead of time, so
                populate it. Other params objects will be able to just use this
                accepted mapping since the constant structure will be the same
            */

            // Set up a mapping for all items in the count
            size_t currPhys = physicalIndex;
            size_t count    = nRequestedSize / 4;

            for (size_t logicalNum = 0; logicalNum < count; ++logicalNum) {
                m_pIntLogicalToPhysical->map.insert(
                    GpuLogicalIndexUseMap::value_type(
                        nLogicalIndex + logicalNum,
                        GpuLogicalIndexUse(currPhys, nRequestedSize)
                    )
                );
                currPhys += 4;
            }
        } else {
            // no match & ignore
            return std::numeric_limits<size_t>::max();
        }

    } else {
        physicalIndex = logi->second.m_nPhysicalIndex;

        // check size
        if (logi->second.m_nCurrentSize < nRequestedSize) {
            /*
                Init buffer entry wasn't big enough; could be a mistake on the part
                of the original use, or perhaps a variable length we can't predict
                until first actual runtime use
            */
            size_t                      insertCount = nRequestedSize - logi->second.m_nCurrentSize;
            IntConstantList::iterator   insertPos   = m_intConstantList.begin();
            std::advance(insertPos, physicalIndex);
            m_intConstantList.insert(insertPos, insertCount, 0);

            // shift all physical positions after this one
            for (GpuLogicalIndexUseMap::iterator i = m_pIntLogicalToPhysical->map.begin();
                 i != m_pIntLogicalToPhysical->map.end(); ++i) {
                if (i->second.m_nPhysicalIndex > physicalIndex) {
                    i->second.m_nPhysicalIndex += insertCount;
                }
            }

            for (AutoConstantList::iterator i = m_autoConstantList.begin(); i != m_autoConstantList.end(); ++i) {
                if (i->m_nPhysicalIndex > physicalIndex) {
                    i->m_nPhysicalIndex += insertCount;
                }
            }
        }
    }

    return physicalIndex;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setNamedConstants(const GpuNamedConstants* pNamedConstants)
{
    m_pNamedConstants = pNamedConstants;

    /*
        Determine any extension to local buffers
        Size and reset buffer (fill with zero to make comparison later ok)
    */
    if (pNamedConstants->m_nFloatBufferSize > m_floatConstantList.size()) {
        m_floatConstantList.insert(m_floatConstantList.end(),
                                   pNamedConstants->m_nFloatBufferSize - m_floatConstantList.size(), 0.0f);
    }

    if (pNamedConstants->m_nIntBufferSize > m_intConstantList.size()) {
        m_intConstantList.insert(m_intConstantList.end(),
                                 pNamedConstants->m_nIntBufferSize - m_intConstantList.size(), 0);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setLogicalIndexes(
    GpuLogicalBufferStruct* pFloatIndexMap,
    GpuLogicalBufferStruct* pIntIndexMap)
{
    m_pFloatLogicalToPhysical   = pFloatIndexMap;
    m_pIntLogicalToPhysical     = pIntIndexMap;

    /*
        Resize the internal buffers.
        Note that these will only contain something after the first parameter
        set has set some parameters
    */

    // Size and reset buffer (fill with zero to make comparison later ok)
    if (pFloatIndexMap->m_nBufferSize > m_floatConstantList.size()) {
        m_floatConstantList.insert(m_floatConstantList.end(),
                                   pFloatIndexMap->m_nBufferSize - m_floatConstantList.size(), 0.0f);
    }

    if (pIntIndexMap->m_nBufferSize > m_intConstantList.size()) {
        m_intConstantList.insert(m_intConstantList.end(),
                                 pIntIndexMap->m_nBufferSize - m_intConstantList.size(), 0);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setSamplers(GpuSamplerDefinitionMap* pSamplerList)
{
    m_pSamplerList = pSamplerList;
}

// -----------------------------------------------------------------------------------------
const size_t CPepeEngineGPUProgramParameters::getNumAutoConstantDefinitions(void) const
{
    return sizeof(m_autoConstantDictionary) / sizeof(AutoConstantDefinition);
}

// -----------------------------------------------------------------------------------------
const CPepeEngineGPUProgramParameters::AutoConstantDefinition*
CPepeEngineGPUProgramParameters::getAutoConstantDefinition(const tstring& strName) const
{
    bool bNameFound         = false;
    size_t i                = 0;
    const size_t    numDefs = getNumAutoConstantDefinitions();

    while (!bNameFound && (i < numDefs)) {
        if (strName == m_autoConstantDictionary[i].m_strName) {
            bNameFound = true;
        } else {
            ++i;
        }
    }

    if (bNameFound) {
        return &m_autoConstantDictionary[i];
    } else {
        return 0;
    }
}

// -----------------------------------------------------------------------------------------
const CPepeEngineGPUProgramParameters::AutoConstantDefinition*
CPepeEngineGPUProgramParameters::getAutoConstantDefinition(
    const CPepeEngineGPUProgramParameters::AutoConstantType& type
) const
{
    bool bNameFound         = false;
    size_t i                = 0;
    const size_t    numDefs = getNumAutoConstantDefinitions();

    while (!bNameFound && (i < numDefs)) {
        if (type == m_autoConstantDictionary[i].m_acType) {
            bNameFound = true;
        } else {
            ++i;
        }
    }

    if (bNameFound) {
        return &m_autoConstantDictionary[i];
    } else {
        return 0;
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setAutoConstant(size_t nIndex, AutoConstantType acType, size_t nExtraInfo)
{
    // Get auto constant definition for sizing
    const AutoConstantDefinition* autoDef = getAutoConstantDefinition(acType);

    // round up to nearest multiple of 4
    size_t sz = autoDef->m_nElementCount;

    if (sz % 4 > 0) {
        sz += 4 - (sz % 4);
    }

    size_t physicalIndex = getFloatConstantPhysicalIndex(nIndex, sz);

    setRawAutoConstant(physicalIndex, acType, nExtraInfo);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setAutoConstantFloat(size_t nIndex, AutoConstantType acType, float fExtraInfo)
{
    // Get auto constant definition for sizing
    const AutoConstantDefinition* autoDef = getAutoConstantDefinition(acType);

    // round up to nearest multiple of 4
    size_t sz = autoDef->m_nElementCount;

    if (sz % 4 > 0) {
        sz += 4 - (sz % 4);
    }

    size_t physicalIndex = getFloatConstantPhysicalIndex(nIndex, sz);

    setRawAutoConstantFloat(physicalIndex, acType, fExtraInfo);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setNamedConstant(const tstring& strName, float val)
{
    const GpuConstantDefinition* def = findNamedConstantDefinition(strName);

    if (def) {
        writeRawConstant(def->m_nPhysicalIndex, val);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setNamedConstant(const tstring& strName, int val)
{
    const GpuConstantDefinition* def = findNamedConstantDefinition(strName);

    if (def) {
        writeRawConstant(def->m_nPhysicalIndex, val);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setNamedConstant(const tstring& strName, const CPepeEngineVector4& vec)
{
    const GpuConstantDefinition* def = findNamedConstantDefinition(strName);

    if (def) {
        writeRawConstant(def->m_nPhysicalIndex, vec, def->m_nElementSize);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setNamedConstant(const tstring& strName, const CPepeEngineVector3& vec)
{
    const GpuConstantDefinition* def = findNamedConstantDefinition(strName);

    if (def) {
        writeRawConstant(def->m_nPhysicalIndex, vec);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setNamedConstant(const tstring& strName, const CPepeEngineMatrix4& m)
{
    const GpuConstantDefinition* def = findNamedConstantDefinition(strName);

    if (def) {
        writeRawConstant(def->m_nPhysicalIndex, m);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setNamedConstant(
    const tstring& strName, const CPepeEngineMatrix4* m, size_t numEntries
)
{
    const GpuConstantDefinition* def = findNamedConstantDefinition(strName);

    if (def) {
        writeRawConstant(def->m_nPhysicalIndex, m, numEntries);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setNamedConstant(
    const tstring& strName, const float *val, size_t count, size_t multiple)
{
    size_t rawCount = count * multiple;

    const GpuConstantDefinition* def = findNamedConstantDefinition(strName);

    if (def) {
        writeRawConstants(def->m_nPhysicalIndex, val, rawCount);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setNamedConstant(const tstring& strName, const CPepeEngineColor& colour)
{
    const GpuConstantDefinition* def = findNamedConstantDefinition(strName);

    if (def) {
        writeRawConstant(def->m_nPhysicalIndex, colour, def->m_nElementSize);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setNamedConstant(
    const tstring& strName, const int *val, size_t count, size_t multiple)
{
    size_t rawCount = count * multiple;

    const GpuConstantDefinition* def = findNamedConstantDefinition(strName);

    if (def) {
        writeRawConstants(def->m_nPhysicalIndex, val, rawCount);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setNamedAutoConstant(
    const tstring& strName, AutoConstantType acType, size_t nExtraInfo)
{
    const GpuConstantDefinition* def = findNamedConstantDefinition(strName);

    if (def) {
        setRawAutoConstant(def->m_nPhysicalIndex, acType, nExtraInfo, def->m_nElementSize);
    }

}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::setNamedAutoConstantFloat(
    const tstring& strName, AutoConstantType acType, float fData)
{
    const GpuConstantDefinition* def = findNamedConstantDefinition(strName);

    if (def) {
        setRawAutoConstantFloat(def->m_nPhysicalIndex, acType, fData, def->m_nElementSize);
    }

}

// -----------------------------------------------------------------------------------------
const GpuConstantDefinition* CPepeEngineGPUProgramParameters::findNamedConstantDefinition(
    const tstring& strName, bool bThrowException
)
{
    if (!m_pNamedConstants) {
        if (bThrowException) {
            PEPE_ENGINE_EXCEPT(
                CPepeEngineException::ERR_INVALID_PARAMETERS,
                _T("Named constants have not been initialised, perhaps a compile error."),
                _T("CPepeEngineGPUProgramParameters::findNamedConstantDefinition")
            );
        }

        return 0;
    }

    GpuConstantDefinitionMap::const_iterator i = m_pNamedConstants->map.find(strName);

    if (i == m_pNamedConstants->map.end()) {
        if (bThrowException) {
            PEPE_ENGINE_EXCEPT(
                CPepeEngineException::ERR_INVALID_PARAMETERS,
                _T("Parameter: ") + strName + _T(" not found."),
                _T("CPepeEngineGPUProgramParameters::findNamedConstantDefinition")
            );
        }

        return 0;
    } else {
        return &(i->second);
    }
}

// -----------------------------------------------------------------------------------------
GpuSamplerDefinition* CPepeEngineGPUProgramParameters::findSamplerDefinition(
    const tstring& strName, bool bThrowException
)
{
    if (!m_pSamplerList) {
        if (bThrowException) {
            PEPE_ENGINE_EXCEPT(
                CPepeEngineException::ERR_INVALID_PARAMETERS,
                _T("Samplers have not been initialised, perhaps a compile error."),
                _T("CPepeEngineGPUProgramParameters::findSamplerDefinition")
            );
        }

        return 0;
    }

    GpuSamplerDefinitionMap::iterator i = m_pSamplerList->find(strName);

    if (i == m_pSamplerList->end()) {
        if (bThrowException) {
            PEPE_ENGINE_EXCEPT(
                CPepeEngineException::ERR_INVALID_PARAMETERS,
                _T("Sampler: ") + strName + _T(" not found."),
                _T("CPepeEngineGPUProgramParameters::findSamplerDefinition")
            );
        }

        return 0;
    } else {
        return &(i->second);
    }
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineGPUProgramParameters::useModelTexture()
{
    GpuSamplerDefinition* def = findSamplerDefinition(_T("model_texture"), false);

    if (def) {
        return true;
    } else {
        return false;
    }
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineGPUProgramParameters::useSkyBox()
{
    GpuSamplerDefinition* def = findSamplerDefinition(_T("skyBox"), false);

    if (def) {
        return true;
    } else {
        return false;
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::bindTexture(const tstring& strName, TexturePtr pTexture)
{
    GpuSamplerDefinition* def = findSamplerDefinition(strName, true);

    def->m_pTexture = pTexture;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::updateAutoConstantParameters(const CPepeEngineAutoParamDataSource& source)
{
    if (!hasAutoConstantParameters()) {
        return;
    }

    AutoConstantList::const_iterator i, iend;
    iend = m_autoConstantList.end();

    for (i = m_autoConstantList.begin(); i != iend; ++i) {
        switch(i->m_paramType) {
        case ACT_WORLD_MATRIX:
            writeRawConstant(i->m_nPhysicalIndex, source.getWorldMatrix());
            break;

        case ACT_VIEW_MATRIX:
            writeRawConstant(i->m_nPhysicalIndex, source.getViewMatrix());
            break;

        case ACT_PROJECTION_MATRIX:
            writeRawConstant(i->m_nPhysicalIndex, source.getProjectionMatrix());
            break;

        case ACT_WORLD_VIEW_PROJECTION_MATRIX:
            writeRawConstant(i->m_nPhysicalIndex, source.getWorldViewProjectionMatrix());
            break;

        case ACT_VIEW_PROJECTION_MATRIX:
            writeRawConstant(i->m_nPhysicalIndex, source.getViewProjectionMatrix());
            break;

        case ACT_WORLD_VIEW_MATRIX:
            writeRawConstant(i->m_nPhysicalIndex, source.getWorldViewMatrix());
            break;

        case ACT_MATERIAL_AMBIENT_COLOUR:
            writeRawConstant(i->m_nPhysicalIndex, source.getMaterialAmbientColor(), i->m_nElementCount);
            break;

        case ACT_MATERIAL_DIFFUSE_COLOUR:
            writeRawConstant(i->m_nPhysicalIndex, source.getMaterialDiffuseColor(), i->m_nElementCount);
            break;

        case ACT_MATERIAL_SPECULAR_COLOUR:
            writeRawConstant(i->m_nPhysicalIndex, source.getMaterialSpecularColor(), i->m_nElementCount);
            break;

        case ACT_MATERIAL_EMISSIVE_COLOUR:
            writeRawConstant(i->m_nPhysicalIndex, source.getMaterialEmissiveColor(), i->m_nElementCount);
            break;

        case ACT_MATERIAL_POWER:
            writeRawConstant(i->m_nPhysicalIndex, source.getMaterialPower());
            break;

        case ACT_AMBIENT_LIGHT_COLOUR:
            writeRawConstant(i->m_nPhysicalIndex, source.getAmbientLightColor(), i->m_nElementCount);
            break;

        case ACT_CAMERA_POSITION: {
            CPepeEngineVector4 vec(source.getCamera()->getPosition());
            writeRawConstant(i->m_nPhysicalIndex, vec, i->m_nElementCount);
        }
        break;

        case ACT_TIME:
            writeRawConstant(i->m_nPhysicalIndex, source.getTime() * i->fData);
            break;

        case ACT_TIME_0_X:
            writeRawConstant(i->m_nPhysicalIndex, source.getTime_0_X(i->fData));
            break;

        case ACT_COSTIME_0_X:
            writeRawConstant(i->m_nPhysicalIndex, source.getCosTime_0_X(i->fData));
            break;

        case ACT_SINTIME_0_X:
            writeRawConstant(i->m_nPhysicalIndex, source.getSinTime_0_X(i->fData));
            break;

        case ACT_TANTIME_0_X:
            writeRawConstant(i->m_nPhysicalIndex, source.getTanTime_0_X(i->fData));
            break;

        case ACT_TIME_0_X_PACKED:
            writeRawConstant(i->m_nPhysicalIndex, source.getTime_0_X_packed(i->fData), i->m_nElementCount);
            break;

        case ACT_TIME_0_1:
            writeRawConstant(i->m_nPhysicalIndex, source.getTime_0_1(i->fData));
            break;

        case ACT_COSTIME_0_1:
            writeRawConstant(i->m_nPhysicalIndex, source.getCosTime_0_1(i->fData));
            break;

        case ACT_SINTIME_0_1:
            writeRawConstant(i->m_nPhysicalIndex, source.getSinTime_0_1(i->fData));
            break;

        case ACT_TANTIME_0_1:
            writeRawConstant(i->m_nPhysicalIndex, source.getTanTime_0_1(i->fData));
            break;

        case ACT_TIME_0_1_PACKED:
            writeRawConstant(i->m_nPhysicalIndex, source.getTime_0_1_packed(i->fData), i->m_nElementCount);
            break;

        case ACT_TIME_0_2PI:
            writeRawConstant(i->m_nPhysicalIndex, source.getTime_0_2Pi(i->fData));
            break;

        case ACT_COSTIME_0_2PI:
            writeRawConstant(i->m_nPhysicalIndex, source.getCosTime_0_2Pi(i->fData));
            break;

        case ACT_SINTIME_0_2PI:
            writeRawConstant(i->m_nPhysicalIndex, source.getSinTime_0_2Pi(i->fData));
            break;

        case ACT_TANTIME_0_2PI:
            writeRawConstant(i->m_nPhysicalIndex, source.getTanTime_0_2Pi(i->fData));
            break;

        case ACT_TIME_0_2PI_PACKED:
            writeRawConstant(i->m_nPhysicalIndex, source.getTime_0_2Pi_packed(i->fData), i->m_nElementCount);
            break;

        case ACT_FRAME_TIME:
            writeRawConstant(i->m_nPhysicalIndex, source.getFrameTime() * i->fData);
            break;

        case ACT_FPS:
            writeRawConstant(i->m_nPhysicalIndex, source.getFPS());
            break;
        }
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::updateAutoConstantParametersLights(const CPepeEngineAutoParamDataSource& source)
{
    if (!hasAutoConstantParameters()) {
        return;
    }

    AutoConstantList::const_iterator i, iend;
    iend = m_autoConstantList.end();

    for (i = m_autoConstantList.begin(); i != iend; ++i) {
        switch(i->m_paramType) {
        case ACT_LIGHT_DIFFUSE_COLOUR:
            writeRawConstant(i->m_nPhysicalIndex, source.getLight(i->nData)->getDiffuse(), i->m_nElementCount);
            break;

        case ACT_LIGHT_SPECULAR_COLOUR:
            writeRawConstant(i->m_nPhysicalIndex, source.getLight(i->nData)->getSpecular(), i->m_nElementCount);
            break;

        case ACT_LIGHT_ATTENUATION: {
            const CPepeEngineLight* l = source.getLight(i->nData);
            CPepeEngineVector4 vec(
                l->getRange(),
                l->getConstantAttenuation(),
                l->getLinearAttenuation(),
                l->getQuadricAttenuation()
            );
            writeRawConstant(i->m_nPhysicalIndex, vec, i->m_nElementCount);
        }
        break;

        case ACT_SPOTLIGHT_PARAMS: {
            const CPepeEngineLight* l = source.getLight(i->nData);

            if (l->getLightType() == LT_SPOTLIGHT) {
                CPepeEngineVector4 vec(
                    CPepeEngineMath::Cos(l->getTheta().valueRadians() * 0.5f),
                    CPepeEngineMath::Cos(l->getPhi().valueRadians() * 0.5f),
                    l->getFalloff(),
                    1.0f
                );
                writeRawConstant(i->m_nPhysicalIndex, vec, i->m_nElementCount);
            }
        }
        break;

        case ACT_LIGHT_POSITION: {
            CPepeEngineVector4 vec(source.getLight(i->nData)->getDerivedPosition());
            writeRawConstant(i->m_nPhysicalIndex, vec, i->m_nElementCount);
        }
        break;

        case ACT_LIGHT_DIRECTION: {
            CPepeEngineVector4 vec(source.getLight(i->nData)->getDerivedDirection());
            writeRawConstant(i->m_nPhysicalIndex, vec, i->m_nElementCount);
        }
        break;

        case ACT_LIGHT_DIFFUSE_COLOUR_ARRAY:
            for (size_t l = 0; l < i->nData; ++l) {
                writeRawConstant(i->m_nPhysicalIndex + l * i->m_nElementCount,
                                 source.getLight(l)->getDiffuse(), i->m_nElementCount);
            }

            break;

        case ACT_LIGHT_SPECULAR_COLOUR_ARRAY:
            for (size_t l = 0; l < i->nData; ++l) {
                writeRawConstant(i->m_nPhysicalIndex + l * i->m_nElementCount,
                                 source.getLight(l)->getSpecular(), i->m_nElementCount);
            }

            break;

        case ACT_LIGHT_ATTENUATION_ARRAY: {
            for (size_t l = 0; l < i->nData; ++l) {
                const CPepeEngineLight* lt = source.getLight(l);
                CPepeEngineVector4 vec(
                    lt->getRange(),
                    lt->getConstantAttenuation(),
                    lt->getLinearAttenuation(),
                    lt->getQuadricAttenuation()
                );
                writeRawConstant(i->m_nPhysicalIndex + l * i->m_nElementCount,
                                 vec, i->m_nElementCount);
            }
        }
        break;

        case ACT_SPOTLIGHT_PARAMS_ARRAY: {
            for (size_t l = 0; l < i->nData; ++l) {
                const CPepeEngineLight* lt = source.getLight(l);

                if (lt->getLightType() == LT_SPOTLIGHT) {
                    CPepeEngineVector4 vec(
                        CPepeEngineMath::Cos(lt->getTheta().valueRadians() * 0.5f),
                        CPepeEngineMath::Cos(lt->getPhi().valueRadians() * 0.5f),
                        lt->getFalloff(),
                        1.0f
                    );
                    writeRawConstant(i->m_nPhysicalIndex + l * i->m_nElementCount,
                                     vec, i->m_nElementCount);
                }
            }
        }
        break;

        case ACT_LIGHT_POSITION_ARRAY: {
            for (size_t l = 0; l < i->nData; ++l) {
                CPepeEngineVector4 vec(source.getLight(l)->getDerivedPosition());
                writeRawConstant(i->m_nPhysicalIndex + l * i->m_nElementCount,
                                 vec, i->m_nElementCount);
            }
        }
        break;

        case ACT_LIGHT_DIRECTION_ARRAY: {
            for (size_t l = 0; l < i->nData; ++l) {
                CPepeEngineVector4 vec(source.getLight(l)->getDerivedDirection());
                writeRawConstant(i->m_nPhysicalIndex + l * i->m_nElementCount,
                                 vec, i->m_nElementCount);
            }
        }
        break;

        }
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineGPUProgramParameters::generateAutoConstantList()
{
    if (m_pNamedConstants->map.empty()) {
        return;
    }

    for (GpuConstantDefinitionMap::const_iterator i = m_pNamedConstants->map.begin();
         i != m_pNamedConstants->map.end(); ++i) {
        const AutoConstantDefinition* autoConstantDef = getAutoConstantDefinition(i->first);

        if (autoConstantDef) {
            switch (autoConstantDef->m_dataType) {
            case ACDT_NONE:
                setNamedAutoConstant(i->first, autoConstantDef->m_acType, 0);
                break;

            case ACDT_INT:
                setNamedAutoConstant(i->first, autoConstantDef->m_acType, i->second.m_nArraySize);
                break;

            case ACDT_FLOAT:
                setNamedAutoConstantFloat(i->first, autoConstantDef->m_acType, 1.0f);
                break;
            }
        }
    }

}

// -----------------------------------------------------------------------------------------
void IPepeEngineGPUProgram::setProgramSamplers(const CPepeEngineTextureUnitState& texUnitState)
{
    for (GpuSamplerDefinitionMap::const_iterator i = m_samplerList.begin();
         i != m_samplerList.end(); ++i) {
        if (i->second.m_pTexture) {
            CPepeEngineCore::getSingleton().getRenderer()->setTextureUnitState(i->second.m_nLogicalIndex, texUnitState);
            CPepeEngineCore::getSingleton().getRenderer()->setTexture(
                i->second.m_pTexture, i->second.m_nLogicalIndex);
        }
    }
}

// -----------------------------------------------------------------------------------------
GPUProgramPtr::GPUProgramPtr() : CPepeEngineSharedPtr<IPepeEngineGPUProgram>()
{
}

// -----------------------------------------------------------------------------------------
GPUProgramPtr::GPUProgramPtr(IPepeEngineGPUProgram* r) : CPepeEngineSharedPtr<IPepeEngineGPUProgram>(r)
{
}

// -----------------------------------------------------------------------------------------
GPUProgramPtr::GPUProgramPtr(const GPUProgramPtr& r) : CPepeEngineSharedPtr<IPepeEngineGPUProgram>(r)
{
}

// -----------------------------------------------------------------------------------------
GPUProgramPtr::GPUProgramPtr(const ResourcePtr& r) : CPepeEngineSharedPtr<IPepeEngineGPUProgram>()
{
    ptr = (IPepeEngineGPUProgram*)r.data();

    if (ptr) {
        ptr->ref();
    }
}

// -----------------------------------------------------------------------------------------
GPUProgramPtr& GPUProgramPtr::operator=(const ResourcePtr& r)
{
    if (ptr == (IPepeEngineGPUProgram*)r.data()) {
        return *this;
    }

    if (ptr) {
        ptr->deref();
    }

    ptr = (IPepeEngineGPUProgram*)r.data();

    if (ptr) {
        ptr->ref();
    }

    return *this;
}

_PEPE_ENGINE_END