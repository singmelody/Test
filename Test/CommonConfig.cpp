#include "StdAfx.h"
#include "CommonConfig.h"
#include "ConfigManager.h"
#include "MyLog.h"

int32 CommonConfig::IsDevMode = 0;
int32 CommonConfig::DBType = eDBType_DBC;

std::string CommonConfig::DBCName = "";
std::string CommonConfig::DBCUser = "";
std::string CommonConfig::DBCPwd = "";
std::string CommonConfig::DBLocalFile;
std::string CommonConfig::DBTempFile;

int32 CommonConfig::DBConnection;

int32 CommonConfig::FlagFuncPerformance;
int32 CommonConfig::FlagPktPerformance;

CommonConfig::CommonConfig(void)
{
}


CommonConfig::~CommonConfig(void)
{
}

void CommonConfig::FillConfig()
{
	ConfigSection* pSection = ConfigManager::GetConfigSection("CommonConfig");

	if(!pSection)
		return;

	ConfigManager::GetConfigValue( "CommonConfig", "IsDevMode", IsDevMode, false);
	MyLog::message( "IsDevMode=[%s]", IsDevMode ? "True" : "False");

	pSection->GetConfigValue( "DBCType", DBType);
	pSection->GetConfigValue( "DBCName", DBCName);
	pSection->GetConfigValue( "DBCUser", DBCUser);
	pSection->GetConfigValue( "DBCPassword", DBCPwd);

	pSection->GetConfigValue( "DBTemplateFile", DBTempFile);
	MyLog::message( "DBTempFile=[%s]", DBTempFile.c_str());
	CONSOLE_CYAN

	switch(DBType)
	{
	case eDBType_DBC:
		MyLog::message("Database = DBC Mysql");
		break;
	case eDBType_Local:
		MyLog::message("Database = Local Sqlite");
		break;
	case eDBType_LocalX:
		MyLog::message("Database = Local Sqlite Encrypted");
		break;
	case eDBType_Temp:
		MyLog::message("Database = Temp Sqlite");
		break;
	}
}

