#pragma once

#include <stdexcept>
#include <errno.h>

#ifdef _MSC_VER
#pragma warning( disable:4297 )
#endif

#define USE_DL_PREFIX
#define MSPACES				1

#if defined(_DEBUG) && !defined(DEBUG)
#define	DEBUG				1
#define FOOTERS				1
#else
#define INSECURE			1
#endif

#define USE_LOCKS			1

#define ABORT				throw std::runtime_error("dlmalloc abort wyb");
#define CORRUPTION_ERROR_ACTION(M)	TuCode_DLMallocCorruption(M)
#define USAGE_ERROR_ACTION(M, P)	TuCode_DLMallocUsageError(M,P);

extern "C" void TuCode_DLMallocCorruption( void* pState) {}
extern "C" void TuCode_DLMallocUsageError(void* pState, void* pointer){}


