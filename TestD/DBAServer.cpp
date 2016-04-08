#include "StdAfx.h"
#include "DBAServer.h"


DBAServer::DBAServer(void)
{
}


DBAServer::~DBAServer(void)
{
}

bool DBAServer::Init(int32 argc, int32 argv[])
{
	ParamDefManager::Instance().m_bLockAllocator = true;
	DBAConfig::DBAUseShm = GetCommandOpt("--UseSHM", argc, argv);

	GetSHMMgr().Init("DBA", true, DBAConfig::DBAUseShm, false, false);
}

bool DBAServer::Start()
{

}

void DBAServer::Exit()
{

}
