#include "StdAfx.h"
#include "Time.h"
#include <windows.h>

Time::Time(void)
{
	m_milliSec = 0;
}

Time::Time(uint64 millisec)
{
	m_milliSec = millisec;
}

Time::~Time(void)
{
}

Time Time::CurrentTime()
{
	const static uint64 FILETIME_OFFSET = 0x19db1ded53e8000;

	FILETIME fileTime;
	::GetSystemTimeAsFileTime(&fileTime);

	ULARGE_INTEGER largeInt;
	memcpy( &largeInt, &fileTime, sizeof(largeInt));
	return (largeInt.QuadPart - FILETIME_OFFSET)/10000;
}
