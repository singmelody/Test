#include "StdAfx.h"
#include "DataSerialization.h"

namespace DS
{
#ifdef PROFILE_VAR_INT
#if defined (_WIN32) && !defined(_WIN64)
	static volatile long g_nCount[profile_count] = {0};
#define ATOMIC_ADD_FUNCTION GInterLockedAdd
#else
	static volatile int64 g_nCount[profile_count] = {0};
#define ATOMIC_ADD_FUNCTION GInterLockedAdd64
#endif

	bool SerializeVarInt(uint32& value, void* buffer, uint32& offset, bool bToBuff, size_t bufferSize /*= 65535*/)
	{
		
	}

}