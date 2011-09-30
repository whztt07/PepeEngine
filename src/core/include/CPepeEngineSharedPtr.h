/**
 * Project:		PepeEngine
 * Tier:		Frontend
 * File:		CPepeEngineSharedPtr.h   
 *
 * @brief		Declaration of CPepeEngineSharedPtr class.
 *
 * @author		Piotr 'pepe' Picheta
 * @date		2008-12-18
 * @copyright   Copyright (c) 2008 Piotr Picheta
 *
 * @version		1.0
 */

#ifndef CPEPEENGINESHAREDPTR_H
#define CPEPEENGINESHAREDPTR_H

#include "PepeEngineInit.h"

_PEPE_ENGINE_START

class _PepeEngineExport CPepeEngineShared 
{
public:	
	inline						CPepeEngineShared();
	inline						CPepeEngineShared(const CPepeEngineShared &);
	inline CPepeEngineShared&	operator=(const CPepeEngineShared &);
	inline void					ref() const;	
	inline void					deref() const;
	inline int					count() const;
protected:
	inline virtual				~CPepeEngineShared();
private:
	mutable int					nCount;
};

template<class T> 
struct CPepeEngineSharedPtr
{
protected:
	T* ptr;
public:
	CPepeEngineSharedPtr() : ptr(0) {}
	CPepeEngineSharedPtr(T* t) : ptr(t) {if (ptr) ptr->ref();}		
	CPepeEngineSharedPtr(const CPepeEngineSharedPtr& p) : ptr(p.ptr) {if (ptr) ptr->ref();}	
	virtual ~CPepeEngineSharedPtr() {if (ptr) ptr->deref();}	
	CPepeEngineSharedPtr<T>& operator=(const CPepeEngineSharedPtr<T>& p) 
	{
		if (ptr == p.ptr) return *this;
		if (ptr) ptr->deref();
		ptr = p.ptr;
		if (ptr) ptr->ref();
		return *this;
	}
	CPepeEngineSharedPtr<T>& operator=(T* p) 
	{
		if (ptr == p)return *this;
		if (ptr) ptr->deref();
		ptr = p;
		if (ptr) ptr->ref();
		return *this;
	}
	bool operator==(const CPepeEngineSharedPtr<T>& p) const {return (ptr == p.ptr);}
	bool operator!=(const CPepeEngineSharedPtr<T>& p) const {return (ptr != p.ptr);}
	bool operator==(const T* p) const {return (ptr == p);}
	bool operator!=(const T* p) const {return (ptr != p);}
	bool operator!() const {return (ptr == 0);}	
	bool isNull() const {return (ptr == 0);}
	operator T*() const	{return ptr;}
	T* data() {return ptr;}
	const T* data() const {return ptr;}
	const T& operator*() const {return *ptr;}
	T& operator*() {return *ptr;}
	const T* operator->() const {return ptr;}
	T* operator->()	{return ptr;}
	int count() const {return ptr ? ptr->count() : 0;}
};

_PEPE_ENGINE_END

#endif