#pragma once

#include "Allocator.h"

struct MemoryCell
{
	MemoryHead		info;
	MemoryCell*		_next;
};

class PoolAllocator : public Allocator
{
public:
	PoolAllocator();
	PoolAllocator(int32 nSize);
	virtual ~PoolAllocator();

	void SetMemoryInfo(const MemoryHead& head);
	virtual void Init(int32 nSize);

protected:
	void	PushMemory();
	void*	PopMemory();


	int32		m_UseCount;
	int32		m_count;

	uint64		m_MemUsage;
	int32		m_ObjSize;
	int32		m_CellSize;
	MemoryCell* m_cell;
};

