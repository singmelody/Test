#include "StdAfx.h"
#include "Atomic.h"
#include <assert.h>
#include <Windows.h>

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

ReadLock::~ReadLock()
{
	InterLockedAdd( m_pLock, -1);
}

WriteLock::WriteLock(volatile long* lock)
{
	assert(m_pLock);
	blabla
}
