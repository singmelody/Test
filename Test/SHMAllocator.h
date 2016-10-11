#pragma once
#include "Allocator.h"
#include "StaticMemoryPool.h"

class SHMAllocator : public Allocator
{
public:
	SHMAllocator(class ParamDef& def, void* pMemoryBlock, uint32 nUintSize, uint32 nUintCount);
	virtual ~SHMAllocator(void);

private:
	SHMAllocator& operator = (const SHMAllocator& );
	SHMAllocator(const SHMAllocator& );

	virtual void* TMalloc(int32 nSize);
	virtual void TFree(void* ptr);

	virtual void SetMemoryInfo(const MemoryHead& info);
	virtual const MemoryHead& GetMemoryInfo();
	virtual void Init(int32 nSize);
	virtual void CollectMemory(){}
	virtual bool IsValidMemory(void* ptr);
	virtual int32 GetAllocInfo(void* ptr);
	virtual uint64 GetMemoryUsage() { return m_nMemUsage; }

	virtual int32 GetType() { return eAlloc_Pool_SHM; }

private:
	StaticMemoryPoolMT	m_memoryPool;

	uint32				m_nObjSize;
	uint32				m_nCellSize;
	MemoryHead			m_Info;
	uint64				m_nMemUsage;
};

