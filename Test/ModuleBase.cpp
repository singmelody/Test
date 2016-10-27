#include "StdAfx.h"
#include "ModuleBase.h"
#include "Log.h"
#include "ServerManager.h"
#include "MyLoadTemplateManager.h"
#include "GameUtil.h"
#include "MyLog.h"
#include "LogThread.h"
#include "ConfigManager.h"
#include "ParamDefManager.h"
#include "ServerConfig.h"
#include <sstream>

#define DOG 1

ModuleBase::ModuleBase(SrvType nSrvType)
	: m_nSrvType( nSrvType), Servers(ServerManager::Instance()), Templates(MyLoadTemplateManager::Instance())
	, m_bDebug(false)
{
	m_nRunState = eRS_Connecting;
	m_nSrvID = -1;
	m_strModuelName = GetSrvTitle( m_nSrvType );

	m_nFrameInterval = 50;
	m_bExitLoop = false;

	m_nLastFrameTicks = 0;
	m_nLastTime = 0;

	m_pModuleDogPool = NULL;

	Servers.SetSrvType( nSrvType );
}


ModuleBase::~ModuleBase(void)
{
}

bool ModuleBase::Init(int32 nArgc, char* argv[])
{
	InitLog( nArgc, argv);

	MyLog::message("Begin ModuleBase::Init ModuleName[%s], Version[%s]", m_strModuelName.c_str(), "0.0.1");

	m_bDebug = GameUtil::GetCommandOpt("--Debug", nArgc, argv);
	MyLog::message("IsDebug=[%s]", IsDebug() ? "True" : "False");

	std::stringstream strArgs;
	std::string strConfig;

	for (int32 i = 0; i < nArgc; ++i)
	{
		char* sArg = argv[i];
		strArgs<<" "<<sArg;
		if( strcmp( sArg, "-Path") == 0)
		{
			MyPath::SetMainPath( argv[i+1] );
			MyLog::message("Path:%s", argv[i+1]);
		}
		else if( (strcmp( sArg, "-StartConfig") == 0) || (strcmp( sArg, "-Config") == 0))
		{
			strConfig = argv[i+1];
		}
		else if( strcmp( sArg, "-Section") == 0)
		{
			std::string strSection = argv[i+1];
			ConfigManager::SetCustomSection( strSection );
		}
	}

	MyLog::message("Start Args[%s]", strArgs.str().c_str());

	// set config file working directory
	std::string strPath = MyPath::MainPath();
	strPath += "Data/Config/";
	ConfigManager::SetMainPath(strPath);

	if (strConfig.length() > 0)
	{
		std::string strPath = ConfigManager::MainPath() + strConfig;
		ConfigManager::LoadConfig( strPath.c_str(), true);
	}

	// load argv argument
	ConfigManager::LoadStartConfig( nArgc, argv);

	// load log config
	MyLog::FillConfig();

	// load srv config
	ServerConfig::FillConfig();

	// set console width & height
	Console::SetBufferSize( ServerConfig::nConsoleW, ServerConfig::nConsoleH);

	// init server db
	std::string strDBFile;
	strDBFile = MyPath::MainPath();
	strDBFile += "Data/Server.db";
	DBLoader::SetDBFile( strDBFile.c_str() );

	// load argument define
	MyLog::message("Begin LoadModule[BaseServer]");
	Templates.AddTemplate( "ParamDefManager", ParamDefManager::Instance());
	Templates.Load("BaseServer");
	MyLog::message("End LoadModule[BaseServer]");

	CreateDogPool();
	OnConfigLoaded();

	AppendLoadTemplate();

#if DOG
	WatchDog::Instance().Start();
#endif

	MyLog::message("End ModuleBase::Init() ModuleName[%s]", m_strModuelName.c_str());

	return true;
}

void ModuleBase::Exit()
{
#if DOG
	WatchDog::Instance().Stop();
#endif

	MemoryLog::Shutdown();
	LogThread::Instance().Stop();
	SAFE_DELETE(g_pLog);
}

void ModuleBase::InitLog(int32 nArgc, char* argv[])
{
	assert(argv);

	std::string strSection;

	if(!GameUtil::GetCommandOpt("-Section", strSection, nArgc, argv))
		strSection = "dummy";

	std::string logName = strSection;

	g_pLog = new Log();
	MyLog::Create( logName.c_str() );
	LogThread::Instance().Start();

}
