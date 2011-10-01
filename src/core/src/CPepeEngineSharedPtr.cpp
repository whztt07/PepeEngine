#include "PepeEnginePrecompiledHeaders.h"
#include "CPepeEngineSharedPtr.h"

_PEPE_ENGINE_START

// -----------------------------------------------------------------------------------------
CPepeEngineShared::CPepeEngineShared() : nCount(0)
{
}

// -----------------------------------------------------------------------------------------
CPepeEngineShared::CPepeEngineShared(const CPepeEngineShared &) : nCount(0)
{
}

// -----------------------------------------------------------------------------------------
CPepeEngineShared& CPepeEngineShared::operator=(const CPepeEngineShared &)
{
    return *this;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineShared::ref() const
{
    nCount++;
}

// -----------------------------------------------------------------------------------------
void CPepeEngineShared::deref() const
{
    assert(nCount > 0 && "Invalid reference count");

    if (!--nCount) {
        delete this;
    }
}

// -----------------------------------------------------------------------------------------
int CPepeEngineShared::count() const
{
    return nCount;
}

// -----------------------------------------------------------------------------------------
CPepeEngineShared::~CPepeEngineShared()
{
}

_PEPE_ENGINE_END