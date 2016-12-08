// TestG.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CrashHandler.h"
#include "GateServer.h"

void MyExit()
{
	GateServer::Instance().Exit();
}

int _tmain(int argc, _TCHAR* argv[])
{
	CrashHandler::InitGlobalHandler("GTest");

	if(GateServer::Instance().Init( argc, argv))
	{
		atexit(MyExit);
		GateServer::Instance().Start();
	}
	return 0;
}

