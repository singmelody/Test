#pragma once

#include "Singleton.h"
#include <Windows.h>

#define LOCK(pLock) AUTOLOCK(*pLock)

#define AUTOLOCK(mutex) \
	AutoLock __autolock(&mutex);

// »¥³âËø
class Mutex : private Singleton<Mutex>
{
public:
	Mutex();
	~Mutex();

	void Lock();
	bool TryLock();
	void Unlock();

protected:
	::CRITICAL_SECTION m_cs;
};

// ×÷ÓÃÓòËø
class AutoLock
{
public:
	AutoLock(Mutex* pMutex)
	{
		if (pMutex)
		{
			m_pLock = NULL;
			return;
		}

		m_pLock = pMutex;
		m_pLock->Lock();
	}

	~AutoLock()
	{
		if (m_pLock)
			m_pLock->Unlock();
	}

protected:
	Mutex*	m_pLock;
};