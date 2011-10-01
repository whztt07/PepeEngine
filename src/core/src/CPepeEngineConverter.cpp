#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineConverter.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
float CPepeEngineConverter::textToNum(char* strText)
{
    float   fTest = 0, fNum = 10;
    bool    bSign;

    int             iTextSize   = strlen(strText);
    unsigned char   i           = 0;

    bSign = false;

    while ((bSign == false) && (i < iTextSize)) {
        switch (strText[i]) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '-':
        case '.':
            bSign = true;
            break;

        default:
            i++;
            break;
        }
    }

    if (i >= iTextSize) {
        return 0.0f;
    }

    if (strText[i] == '-') {
        bSign = true;
        i++;
    } else {
        bSign = false;
    }

    while ((strText[i] >= '0') && (strText[i] <= '9')) {
        fTest *= fNum;
        fTest += (strText[i++] - '0');
    }

    fNum = 0.1f;

    if ((i < iTextSize) && (strText[i++] == '.')) {
        while ((strText[i] >= '0') && (strText[i] <= '9')) {
            fTest   += (strText[i++] - '0') * fNum;
            fNum    *= 0.1f;
        }
    }

    if (bSign) {
        fTest = -fTest;
    }

    return fTest;
}

// -----------------------------------------------------------------------------------------
tstring CPepeEngineConverter::getTString(const std::string& str)
{
#if UNICODE
    std::wstring temp(str.length(), L' ');
    std::copy(
        str.begin(),
        str.end(),
        temp.begin()
    );

    return temp;
#else
    return str;
#endif
}

// -----------------------------------------------------------------------------------------
tstring CPepeEngineConverter::getTString(const char* str)
{
#if UNICODE

    size_t          nOrigSize           = strlen(str) + 1;
    const size_t    nNewSize            = 100;
    size_t          nConvertedChars     = 0;
    wchar_t         wcstring[nNewSize];
    mbstowcs_s(&nConvertedChars, wcstring, nOrigSize, str, _TRUNCATE);

    std::wstring temp(wcstring);

    return temp;
#else
    std::string tmp(str);
    return tmp;
#endif
}

// -----------------------------------------------------------------------------------------
tstring CPepeEngineConverter::getTString(const wchar_t* str)
{
#if UNICODE
    std::wstring temp(str);
    return temp;
#else
    size_t          nOrigSize           = wcslen(str) + 1;
    const size_t    nNewSize            = 100;
    size_t          nConvertedChars     = 0;
    char            nstring[nNewSize];
    wcstombs_s(&nConvertedChars, nstring, nOrigSize, str, _TRUNCATE);

    std::string tmp(nstring);

    return tmp;
#endif
}

// -----------------------------------------------------------------------------------------
tstring CPepeEngineConverter::getTString(const int i)
{
    tstringstream stream;
    stream.width(0);
    stream.fill(' ');
    stream << i;
    return stream.str();
}

// -----------------------------------------------------------------------------------------
std::string CPepeEngineConverter::getString(const tstring& str)
{
#if UNICODE
    size_t          nOrigSize           = str.length() + 1;
    const size_t    nNewSize            = 100;
    size_t          nConvertedChars     = 0;
    char            nstring[nNewSize];
    wcstombs_s(&nConvertedChars, nstring, nOrigSize, str.c_str(), _TRUNCATE);

    std::string tmp(nstring);

    return tmp;
#else
    return str;
#endif
}

// -----------------------------------------------------------------------------------------
void CPepeEngineConverter::getChars(const tstring& str, char** newStr)
{
#if UNICODE
    size_t          nOrigSize           = str.length() + 1;
    const size_t    nNewSize            = 100;
    size_t          nConvertedChars     = 0;

    *newStr = new char[nNewSize];
    wcstombs_s(&nConvertedChars, *newStr, nOrigSize, str.c_str(), _TRUNCATE);
#else
    *newStr = new char[100];
    strcpy_s(*newStr, 100, (char *)str.c_str());
#endif
}

// -----------------------------------------------------------------------------------------
void CPepeEngineConverter::getWChars(const tstring& str, wchar_t** newStr)
{
#if UNICODE
    *newStr = new wchar_t[100];
    wcscpy_s(*newStr, 100, (wchar_t *)str.c_str());
#else
    size_t          nOrigSize           = str.length() + 1;
    const size_t    nNewSize            = 100;
    size_t          nConvertedChars     = 0;

    *newStr = new wchar_t[nNewSize];

    mbstowcs_s(&nConvertedChars, *newStr, nOrigSize, str.c_str(), _TRUNCATE);
#endif
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineConverter::parseBool(const tstring& str)
{
    if (
        (str.find_first_of(_T("true")) == 0) ||
        (str.find_first_of(_T("yes")) == 0) ||
        (str.find_first_of(_T("1")) == 0)
    ) {
        return true;
    } else {
        return false;
    }
}

// -----------------------------------------------------------------------------------------
size_t CPepeEngineConverter::parseUnsignedInt(const tstring& str)
{
    if (str == _T("none")) {
        return 0;
    }

    tstringstream sstr(str);
    unsigned int ret = 0;
    sstr >> ret;

    return ret;
}

// -----------------------------------------------------------------------------------------
int CPepeEngineConverter::parseInt(const tstring& str)
{
    tstringstream sstr(str);
    int ret = 0;
    sstr >> ret;

    return ret;
}

// -----------------------------------------------------------------------------------------
tstring CPepeEngineConverter::parseString(float fVal)
{
    tstringstream sstr;
    sstr << fVal;

    return sstr.str();
}

// -----------------------------------------------------------------------------------------
tstring CPepeEngineConverter::parseString(bool bVal)
{
    if (bVal) {
        return _T("yes");
    } else {
        return _T("no");
    }
}

_PEPE_ENGINE_END