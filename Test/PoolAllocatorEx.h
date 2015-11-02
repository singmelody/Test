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
protected:
	void* PopMemory();
	void PushMemory( void* ptr);

	int32 m_maxSize;
	Mutex m_lock;
};

