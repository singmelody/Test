#include "StdAfx.h"
#include "DBAFuncManager.h"
#include "DBAServer.h"

DBAFuncManager::DBAFuncManager(void)
	: ModuleFuncManager(DBAServer::Instance())
	, DBASrv(DBAServer::Instance())
{
}


DBAFuncManager::~DBAFuncManager(void)
{
}

Char* DBAFuncManager::GetBuffer()
{
	static char inBuffer[2048];
	memset( inBuffer, 0, sizeof(inBuffer));
	return inBuffer;
}
