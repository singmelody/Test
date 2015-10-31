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

	virtual void	Init(int32 nSize);
	virtual void*	TMalloc(int32 nSize);
	virtual void	TFree(void* ptr);

	void SetMemoryInfo(const MemoryHead& head);
	const MemoryHead& GetMemoryInfo();

	virtual int32 GetAllocInfo(void* ptr);

	virtual int32 GetMemorySize(void* ptr);
	virtual uint64 GetMemoryUsage() { return m_memUsage; }

	virtual void CollectMemory();
	virtual bool IsValidMemory(void* ptr);

	static int32 GetAllocInfoStatic(void* ptr);
	static int32 GetMemorySizeStatic(void* ptr);
	MemoryHead* GetMemoryInfoStaitc(void* ptr);
protected:
	void	PushMemory(void* ptr);
	void*	PopMemory();

	int32		m_useCount;
	int32		m_count;

	uint64		m_memUsage;
	int32		m_objSize;
	int32		m_cellSize;
	MemoryHead  m_info;
	MemoryCell* m_head;
};

