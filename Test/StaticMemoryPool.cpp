#include "StdAfx.h"
#include "StaticMemoryPool.h"
#include "MyLog.h"
#include <assert.h>



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

void* StaticMemoryPool::Allocate(uint32 nObjSize)
{
	assert( nObjSize == m_nUnitSize - 1);

	MemBlockView_Raw* ptrRet = m_pFreeBlockList;

	if( NULL == ptrRet )
	{
		MyLog::error("StaticMemoryPool::Allocate Failed m_nUnitSize=[%d] m_nUnitCount=[%d]", m_nUnitSize, m_nUnitCount);
		return NULL;
	}

	char* ptrChRet = ((char*)ptrRet) + HEADER_SIZE_FOR_SHARED_MEMORY;

	memset( ptrChRet + (nObjSize - m_nTailZeroFillSize), 0, m_nTailZeroFillSize);

	ptrRet->IsInUse(true);
	m_pFreeBlockList = m_pFreeBlockList->m_pNext;

	return ptrChRet;
}

void StaticMemoryPool::Free(void* ptr)
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
