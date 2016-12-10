#include "StdAfx.h"
#include "SHMAllocator.h"
#include "ParamDef.h"

SHMAllocator::SHMAllocator(ParamDef& def, void* pMemoryBlock, uint32 nUintSize, uint32 nUintCount)
	: m_memoryPool(pMemoryBlock, nUintSize, nUintCount, def.ExtraSize())
	, m_nObjSize()
{

}

void* SHMAllocator::TMalloc(int32 nSize)
{
	int32 nTotalSize = nSize + sizeof(MemoryHead);
	assert( nSize == m_nObjSize );

	void* ptr = m_memoryPool.Allocate(nTotalSize);

	if(!ptr)
		return 0;

	((MemoryHead*)ptr)->MemInfo |= 0x80000000;
	m_nMemUsage += m_nCellSize;
	return (char*)ptr + sizeof(MemoryHead);
}

void SHMAllocator::TFree(void* ptr)
{
	char* chPtr = (char*)ptr;
	assert( chPtr != 0 && IsValidMemory(chPtr - sizeof(MemoryHead)));

	((MemoryHead*)(chPtr - sizeof(MemoryHead)))->MemInfo &= (~0x80000000);
	m_memoryPool.Free((char*)ptr-sizeof(MemoryHead));
}

void SHMAllocator::SetMemoryInfo(const MemoryHead& info)
{
	m_Info.AllocInfo = info.AllocInfo;
	m_Info.MemInfo = 0x7cfcfcfc;
}

SHMAllocator::~SHMAllocator(void)
{
}

const MemoryHead& SHMAllocator::GetMemoryInfo()
{
	return m_Info;
}

void SHMAllocator::Init(int32 nSize)
{
	assert( m_nObjSize == nSize );
}

bool SHMAllocator::IsValidMemory(void* ptr)
{
	if(!ptr)
		return false;

	MemoryHead* pHead = (MemoryHead*)ptr;
	return 0 != (pHead->MemInfo & 0x80000000);
}

int32 SHMAllocator::GetAllocInfo(void* ptr)
{
	if(!ptr)
		return 0;

	MemoryHead* pInfo = (MemoryHead*)((char*)ptr-sizeof(MemoryHead));
	return pInfo->AllocInfo;
}

