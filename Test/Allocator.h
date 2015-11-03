#pragma once

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

	virtual void* TMalloc(int32 nSize) = 0;
};

