#include "StdAfx.h"
#include "DBAServer.h"
#include "GameUtil.h"
#include "DBAConfig.h"
#include "DBAAvatarManager.h"

DBAServer::DBAServer(void)
{
}


DBAServer::~DBAServer(void)
{
}

bool DBAServer::Init(int32 argc, int32 argv[])
{
	ParamDefManager::Instance().m_bLockAllocator = true;
	DBAConfig::DBAUseSHM = GameUtil::GetCommandOpt("--UseSHM", argc, argv);

	GetSHMMgr().Init("DBA", true, DBAConfig::DBAUseSHM, false, false);
}

bool DBAServer::Start()
{

}

void DBAServer::Exit()
{
	// avatar & avatar_commondata sync 2 db
	{
		MyLog::message("DBAServer::Exit() Waiting Avatar SyncAll2DB...");
		DBAAvatarManagerEx::Instance().SyncAll2DBAndClearAll();
		MyLog::message("DBAServer::Exit() Avatar SyncAll2DB Finished...");
	}


	// guid & guid_commondata sync 2 db
	{
		MyLog::message("DBAServer::Exit() Waiting GUID SyncAll2DB...");
		DBAGuidManagerEx::Instance().SyncAll2DBAndClearAll();
		MyLog::message("DBAServer::Exit() GUID SyncAll2DB Finished...");
	}

	// wait for task list empty
	{
		MyLog::message("DBAServer::Exit() Waiting TaskQueue Empty...");
		while (DBThreadPool::Instance().GetAllTaskLen() > 0)
		{
			GSleep(50);
		}
		MyLog::message("DBAServer::Exit() TaskQueue Empty...");
	}

	// wait for avatar list empty
	{
		MyLog::message("DBAServer::Exit() Waiting AvatarMap Empty...");
		while (DBAAvatarManagerEx::Instance().GetAvatarCount() > 0)
		{
			DBAAvatarManagerEx::Instance().ProcExitedAvatars();
			GSleep(5);
		}
		MyLog::message("DBAServer::Exit() AvatarMap Empty...");
	}

	// waiting  task thread close
	{
		MyLog::message("DBAServer::Exit() Waiting DBThread Close...");
		DBThreadPool::Instance().Close();
		MyLog::message("DBAServer::Exit() DBThread closed...");
	}

	DBABase::Exit();
}

void DBAServer::OnWorldDisconnect(ServerInfo* pInfo)
{
	MyLog::message( "World Srv Lose ! Will Exit!");
	StopMainLoop();
}

IParamPool2SqlProcessor* DBAServer::GetParam2SqlProcessor(int32 nParamIndex)
{
	return GetSHMMgr().GetParam2SqlProcessor(nParamIndex);
}

void DBAServer::ProcessPacket( PacketPrintMemory* )