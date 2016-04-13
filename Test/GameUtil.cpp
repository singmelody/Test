#include "StdAfx.h"
#include "GameUtil.h"


GameUtil::GameUtil(void)
{
}


GameUtil::~GameUtil(void)
{
}

bool GameUtil::GetCommandOpt(const std::string& strOptName, std::string& strOptValue, int32 nArgc, char* argv[])
{
	for (int32 i = 0; i < nArgc-1; ++i)
	{
		if( strOptName != argv[i])
			continue;

		strOptValue = argv[i];
		return true;
	}

	return false;
}

bool GameUtil::GetCommandOpt(const std::string& strOptName, int32 nArgc, char* argv[])
{
	for (int32 i = 0; i < nArgc-1; ++i)
	{
		if( strOptName != argv[i])
			continue;

		return true;
	}

	return false;
}
