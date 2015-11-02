#include "stdafx.h"
#include "PoolAllocator.h"
#include "DLMalloc.h"

PoolAllocator::PoolAllocator()
{
	m_objSize = 0;
	m_cellSize = 0;
	m_info.AllocInfo = 0;
	m_info.MemInfo = 0x7cfcfcfc;
	m_head = NULL;
	m_memUsage = 0;
	m_count = 0;
	m_useCount = 0;
}


// PoolAllocator::PoolAllocator(int32 nSize)
// {
// 	if( nSize < sizeof(MemoryCell*) )
// 		nSize = sizeof(MemoryCell*);
// 
// 	m_objSize = nSize;
// 	m_cellSize = nSize + sizeof(MemoryHead);
// 	m_info.AllocInfo = 0;
// 	m_info.MemInfo = 0x7cfcfcfc;	
// 	m_cell = NULL;
// 	m_memUsage = 0;
// 	m_count = 0;
// 	m_useCount = 0;
// }

PoolAllocator::~PoolAllocator()
{

}

void PoolAllocator::SetMemoryInfo(const MemoryHead& head)
{
	m_info.AllocInfo = head.AllocInfo;
	m_info.MemInfo = 0x7cfcfcfc;
}

const MemoryHead& PoolAllocator::GetMemoryInfo()
{
	return m_info;
}

void PoolAllocator::Init(int32 nSize)
{
	if( nSize < sizeof(MemoryCell*) )
		nSize = sizeof(MemoryCell*);

	m_objSize = nSize;
	m_cellSize = nSize + sizeof(MemoryHead);
	m_info.AllocInfo = 0;
	m_info.MemInfo = 0x7cfcfcfc;	
	m_head = NULL;
	m_memUsage = 0;
	m_count = 0;
	m_useCount = 0;
}

void* PoolAllocator::TMalloc(int32 nSize)
{
	if ( nSize > m_objSize )
		return NULL;

	return PopMemory();
}

void PoolAllocator::TFree(void* ptr)
{
	if(!ptr)
		return;

	return PushMemory(ptr);
}

void* PoolAllocator::PopMemory()
{
	char* pRes = NULL;

	if ( !m_head )
	{
		pRes = (char*)dlmalloc(m_cellSize);
		if(!pRes)
			return NULL;

		m_memUsage += m_cellSize;
		m_count++;
	}
	else
	{
		pRes = (char*)m_head;
		m_head = m_head->_next;
	}

	*((MemoryHead*)pRes) = m_info;
	((MemoryHead*)pRes)->MemInfo |= 0x8000000;
	pRes += sizeof(MemoryHead);

	m_useCount++;

	return (void*)pRes;
}

void PoolAllocator::PushMemory( void* ptr)
{
	MemoryCell* pCell = (MemoryCell*)((char*)ptr - sizeof(MemoryHead));
	if(pCell->info.AllocInfo != m_info.AllocInfo)
	{
		printf("Pool Allocator Push Memory Failed, Wrong AllocInfo\n");
		dlfree((void*)pCell);
		return;
	}

	if((pCell->info.MemInfo & 0x80000000) == 0)
	{
		printf("Free Unused Memory\n");
		return;
	}

	pCell->info.MemInfo &= 0x7fffffff;
	pCell->_next = m_head;
	m_head = pCell;
	m_useCount--;
}


int32 PoolAllocator::GetAllocInfo(void* ptr)
{
	if(!ptr)
		return -1;

	MemoryHead* pCell = (MemoryHead*)((char*)ptr-sizeof(MemoryHead));
	return pCell->AllocInfo;
}

int32 PoolAllocator::GetMemorySize(void* ptr)
{
	if(!ptr)
		return -1;

	MemoryHead* pHead = (MemoryHead*)((char*)ptr-sizeof(MemoryHead));
	return pHead->MemInfo;
}

void PoolAllocator::CollectMemory()
{
	while(m_head)
	{
		void* ptr = PopMemory();
		void* realPtr = (void*)((char*)ptr-sizeof(MemoryHead));
		dlfree(realPtr);
		m_memUsage -= m_cellSize;
		--m_count;
	}
}

bool PoolAllocator::IsValidMemory(void* ptr)
{
	if(!ptr)
		return false;

	MemoryHead* pCell = (MemoryHead*)((char*)ptr-sizeof(MemoryHead));
	return ( pCell->MemInfo & 0x80000000 ) 
		&& (pCell->AllocInfo == m_info.AllocInfo);
}

int32 PoolAllocator::GetAllocInfoStatic(void* ptr)
{
	if (!ptr)
		return -1;

	MemoryHead* pCell = (MemoryHead*)((Char*)ptr - sizeof(MemoryHead));
	return pCell->AllocInfo;
}

int32 PoolAllocator::GetMemorySizeStatic(void* ptr)
{
	if(!ptr)
		return -1;

	MemoryHead* pCell = (MemoryHead*)((Char*)ptr - sizeof(MemoryHead));
	return pCell->MemInfo & 0x7fffffff;
}

MemoryHead* PoolAllocator::GetMemoryInfoStaitc(void* ptr)
{
	if(!ptr)
		return NULL;

	MemoryHead* pCell = (MemoryHead*)((Char*)ptr - sizeof(MemoryHead));
	return pCell;
}