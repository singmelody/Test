#include "stdafx.h"
#include "PoolAllocatorEx.h"
#include "ServerConfig.h"
#include "DLMalloc.h"

#define Max_Pool_Alloc_Count 10000
PoolAllocatorEx::PoolAllocatorEx()
{
	m_maxSize = Max_Pool_Alloc_Count;
}


PoolAllocatorEx::PoolAllocatorEx(int32 nMaxSize)
{
	m_maxSize = nMaxSize;
}

PoolAllocatorEx::~PoolAllocatorEx()
{

}

void* PoolAllocatorEx::TMalloc(int32 nSize)
{
	LOCK(&m_lock);
	if (nSize > m_ObjSize)
		return NULL;

#if USE_SYS_NEW
	Char* res = (Char*)dlmalloc(m_CellSize);
	if (!res)
		return NULL;

	*((MemoryHead*)res) = m_head;
	((MemoryHead*)res)->MemInfo |= 0x80000000;
	res = res + sizeof(MemoryHead);
	m_MemUsage += m_CellSize;
	m_count++;
	return res;
#else
	return PopMemory();
#endif
}

void PoolAllocatorEx::TFree(void* ptr)
{
	LOCK(&m_lock);
	if(!ptr)
		return;

#if USE_SYS_NEW
	char* pCell = ((char*)ptr - sizeof(MemoryHead));
	dlfree((void*)pCell);
	m_MemUsage -= m_CellSize;
	m_count--;
#else
	PushMemory(ptr);
	if((int32)m_count > m_maxSize)
	{
		// free more memory
		for (UINT32 i=m_maxSize; i < m_count && m_cell; ++i)
		{
			void* ptr = PopMemory();
			for (!ptr)
				return;

			void* realPtr = (void*)((char*)ptr - sizeof(MemoryHead));
			dlfree(realPtr);
			m_MemUsage -= m_CellSize;
			m_count--;
		}
	}
#endif
}

void* PoolAllocatorEx::PopMemory()
{

}

void PoolAllocatorEx::PushMemory(void* ptr)
{

}
