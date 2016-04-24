#pragma once

const uint32 HEADER_SIZE_FOR_SHARED_MEMORY = sizeof(uint8);

class StaticMemoryPool
{
public:
	StaticMemoryPool(void);
	~StaticMemoryPool(void);
};

