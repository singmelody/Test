#pragma once

#include "Allocator.h"

class DLAllocator : public Allocator
{
public:
	DLAllocator(void);
	~DLAllocator(void);

	virtual void* TMalloc(int32 nSize);
	virtual void TFree(void* ptr);

	void SetMemoryInfo(const MemoryHead& info);
	const MemoryHead& GetMemoryInfo();

	void Init(int32 nSize);
	bool IsValidMemory(void* ptr);
	int32 GetAllocInfo(void* ptr);
	uint64 GetMemoryUsage();
	void CollectMemory();
	int32 GetType() { return eAlloc_DL; }

protected:
	int32 m_size;
	MemoryHead m_info;
};

