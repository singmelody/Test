#include "StdAfx.h"
#include "ModuleBase.h"
#include "Log.h"
#include "ServerManager.h"
#include "MyLoadTemplateManager.h"

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
	assert(argv);

	std::string strSection;

	if(!GetCommandOption("-Section", strSection, argc, argv))
		strSection = "dummy";

	std::string logName = strSection;

	g_pLog = new Log();
	MyLog::Create( logName.c_str() );
	LogThread::Instance().Start();
}
