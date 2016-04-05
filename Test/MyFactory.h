#pragma once
#include "PoolAllocatorEx.h"

class PoolPacketAllocator : public PoolAllocatorEx
{
public:
	PoolPacketAllocator(){}
	PoolPacketAllocator(int32 nMaxSize) : PoolAllocatorEx(nMaxSize)
	{}

	~PoolPacketAllocator();

	virtual int32 GetType() { return eAlloc_Pool_Packet; }

	void* TMalloc(int32 nSize);
	void TFree(void* ptr);
};