// TestW.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "WorldServerNormal.h"

void MyExit()
{
	WorldServer::Instance().Exit();
}

int _tmain(int argc, char* argv[])
{
	if(!WorldServer::PreProcessShutdown( argc, argv))
		return 0;

	WorldServer* pSrv = new WorldServerNormal();
	if(!pSrv)
		return 0;

	Singleton<WorldServer>::SetInstance(pSrv);

	if( pSrv->Init( argc, argv))
	{
		atexit(MyExit);
		pSrv->Start();
	}

	return 0;
}

