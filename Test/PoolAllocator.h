#pragma once

#include "Allocator.h"

class PoolAllocator : public Allocator
{
public:
	PoolAllocator();
	~PoolAllocator();
};

