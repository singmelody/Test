#include "StdAfx.h"
#include "DLAllocator.h"
#include "DLMalloc.h"

DLAllocator::DLAllocator(void)
{
}


DLAllocator::~DLAllocator(void)
{
}

void* DLAllocator::TMalloc(int32 nSize)
{
	int32 nRealSize = nSize + sizeof(MemoryHead);
	void* result = dlmalloc(nRealSize);
	if(!result)
		return NULL;

	*((MemoryHead*)result) = m_info;
	((MemoryHead*)result)->MemInfo |= 0x80000000;

	return (void*)((char*)result + sizeof(MemoryHead));
}

void DLAllocator::TFree(void* ptr)
{
	if(!ptr)
		return;

	void* realPtr = (void*)((char*)ptr - sizeof(MemoryHead));
	MemoryHead* pHead = (MemoryHead*)(realPtr);
	pHead->MemInfo &= 0x7fffffff;
	dlfree(realPtr);
}

void DLAllocator::SetMemoryInfo(const MemoryHead& info)
{
	m_info.AllocInfo = info.AllocInfo;
	m_info.MemInfo = 0x7cfcfcfc;
}

const MemoryHead& DLAllocator::GetMemoryInfo()
{
	return m_info;
}

void DLAllocator::Init(int32 nSize)
{
	m_size = nSize + sizeof(MemoryHead);
	m_info.MemInfo = 0x7cfcfcfc;
}

bool DLAllocator::IsValidMemory(void* ptr)
{
	if(!ptr)
		return false;

	MemoryHead* pHead = (MemoryHead*)((char*)ptr - sizeof(MemoryHead));
	return (pHead->MemInfo & 0x80000000) && (pHead->AllocInfo == m_info.AllocInfo);
}

int32 DLAllocator::GetAllocInfo(void* ptr)
{
	if(!ptr)
		return false;

	MemoryHead* pHead = (MemoryHead*)((char*)ptr - sizeof(MemoryHead));
	return pHead->AllocInfo;
}

uint64 DLAllocator::GetMemoryUsage()
{
	return dlmalloc_footprint();
}

void DLAllocator::CollectMemory()
{
	dlmalloc_trim(0);
}
