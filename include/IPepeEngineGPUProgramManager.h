/**
 * Project: PepeEngine
 * Tier:    Middleware
 * File:    IPepeEngineGPUProgramManager.h
 *
 * @brief   Declaration of IPepeEngineGPUProgramManager interface.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-09-17
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef IPEPEENGINEGPUPROGRAMMANAGER_H
#define IPEPEENGINEGPUPROGRAMMANAGER_H

#include "PepeEngineInit.h"

#include "CPepeEngineSingleton.h"
#include "IPepeEngineGPUProgram.h"
#include "IPepeEngineResourceManager.h"

_PEPE_ENGINE_START

class _PepeEngineExport IPepeEngineGPUProgramManager :
    public IPepeEngineResourceManager, public CPepeEngineSingleton<IPepeEngineGPUProgramManager>
{
    tstring m_strFilePath;
    tstring m_strDefPSShaderModel;
    tstring m_strDefVSShaderModel;
    tstring m_strDefPSEntryPoint;
    tstring m_strDefVSEntryPoint;

    std::vector<tstring> m_syntaxCodes;
public:

    IPepeEngineGPUProgramManager();
    virtual ~IPepeEngineGPUProgramManager();

    ShaderType getTypeFromFileExtension(const tstring& strFileName) const;

    virtual GPUProgramPtr createPixelShader(
        const tstring& strName,
        const tstring& strShaderModel,
        const tstring& strEntryPoint
    ) = 0;

    virtual GPUProgramPtr createVertexShader(
        const tstring& strName,
        const tstring& strShaderModel,
        const tstring& strEntryPoint
    ) = 0;

    void setFilePath(const tstring& strFilePath) {
        m_strFilePath = strFilePath;
    }
    const tstring& getFilePath() const {
        return m_strFilePath;
    }

    void setPSShaderModel(const tstring& strShaderModel);
    const tstring& getPSShaderModel() const {
        return m_strDefPSShaderModel;
    }

    void setVSShaderModel(const tstring& strShaderModel);
    const tstring& getVSShaderModel() const {
        return m_strDefVSShaderModel;
    }

    void setVSEntryPoint(const tstring& strEntryPoint) {
        m_strDefVSEntryPoint = strEntryPoint;
    }
    const tstring& getVSEntryPoint() const {
        return m_strDefVSEntryPoint;
    }

    void setPSEntryPoint(const tstring& strEntryPoint) {
        m_strDefPSEntryPoint = strEntryPoint;
    }
    const tstring& getPSEntryPoint() const {
        return m_strDefPSEntryPoint;
    }

    void pushSyntaxCode(const tstring& strCode) {
        m_syntaxCodes.push_back(strCode);
    }
    bool isSyntaxSupported(const tstring& strCode) const;

    static IPepeEngineGPUProgramManager& getSingleton(void);
    static IPepeEngineGPUProgramManager* getSingletonPtr(void);
};

_PEPE_ENGINE_END

#endif