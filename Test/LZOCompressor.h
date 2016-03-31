#pragma once

#include "minilzo.h"
#include "PoolAllocatorEx.h"
#include "Factory.h"

#if defined(_DEBUG) && defined(_WIN32)
#define DEBUG_LZO_RATIO
#endif

#define HEAP_ALLOC( var, size) \
	lzo_align_t __LZO_MMODEL var [ ( (size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t)]

class LZOCompressor
{
	DECLARE_FACTORY_ARG0( LZOCompressor, -1, new PoolAllocatorEx);
public:
	LZOCompressor(void);
	~LZOCompressor(void);

	static bool InitLzo();

	bool Compress( char* src, uint32 srcLen, char* dst, uint32 & destLen);
	bool DeCompress( char* src, uint32 nSrcLen, char* dst, uint32& destLen);

protected:
	lzo_align_t __LZO_MMODEL wrkmem [ (LZO1X_MEM_COMPRESS) + (sizeof(lzo_align_t) - 1) / sizeof(lzo_align_t)];

private:
#ifdef DEBUG_LZO_RATIO
	uint64	m_times;
	uint64	m_byteRaw;
	uint64	m_byteRawPrint;
	uint64	m_byteCompressed;
#endif
};

