#include "stdafx.h"
#include "PoolAllocatorEx.h"


PoolAllocatorEx::PoolAllocatorEx() : PoolAllocator()
{
	m_nAllocType = eAlloc_Pool_Ex;
}


PoolAllocatorEx::PoolAllocatorEx(int32 nMaxSize)
{

}

PoolAllocatorEx::~PoolAllocatorEx()
{
}

void* PoolAllocatorEx::TMalloc(int32 nSize)
{
	LOCK(&m_lock);
	if (size > m_ObjSize)
		return NULL;

#if USE_SYS_NEW
	Char* res = (Char*)dlmalloc(m_CellSize);
	if (!res)
		return NULL;

	*((MemoryHead*)res) = m_Info;
	((MemoryHead*)res)->MemInfo |= 0x80000000;
	res = res + sizeof(MemoryHead);
	m_MemUsage += m_CellSize;
	m_Count++;
	return res;
#else
	return PopMemory();
#endif
}

void PoolAllocatorEx::TFree(void* ptr)
{

}

void PoolAllocatorEx::SetMemoryInfo(const MemoryHead& head)
{

}

void PoolAllocatorEx::Init(int32 nSize)
{

}

int32 PoolAllocatorEx::GetAllocInfo()
{

}