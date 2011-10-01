#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineConfig.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineConfig::CPepeEngineConfig()
{
}

// -----------------------------------------------------------------------------------------
CPepeEngineConfig::~CPepeEngineConfig()
{
    ConfigOptions::iterator it = m_options.begin(), end = m_options.end();

    while (it != end) {
        delete it->second;
        it->second = NULL;
        it++;
    }

    m_options.clear();
}

// -----------------------------------------------------------------------------------------
void CPepeEngineConfig::addOption(const tstring& strName, ConfigOptionValue* strValue)
{
    m_options.insert(ConfigOptions::value_type(strName, strValue));
}

// -----------------------------------------------------------------------------------------
void CPepeEngineConfig::setOptionValue(const tstring& strName, const tstring& strValue, bool bThrow)
{
    ConfigOptionValue* val = getOption(strName);

    if (val->immutable && bThrow) {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_INVALID_PARAMETERS,
            _T("Option ") + strName + _T(" is immutable."),
            _T("CPepeEngineConfig::setOptionValue")
        );
    }

    std::vector<tstring>::const_iterator itValue = std::find(
                                                       val->possibleValues.begin(),
                                                       val->possibleValues.end(),
                                                       val->strValue
                                                   );

    if ( (itValue == val->possibleValues.end()) && bThrow) {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_INVALID_PARAMETERS,
            _T("Value ") + strValue + _T(" is not possible for option ") + strName,
            _T("CPepeEngineConfig::setOptionValue")
        );
    } else {
        val->strValue = strValue;
    }

    notifyListeners(CPepeEngineEvent(EVENT_CONFIG_CHANGED));
}

// -----------------------------------------------------------------------------------------
ConfigOptionValue* CPepeEngineConfig::getOption(const tstring& strName)
{
    ConfigOptions::iterator it = m_options.find(strName);

    if (it == m_options.end()) {
        PEPE_ENGINE_EXCEPT(
            CPepeEngineException::ERR_ITEM_NOT_FOUND_ERROR,
            strName + _T(" - Option not found"),
            _T("CPepeEngineConfig::getOption")
        );
    } else {
        return it->second;
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineConfig::removeOption(const tstring& strName)
{
    ConfigOptions::iterator it = m_options.find(strName);

    if (it != m_options.end()) {
        delete it->second;
        it->second = NULL;
        m_options.erase(it);
    }
}

// -----------------------------------------------------------------------------------------
void CPepeEngineConfig::clearOptions(const ConfigOptionType& type)
{
    ConfigOptions::iterator it = m_options.begin(), end = m_options.end();
    std::vector<tstring> names;

    while (it != end) {
        if (it->second->type == type) {
            names.push_back(it->first);
        }

        it++;
    }

    std::vector<tstring>::iterator nIt = names.begin(), nend = names.end();

    while (nIt != nend) {
        removeOption(*nIt);

        nIt++;
    }

}

// -----------------------------------------------------------------------------------------
ConfigOptions CPepeEngineConfig::getOptions(const ConfigOptionType& type)
{
    ConfigOptions::iterator it = m_options.begin(), end = m_options.end();
    ConfigOptions ret;

    while (it != end) {
        if (it->second->type == type) {
            ret.insert(ConfigOptions::value_type(it->first, it->second));
        }

        it++;
    }

    return ret;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineConfig::log()
{
    LOG("Config options");
    LOG("-------------------------");
    ConfigOptions::iterator it = m_options.begin(), end = m_options.end();

    while (it != end) {
        LOG(" * " + it->first + _T(": ") + (tstring)it->second->strValue);
        it++;
    }
}

_PEPE_ENGINE_END