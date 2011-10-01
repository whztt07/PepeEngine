/**
 * Project: PepeEngine
 * Tier:    Frontend
 * File:    CPepeEngineException.h
 *
 * @class CPepeEngineException
 *
 * @brief   Declaration of CPepeEngineException class. This is exception class throwed by engine. Inspired by Ogre :)
 *
 * @author      Piotr 'pepe' Picheta
 * @date        2008-02-16
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version 1.0
 */

#ifndef CPEPEENGINEEXCEPTION_H
#define CPEPEENGINEEXCEPTION_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

/**
 * CPepeEngineException class.
 */
class _PepeEngineExport CPepeEngineException : public std::exception
{
    int                 m_iError;           /**< code of error */
    long                m_lLine;

    tstring         m_strDescription;   /**< error message */
    tstring         m_strSource;
    tstring         m_strType;
    char*           m_strFile;

    mutable tstring m_strFullDescription;
public:

    /**
     * Enumeration of error codes
     */
    enum ERRORS {
        ERR_TEST_MESSAGE            ,   /**< Code of test message                   */
        ERR_RENDERINGAPI_ERROR      ,   /**< Code of rendering api error message    */
        ERR_INTERNAL_ERROR          ,   /**< Code of internal error message         */
        ERR_OUT_OF_MEMORY           ,   /**< Code of out of memory error            */
        ERR_FILE_NOT_FOUND_ERROR    ,   /**< Code of file not found error           */
        ERR_ITEM_NOT_FOUND_ERROR    ,   /**< Code of item not found error           */
        ERR_FILE_LOAD_ERROR         ,   /**< Code of file load error                */
        ERR_INVALID_PARAMETERS      ,   /**< Code of invalid parameters error       */
        ERR_REFERENCE_COUNTER_ERROR     /**< Code of reference counter error        */
    };

    /**
     * Constructor of CPepeEngineException.
     *
     * @param iError code of error message
     */
    CPepeEngineException(
        const int &iErrorType, const tstring& strDescription, const tstring& strSource,
        const tstring& strType, char* strFile, const long &lLine
    ) : m_iError(iErrorType), m_strDescription(strDescription), m_strSource(strSource),
        m_strType(strType), m_strFile(strFile), m_lLine(lLine)
    {}

    /**
     * Needed for compatibility with std::exception
     */
    ~CPepeEngineException() throw() {}

    /**
     * Method return error message.
     *
     * @return error message
     */
    virtual const tstring& getFullDescription() const;
    virtual const tstring& getDescription() const {
        return m_strDescription;
    }
    virtual const tstring& getSource() const {
        return m_strSource;
    }
    virtual const tstring& getType() const {
        return m_strType;
    }
    virtual const char* getFile() const {
        return m_strFile;
    }

    virtual int getLine() const {
        return m_lLine;
    }

    /**
     * Override std::exception::what
     */
    const char* what() const throw() {
        return (char *)getFullDescription().c_str();
    }
};

template<int iCode>
struct ErrorCode {
    enum {iErrCode = iCode};
};

//-------------------------------------------------------------------------------
class _PepeEngineExport CPepeEngineRenderingAPIException : public CPepeEngineException
{
public:
    CPepeEngineRenderingAPIException(const int &iErrorType, const tstring& strDescription, const tstring& strSource, char* strFile, const long &lLine)
        : CPepeEngineException(iErrorType, strDescription, strSource, _T("RenderingAPIException"), strFile, lLine) {}
};

class _PepeEngineExport CPepeEngineInternalException : public CPepeEngineException
{
public:
    CPepeEngineInternalException(const int &iErrorType, const tstring& strDescription, const tstring& strSource, char* strFile, const long &lLine)
        : CPepeEngineException(iErrorType, strDescription, strSource, _T("InternalException"), strFile, lLine) {}
};

class _PepeEngineExport CPepeEngineOutOfMemoryException : public CPepeEngineException
{
public:
    CPepeEngineOutOfMemoryException(const int &iErrorType, const tstring& strDescription, const tstring& strSource, char* strFile, const long &lLine)
        : CPepeEngineException(iErrorType, strDescription, strSource, _T("OutOfMemoryException"), strFile, lLine) {}
};

class _PepeEngineExport CPepeEngineFileNotFoundException : public CPepeEngineException
{
public:
    CPepeEngineFileNotFoundException(const int &iErrorType, const tstring& strDescription, const tstring& strSource, char* strFile, const long &lLine)
        : CPepeEngineException(iErrorType, strDescription, strSource, _T("FileNotFoundException"), strFile, lLine) {}
};

class _PepeEngineExport CPepeEngineItemNotFoundException : public CPepeEngineException
{
public:
    CPepeEngineItemNotFoundException(const int &iErrorType, const tstring& strDescription, const tstring& strSource, char* strFile, const long &lLine)
        : CPepeEngineException(iErrorType, strDescription, strSource, _T("ItemNotFoundException"), strFile, lLine) {}
};

class _PepeEngineExport CPepeEngineFileLoadException : public CPepeEngineException
{
public:
    CPepeEngineFileLoadException(const int &iErrorType, const tstring& strDescription, const tstring& strSource, char* strFile, const long &lLine)
        : CPepeEngineException(iErrorType, strDescription, strSource, _T("FileLoadException"), strFile, lLine) {}
};

class _PepeEngineExport CPepeEngineInvalidParametersException : public CPepeEngineException
{
public:
    CPepeEngineInvalidParametersException(const int &iErrorType, const tstring& strDescription, const tstring& strSource, char* strFile, const long &lLine)
        : CPepeEngineException(iErrorType, strDescription, strSource, _T("InvalidParametersException"), strFile, lLine) {}
};

class _PepeEngineExport CPepeEngineReferenceCounterException : public CPepeEngineException
{
public:
    CPepeEngineReferenceCounterException(const int &iErrorType, const tstring& strDescription, const tstring& strSource, char* strFile, const long &lLine)
        : CPepeEngineException(iErrorType, strDescription, strSource, _T("ReferenceCounterException"), strFile, lLine) {}
};
//-------------------------------------------------------------------------------

class _PepeEngineExport CPepeEngineExceptionFactory
{
    CPepeEngineExceptionFactory() {}
public:
    static CPepeEngineRenderingAPIException create(
        const ErrorCode<CPepeEngineException::ERR_RENDERINGAPI_ERROR> &iCode,
        const tstring &strDesc,
        const tstring &strSrc,
        char* strFile,
        const long &lLine
    ) {
        return CPepeEngineRenderingAPIException(iCode.iErrCode, strDesc, strSrc, strFile, lLine);
    }

    static CPepeEngineInternalException create(
        const ErrorCode<CPepeEngineException::ERR_INTERNAL_ERROR> &iCode,
        const tstring &strDesc,
        const tstring &strSrc,
        char* strFile,
        const long &lLine
    ) {
        return CPepeEngineInternalException(iCode.iErrCode, strDesc, strSrc, strFile, lLine);
    }


    static CPepeEngineOutOfMemoryException create(
        const ErrorCode<CPepeEngineException::ERR_OUT_OF_MEMORY> &iCode,
        const tstring &strDesc,
        const tstring &strSrc,
        char* strFile,
        const long &lLine
    ) {
        return CPepeEngineOutOfMemoryException(iCode.iErrCode, strDesc, strSrc, strFile, lLine);
    }

    static CPepeEngineFileNotFoundException create(
        const ErrorCode<CPepeEngineException::ERR_FILE_NOT_FOUND_ERROR> &iCode,
        const tstring &strDesc,
        const tstring &strSrc,
        char* strFile,
        const long &lLine
    ) {
        return CPepeEngineFileNotFoundException(iCode.iErrCode, strDesc, strSrc, strFile, lLine);
    }

    static CPepeEngineItemNotFoundException create(
        const ErrorCode<CPepeEngineException::ERR_ITEM_NOT_FOUND_ERROR> &iCode,
        const tstring &strDesc,
        const tstring &strSrc,
        char* strFile,
        const long &lLine
    ) {
        return CPepeEngineItemNotFoundException(iCode.iErrCode, strDesc, strSrc, strFile, lLine);
    }

    static CPepeEngineFileLoadException create(
        const ErrorCode<CPepeEngineException::ERR_FILE_LOAD_ERROR> &iCode,
        const tstring &strDesc,
        const tstring &strSrc,
        char* strFile,
        const long &lLine
    ) {
        return CPepeEngineFileLoadException(iCode.iErrCode, strDesc, strSrc, strFile, lLine);
    }

    static CPepeEngineInvalidParametersException create(
        const ErrorCode<CPepeEngineException::ERR_INVALID_PARAMETERS> &iCode,
        const tstring &strDesc,
        const tstring &strSrc,
        char* strFile,
        const long &lLine
    ) {
        return CPepeEngineInvalidParametersException(iCode.iErrCode, strDesc, strSrc, strFile, lLine);
    }

    static CPepeEngineReferenceCounterException create(
        const ErrorCode<CPepeEngineException::ERR_REFERENCE_COUNTER_ERROR> &iCode,
        const tstring &strDesc,
        const tstring &strSrc,
        char* strFile,
        const long &lLine
    ) {
        return CPepeEngineReferenceCounterException(iCode.iErrCode, strDesc, strSrc, strFile, lLine);
    }
};


#ifndef PEPE_ENGINE_EXCEPT
#define PEPE_ENGINE_EXCEPT(iNum, strDesc, strSrc) throw CPepeEngineExceptionFactory::create( \
    ErrorCode<iNum>(), strDesc, strSrc, __FILE__, __LINE__);
#endif

_PEPE_ENGINE_END

#endif