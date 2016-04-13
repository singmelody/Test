#pragma once
#include <string>

class GameUtil
{
public:
	GameUtil(void);
	~GameUtil(void);

	static bool GetCommandOpt( const std::string& strOptName, std::string& strOptValue, int32 nArgc, char* argv[]);
	static bool GetCommandOpt( const std::string& strOptName, int32 nArgc, char* argv[]);
};

