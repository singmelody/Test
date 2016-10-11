#pragma once

#include "BaseType.h"
#include "MyMutex.h"

const uint32 HEADER_SIZE_FOR_SHARED_MEMORY = sizeof(uint8);

struct MemBlockView_Raw
{
	inline void IsInUse(bool b) { m_flag = b ? 1 : 0; }
	inline bool IsInUse() const { return m_flag != 0; }

	uint8				m_flag;
	MemBlockView_Raw*	m_pNext;
};

class StaticMemoryPool
{
public:
	StaticMemoryPool(void* pMemoryBlock, uint32 nUnitSize, uint32 nUnitCnt, uint32 nZeroFillSize);
	~StaticMemoryPool(void);

	virtual void* Allocate(uint32 nObjSize);
	virtual void Free(void* ptr);

protected:
	void*				m_pMemoryBlock;
	const uint32		m_nUnitSize;
	const uint32		m_nTailZeroFillSize;
	const uint32		m_nUnitCount;

	MemBlockView_Raw*	m_pFreeBlockList;
};


class StaticMemoryPoolMT : public StaticMemoryPool
{
public:
	StaticMemoryPoolMT(void* pMemoryBlock, uint32 nUnitSize, uint32 nUnitCnt, uint32 nZeroFillSize)
		: StaticMemoryPool(pMemoryBlock, nUnitSize, nUnitCnt, nZeroFillSize)
	{

	}

	virtual void* Alloc(uint32 nObjSize);
	virtual void Free(void* ptr);

protected:
	StaticMemoryPoolMT(const StaticMemoryPoolMT& );
	StaticMemoryPoolMT& operator=(const StaticMemoryPoolMT&);
	Mutex	m_lock;
};