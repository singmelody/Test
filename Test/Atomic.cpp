#include "StdAfx.h"
#include "Atomic.h"
#include <assert.h>
#include <Windows.h>

static union { char c[4]; unsigned long myLong; }test_endian = { {'L', '?', '?', 'B'}};

#define ENDIANNESS ( (char)(test_endian.myLong))

void InterLockedAdd(volatile long * dst, long nVal)
{
	assert(dst);
	InterlockedExchangeAdd( dst, nVal);
}

void InterLockedAdd64(volatile int64 * dst, int64 nVal)
{
	assert(dst);
	InterlockedAdd64( dst, nVal);
}

long InterLockedCompareExchange(volatile long *dst, long val, long compare)
{
	assert(dst);
	return InterlockedCompareExchange( dst, val, compare);
}

void AtomicInt64::Add(int64 nVal)
{
	InterLockedAdd64( &m_val, nVal);
	InterLockedAdd64( &m_addTimes, 1);
}

void AtomicInt64::Reset()
{
	m_val = 0;
	m_addTimes = 0;
}

const long WRITE_LOCK_VALUE = 1 << 24;
ReadLock::ReadLock(volatile long* lock)
{
	InterLockedAdd( m_pLock, 1);

	{
		volatile char* pWL = (volatile char *)m_pLock;
		if(ENDIANNESS == 'L')
			pWL += 3;

		for (;*pWL;)
			;
	}
}

ReadLock::~ReadLock()
{
	InterLockedAdd( m_pLock, -1);
}


WriteLock::WriteLock(volatile long* lock)
{
	assert(m_pLock);
	while ( 0 != InterLockedCompareExchange( m_pLock, WRITE_LOCK_VALUE, 0))
	{
	}
}

WriteLock::~WriteLock()
{
	InterLockedAdd( m_pLock, WRITE_LOCK_VALUE);
}