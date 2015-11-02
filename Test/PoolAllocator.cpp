#include "stdafx.h"
#include "PoolAllocator.h"
#include "DLMalloc.h"

PoolAllocator::PoolAllocator()
{
	m_nAllocType = eAlloc_Pool;

	m_ObjSize = 0;
	m_CellSize = sizeof(MemoryCell*);
	m_head.AllocInfo = 0;
	m_head.MemInfo = 0x7cfcfcfc;
	m_cell = NULL;
	m_MemUsage = 0;
	m_count = 0;
	m_UseCount = 0;
}


PoolAllocator::PoolAllocator(int32 nSize)
{
	if( nSize < sizeof(MemoryCell*) )
		nSize = sizeof(MemoryCell*);

	m_ObjSize = nSize;
	m_CellSize = nSize + sizeof(MemoryHead);
	m_head.AllocInfo = 0;
	m_head.MemInfo = 0x7cfcfcfc;	
	m_cell = NULL;
	m_MemUsage = 0;
	m_count = 0;
	m_UseCount = 0;
}

PoolAllocator::~PoolAllocator()
{

}

void PoolAllocator::SetMemoryInfo(const MemoryHead& head)
{
	m_head.AllocInfo = head.AllocInfo;
	m_head.MemInfo = 0x7cfcfcfc;
}

void PoolAllocator::Init(int32 nSize)
{
	if( nSize < sizeof(MemoryCell*))

}

void* PoolAllocator::PopMemory()
{
	char* pRes = NULL;

	if ( NULL == m_cell )
	{
		pRes = (char*)dlmalloc(m_CellSize);
		if(!pRes)
			return NULL;


	}
}

void PoolAllocator::PushMemory()
{

}
