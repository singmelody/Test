#pragma once

#include "BaseType.h"
#include <Windows.h>

typedef EXCEPTION_POINTERS MY_EXCEPTION_POINTERS;

class CrashHandler
{
public:
	CrashHandler(void);
	~CrashHandler(void);

	static int32 DumpFile(MY_EXCEPTION_POINTERS* pExceptionPointers, const char* sAppName);
	static void InitGlobalHandler(const char* appName);
};

