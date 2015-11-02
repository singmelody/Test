#include "StdAfx.h"
#include "MyMutex.h"


Mutex::Mutex()
{
	::InitializeCriticalSection(&m_cs);
}

Mutex::~Mutex()
{
	::DeleteCriticalSection(&m_cs);
}

void Mutex::Lock()
{
	::EnterCriticalSection(&m_cs);
}

bool Mutex::TryLock()
{
	return ::TryEnterCriticalSection(&m_cs);
}

void Mutex::Unlock()
{
	::LeaveCriticalSection(&m_cs);
}

