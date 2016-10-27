#include "StdAfx.h"
#include "ServerConfig.h"
#include "ConfigManager.h"
#include "MyLog.h"
#include <string>

int16 ServerConfig::nConsoleH = 800;
int16 ServerConfig::nConsoleW = 600;

int32 ServerConfig::nNodeTickTime = 40;
int32 ServerConfig::nNormalTickTime = 150;

std::string ServerConfig::sMotherIP = "127.0.0.1";
int32 ServerConfig::nMotherPort = 1014;

ServerConfig::ServerConfig(void)
{
}


ServerConfig::~ServerConfig(void)
{
}


void ServerConfig::FillConfig()
{
	CommonConfig::FillConfig();

	ConfigManager::GetConfigValue( "CommonConfig", "ConsoleW", nConsoleW);
	ConfigManager::GetConfigValue( "CommonConfig", "ConsoleH", nConsoleH);

	{
		std::string strTime;
		ConfigManager::GetConfigValue( "CommonConfig", "SrvStartTime", strTime);

		int32 Y,M,D,h,m,s;
		if( sscanf( strTime.c_str(), "%d-%d-%d %d:%d:%d", &Y, &M, &D, &h, &m, &s) != 6)
		{
			MyLog::error("Config Error, SrvStartTime=[%s]", strTime.c_str());
		}
		else
		{
				
		}
	}

	// NodeConfig
	ConfigManager::GetConfigValue( "NodeConfig" , "NodeTickTime", nNodeTickTime);
	MyLog::message("NodeTickTime[%d]!", nNodeTickTime);

	ConfigManager::GetConfigValue( "NodeConfig" , "NormalTickTime", nNormalTickTime);
	MyLog::message("NormalTickTime[%d]!", nNormalTickTime);

	ConfigManager::GetConfigValue("WorldConfig", "MontherIP", sMotherIP);
	MyLog::message("MotherIP[%s]", sMotherIP.c_str());

	ConfigManager::GetConfigValue("WorldConfig", "MontherPort", nMotherPort);
	MyLog::message("MotherPort[%d]", nMotherPort);
}