#include "StdAfx.h"
#include "DBAServer.h"
#include "GameUtil.h"
#include "DBAConfig.h"
#include "DBAAvatarManager.h"
#include "MyLog.h"
#include "Thread.h"
#include "ParamPool.h"
#include "GameUtil.h"
#include "DBThreadPool.h"
#include "GameUtil.h"
#include "WatchDog.h"
#include "LoadTemplateManager.h"
#include "DBAConfig.h"
#include "ConfigManager.h"

DBAServer::DBAServer(void)
{
	m_pInterface = NULL;
	DBAConfig_delta_time_of_db_statics = 1000;
	m_bUseConnectionThread = true;
}


DBAServer::~DBAServer(void)
{
}

bool DBAServer::Init(int32 argc, char* argv[])
{
	ParamDefManager::Instance().m_bLockAllocator = true;
	DBAConfig::DBAUseSHM = GameUtil::GetCommandOpt("--UseSHM", argc, argv);

	GetSHMMgr().Init("DBA", true, DBAConfig::DBAUseSHM, false, false);

	ParamDefManager::Instance().SetListener(&GetSHMMgr());
	ParamDefManager::Instance().IsSHMEnable(DBAConfig::DBAUseSHM);
	ParamDefManager::Instance().IsNodeServer(false);

	if(!DBABase::Init( argc, argv))
	{
		MyLog::error("DBAServer::Init Failed DBABase::InitModule()");
		return false;
	}

	if(DBAConfig::DBAUseSHM)
	{
		// 
	}

	MyLog::message("Begin LoadModue[DBAServer]");
	Templates.Load("DBAServer");
	MyLog::message("End LoadModule[DBAServer]");

	ParamSet set;
	{
		DBAConfig::FillConfig();

		DBAAvatarManagerEx::Instance().FillConfig();
		DBThreadPool::Instance().FillConfig();

		int32 nTime = 1000;
		ConfigManager::GetConfigValue("DBAConfig", "delta_time_of_db_statics", nTime);
		DBAConfig_delta_time_of_db_statics = nTime;
		if(DBAConfig_delta_time_of_db_statics < 1000)
			DBAConfig_delta_time_of_db_statics = 1000;

		MyLog::message("delta_time_of_db_statics = [%d]!", DBAConfig_delta_time_of_db_statics);

	}
}

bool DBAServer::Start()
{
	WatchDog::Instance().RegWatchDog( GAME_THREAD_WATCHDOG_ID, "DBA-Main-Thread", DBA_FRAME_TIME * 10);
	return StartMainLoop(DBA_FRAME_TIME);
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
		//DBAGuidManagerEx::Instance().SyncAll2DBAndClearAll();
		MyLog::message("DBAServer::Exit() GUID SyncAll2DB Finished...");
	}

	// wait for task list empty
	{
		MyLog::message("DBAServer::Exit() Waiting TaskQueue Empty...");
		while (DBThreadPool::Instance().GetAllTaskLen() > 0)
		{
			GameUtil::Sleep(50);
		}
		MyLog::message("DBAServer::Exit() TaskQueue Empty...");
	}

	// wait for avatar list empty
	{
		MyLog::message("DBAServer::Exit() Waiting AvatarMap Empty...");
		while (DBAAvatarManagerEx::Instance().GetAvatarCount() > 0)
		{
			DBAAvatarManagerEx::Instance().ProcExitedAvatars();
			GameUtil::Sleep(5);
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

void DBAServer::Send2World(PacketBase& pkt)
{

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