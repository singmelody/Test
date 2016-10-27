#pragma once

#include "CommonConfig.h"
#include "BaseType.h"

#define USE_MEMORY_POOL_PACKET	1

#define	USE_SYS_NEW_PACKET		0

class ServerConfig : public CommonConfig
{
public:
	ServerConfig(void);
	~ServerConfig(void);

	static void FillConfig();

	static int16 nConsoleW;
	static int16 nConsoleH;

	static int32 nNodeTickTime;
	static int32 nNormalTickTime;

	static std::string sMotherIP;
	static int32 nMotherPort;
};
