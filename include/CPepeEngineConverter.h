/**
 * Project: PepeEngine
 * Tier:    Frontend
 * File:    CPepeEngineConverter.h
 *
 * @brief   Declaration of CPepeEngineConverter class.
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-06-20
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINECONVERTER_H
#define CPEPEENGINECONVERTER_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

#define PEPE_ENGINE_STRING(str) CPepeEngineConverter::getTString(str)
#define _TSTR(str) CPepeEngineConverter::getTString(str)

class _PepeEngineExport CPepeEngineConverter
{
public:
    static float textToNum(char* strText);

    static tstring getTString(const std::string& str);
    static tstring getTString(const char* str);
    static tstring getTString(const wchar_t* str);
    static tstring getTString(const int i);

    static std::string getString(const tstring& str);
    static void getChars(const tstring& str, char** newStr);
    static void getWChars(const tstring& str, wchar_t** newStr);

    static bool parseBool(const tstring& str);
    static size_t parseUnsignedInt(const tstring& str);
    static int parseInt(const tstring& str);

    static tstring parseString(float fVal);
    static tstring parseString(bool bVal);
};

_PEPE_ENGINE_END

#endif