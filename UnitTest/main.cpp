#include "stdafx.h"
#include "UtilID.h"
#include <assert.h>
#include "DBInterface.h"
#include "LoadTemplateManager.h"
#include "CommonConfig.h"
#include "MyLog.h"
#include "Log.h"
#include "LogThread.h"
// unit test
#include "TestODBCManager.h"

int _tmain(int argc, _TCHAR* argv[])
{
	g_pLog = new Log();
	MyLog::Create( "log.txt");
	LogThread::Instance().Start();

	CommonConfig::DBType	= eDBType_DBC;
	CommonConfig::DBCName	= "testdb";
	CommonConfig::DBCUser	= "root";
	CommonConfig::DBCPwd= "123456";
	
	unsigned int tt = UtilID::CreateFromString("wangyanbin");
	assert(tt == 65398277);
	
	FACTORY_NEWOBJ(DBRow);
	FACTORY_NEWOBJ(DBRow);
	
	LoadTemplateManager templateMgr;
	
	templateMgr.AddTemplate("TestODBCManager", TestODBCManager::Instance());
	templateMgr.Load("NodeServer");

	MyLog::message("I am here waiting for u");

	

	return 0;
}

