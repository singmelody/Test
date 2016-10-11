#include "StdAfx.h"
#include "StaticMemoryPool.h"




StaticMemoryPool::StaticMemoryPool(void* pMemoryBlock, uint32 nUnitSize, uint32 nUnitCnt, uint32 nZeroFillSize)
	: m_nUnitSize(nUnitSize)
	, m_nUnitCount(nUnitCnt)
	, m_nTailZeroFillSize(nZeroFillSize)
	, m_pMemoryBlock(pMemoryBlock)
	, m_pFreeBlockList(NULL)
{

}

StaticMemoryPool::~StaticMemoryPool(void)
{
}

void* StaticMemoryPoolMT::Alloc(uint32 nObjSize)
{
	AUTOLOCK(m_lock);
	return StaticMemoryPool::Allocate(nObjSize);
}

void StaticMemoryPoolMT::Free(void* ptr)
{
	AUTOLOCK(m_lock);
	return StaticMemoryPool::Free(ptr);
}
