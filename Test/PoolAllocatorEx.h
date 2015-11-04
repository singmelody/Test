#pragma once

#include "PoolAllocator.h"

class PoolAllocatorEx : public PoolAllocator
{
public:
	PoolAllocatorEx();
	PoolAllocatorEx(int32 nMaxSize);
	~PoolAllocatorEx();

	virtual void* TMalloc(int32 nSize);
	virtual void TFree(void* ptr);

	virtual void SetMemoryInfo(const MemoryHead& head);

	virtual void Init(int32 nSize);
	virtual int32 GetAllocInfo();

protected:
	int32 m_maxSize;
};

