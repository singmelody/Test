#pragma once

#include "MyMutex.h"

template <typename T>
class SmartPtr;

class IReferable
{
	friend class SmartPtr<IReferable>;

public:
	IReferable() : m_nRefCount(0){}
	virtual ~IReferable(){}

	void IncRef()
	{
		m_Mutex.Lock();
		++m_nRefCount;
		m_Mutex.Unlock();
	}

	void DecRef()
	{
		m_Mutex.Lock();
		--m_nRefCount;
		m_Mutex.Unlock();
	}
protected:
	virtual void Release()
	{
		delete this;
	}

	size_t		m_nRefCount;
	Mutex		m_Mutex;
};

template <typename T>
class SmartPtr
{
public:
	SmartPtr() : m_ptr(NULL){}
	SmartPtr(T* ptr) : m_ptr(ptr)
	{
		if( m_ptr != NULL) m_ptr->IncRef();
	}

	~SmartPtr()
	{
		if( m_ptr != NULL)
			m_ptr->DecRef();
	}

	const SmartPtr& operator=(const SmartPtr& ptr)
	{
		if( this == &ptr )
			return *this;

		if( m_ptr )
			m_ptr->DecRef();

		m_ptr = ptr.m_ptr;

		if(m_ptr)
			m_ptr->IncRef();

		return *this;
	}

	const SmartPtr& operator=(const T* ptr)
	{
		if( this == ptr )
			return *this;

		if( m_ptr )
			m_ptr->DecRef();

		m_ptr = ptr;

		if(m_ptr)
			m_ptr->IncRef();

		return *this;
	}

	bool operator == (const SmartPtr& ptr) const
	{
		return m_ptr == ptr.m_ptr;
	}

	bool operator == (const T& ptr) const
	{
		return m_ptr == m_ptr;
	}

	bool operator != (const SmartPtr& ptr) const
	{
		return m_ptr != ptr.m_ptr;
	}

	bool operator != (const T& ptr) const
	{
		return m_ptr != ptr;
	}

	T* operator->()
	{
		return m_ptr;
	}

	const T* operator->() const
	{
		return m_ptr;
	}

	T& operator*()
	{
		return *m_ptr;
	}

	const T& operator*() const
	{
		return *m_ptr;
	}

protected:
	T*	m_ptr;
};