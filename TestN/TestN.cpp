// TestN.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "NodeServer.h"

void MyExit()
{
	NodeServer::Instance().Exit();
}

int _tmain(int argc, _TCHAR* argv[])
{

	CrashHandler::InitGlobalHandler("TestN");

	if(NodeServer::Instance().Init( argc, argv))
	{
		atexit(MyExit);
		NodeServer::Instance().Start();
	}

	return 0;
}

