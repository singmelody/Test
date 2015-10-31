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
	if (nSize > m_objSize)
		return NULL;

	return PopMemory();
}

void PoolAllocatorEx::TFree(void* ptr)
{
	LOCK(&m_lock);
	if(!ptr)
		return;

	PushMemory(ptr);
	if((int32)m_count > m_maxSize)
	{
		// free more memory
		for (int32 i=m_maxSize; i < m_count && m_head; ++i)
		{
			void* getPtr = PopMemory();
			if(!getPtr)
				return;

			void* realPtr = (void*)((char*)getPtr - sizeof(MemoryHead));
			dlfree(realPtr);
			m_memUsage -= m_cellSize;
			m_count--;
		}
	}
}
