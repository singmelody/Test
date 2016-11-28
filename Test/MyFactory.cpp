#include "StdAfx.h"
#include "MyFactory.h"


PoolPacketAllocator::~PoolPacketAllocator()
{

}

void* PoolPacketAllocator::TMalloc(int32 nSize)
{
	void* res = PoolAllocatorEx::TMalloc(nSize);
	return res;
}

void PoolPacketAllocator::TFree(void* ptr)
{
	PoolAllocatorEx::TFree(ptr);
}
