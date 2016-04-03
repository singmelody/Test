#include "StdAfx.h"
#include "ModuleBase.h"
#include "Log.h"

ModuleBase::ModuleBase(void)
{
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
