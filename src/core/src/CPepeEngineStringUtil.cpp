#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineStringUtil.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
bool CPepeEngineStringUtil::startsWith(const tstring& str, const tstring& pattern, bool lowerCase)
{
	size_t thisLen		= str.length();
	size_t patternLen	= pattern.length();
	if (thisLen < patternLen || patternLen == 0)
	{
		return false;
	}

	tstring startOfThis = str.substr(0, patternLen);
	if (lowerCase)
	{
		CPepeEngineStringUtil::toLowerCase(startOfThis);
	}

	return (startOfThis == pattern);
}

// -----------------------------------------------------------------------------------------
bool CPepeEngineStringUtil::endsWith(const tstring& str, const tstring& pattern, bool lowerCase)
{
	size_t thisLen		= str.length();
	size_t patternLen	= pattern.length();

	if (thisLen < patternLen || patternLen == 0)
	{
		return false;
	}

	tstring endOfThis = str.substr(thisLen - patternLen, patternLen);
	if (lowerCase)
	{
		CPepeEngineStringUtil::toLowerCase(endOfThis);
	}

	return (endOfThis == pattern);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineStringUtil::toLowerCase(tstring& str)
{
	std::transform(
		str.begin(),
		str.end(),
		str.begin(),
		tolower
	);
}

// -----------------------------------------------------------------------------------------
void CPepeEngineStringUtil::toUpperCase(tstring& str) 
{
	std::transform(
		str.begin(),
		str.end(),
		str.begin(),
		toupper
	);
}

_PEPE_ENGINE_END