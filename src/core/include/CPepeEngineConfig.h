/**
 * Project: PepeEngine
 * Tier:    Frontend
 * File:    CPepeEngineConfig.h
 *
 * @brief   Declaration of CPepeEngineConfig class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-10-15
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINECONFIG_H
#define CPEPEENGINECONFIG_H

#include "PepeEngineInit.h"

#include "CPepeEngineEvent.h"

_PEPE_ENGINE_START

enum ConfigOptionType {
    RENDERER_OPTION         = 0,
    OPERATING_SYSTEM_OPTION = 1
};

struct ConfigOptionValue {
    tstring                 strValue;
    tstring                 strDefValue;
    std::vector<tstring>    possibleValues;
    ConfigOptionType        type;
    bool                    immutable;          /**<    could/couldn't change value */

    ConfigOptionValue() : type(RENDERER_OPTION) {
        strValue            = _T("");
        strDefValue         = _T("");
        immutable           = false;
        possibleValues.clear();
    }
};

typedef stdext::hash_map<tstring, ConfigOptionValue*> ConfigOptions;

class _PepeEngineExport CPepeEngineConfig : public CPepeEngineObservable
{
    ConfigOptions m_options;
public:
    CPepeEngineConfig();
    ~CPepeEngineConfig();

    void addOption(const tstring& strName, ConfigOptionValue* strValue);
    void setOptionValue(const tstring& strName, const tstring& strValue, bool bThrow = true);
    ConfigOptionValue* getOption(const tstring& strName);

    void removeOption(const tstring& strName);

    void clearOptions(const ConfigOptionType& type);
    ConfigOptions getOptions(const ConfigOptionType& type);

    void log();

};

_PEPE_ENGINE_END

#endif