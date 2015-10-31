#pragma once

#include "PoolAllocator.h"
#include "MyMutex.h"

class PoolAllocatorEx : public PoolAllocator
{
public:
	PoolAllocatorEx();
	PoolAllocatorEx(int32 nMaxSize);
	~PoolAllocatorEx();

	virtual void* TMalloc(int32 nSize);
	virtual void TFree(void* ptr);
	virtual int32 GetType() { return eAlloc_Pool_Ex; }
protected:

	int32 m_maxSize;
	Mutex m_lock;
};

