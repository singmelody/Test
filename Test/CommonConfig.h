#pragma once
#include <string>

enum DBType
{
	eDBType_DBC,
	eDBType_Local,
	eDBType_LocalX,
	eDBType_Temp
};

class CommonConfig
{
public:
	CommonConfig(void);
	virtual ~CommonConfig(void);

	static void FillConfig();

	static int32		DBType;
	static std::string	DBCName;
	static std::string	DBCUser;
	static std::string	DBCPwd;
};

