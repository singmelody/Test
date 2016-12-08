#include "StdAfx.h"
#include "CrashHandler.h"
#include <string>
#include <assert.h>
#include <stdio.h>
#include <DbgHelp.h>
#include <ShellAPI.h>
#include <ShlObj.h>
#include <excpt.h>

static std::string s_strAppName;
static long WINAPI sMyUnhandleExceptionFilter(struct _EXCEPTION_POINTERS* pExceptionInfo)
{
	CrashHandler::DumpFile( pExceptionInfo, s_strAppName.c_str());
	return EXCEPTION_EXECUTE_HANDLER;
}

CrashHandler::CrashHandler(void)
{
}


CrashHandler::~CrashHandler(void)
{
}

int32 CrashHandler::DumpFile(MY_EXCEPTION_POINTERS* pExceptionPointers, const char* sAppName)
{
	char szFileName[MAX_PATH];
	uint32 dwBufferSize = MAX_PATH;
	SYSTEMTIME stLocalTime;
	::GetLocalTime(&stLocalTime);

	sprintf( szFileName, "%s-%04d%02d%02d-%02d%02d%02d.dmp", sAppName, 
		stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
		stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);

	MINIDUMP_EXCEPTION_INFORMATION ExpParam;

	ExpParam.ThreadId = ::GetCurrentThreadId();
	ExpParam.ExceptionPointers = pExceptionPointers;
	ExpParam.ClientPointers = NULL;

	HANDLE hDumpFile = ::CreateFile( szFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	MINIDUMP_TYPE mDumpValue = (MINIDUMP_TYPE)(
		MiniDumpNormal |
		MiniDumpWithIndirectlyReferencedMemory |
		MiniDumpWithDataSegs |
		MiniDumpWithFullMemory
		);

	bool bMiniDumpSucessful = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(),
		hDumpFile, mDumpValue, &ExpParam, NULL, NULL);

	::CloseHandle(hDumpFile);
	return bMiniDumpSucessful;
}

void CrashHandler::InitGlobalHandler(const char* appName)
{
	assert(appName);
	assert( s_strAppName.size() == 0 );

	s_strAppName = std::string(appName);

	SetUnhandledExceptionFilter(sMyUnhandleExceptionFilter);
}
