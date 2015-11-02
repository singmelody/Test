#pragma once

#include "BaseType.h"

enum
{
	eAlloc_Base,
	eAlloc_DL,
	eAlloc_Pool,
	eAlloc_Pool_Ex,
	eAlloc_Pool_SHM,
	eAlloc_Count
};

struct MemoryHead
{
	int32 MemInfo;
	int32 AllocInfo;

	bool operator == (MemoryHead& rhs)
	{
		return AllocInfo == rhs.AllocInfo;
	}

	bool operator != (MemoryHead& rhs)
	{
		return AllocInfo != rhs.AllocInfo;
	}

	MemoryHead& operator=(MemoryHead& rhs)
	{
		MemInfo = rhs.MemInfo;
		AllocInfo = rhs.AllocInfo;
		return *this;
	}
};

class Allocator
{
public:
	Allocator();
	virtual ~Allocator();

	virtual void Init(int32 nSize) = 0;
	virtual void* TMalloc(int32 nSize) = 0;
	virtual void TFree(void* ptr) = 0;

	virtual void SetMemoryInfo(const MemoryHead& head) = 0;
	const MemoryHead& GetMemoryHead() { return m_head; }

	virtual int32 GetType() { return m_nAllocType; }
protected:
	MemoryHead  m_head;
	int			m_nAllocType;
};

