#include "stdafx.h"
#include "PoolAllocator.h"


PoolAllocator::PoolAllocator()
{
	m_nAllocType = eAlloc_Pool;

	m_ObjSize = 0;
	m_CellSize = sizeof(MemoryCell*);
	m_Info.AlloInfo = 0;
	m_Info.MemInfo = 0x7cfcfcfc;
	_head = NULL;
	m_MemUsage = 0;
	m_Count = 0;
	m_UseCount = 0;
}


PoolAllocator::~PoolAllocator()
{
	if( size < sizeof(MemoryCell*) )
		size = sizeof(MemoryCell*);

	m_ObjSize = size;
	m_CellSize = size + sizeof(MemoryHead);
	m_Info.AllocInfo = 0;
	m_Info.MemInfo = 0x7cfcfcfc;	
	_head = NULL;
	m_MemUsage = 0;
	m_Count = 0;
	m_UseCount = 0;
}
