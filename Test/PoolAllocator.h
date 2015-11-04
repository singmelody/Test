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
	~PoolAllocator();
};

