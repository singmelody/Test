#pragma once
#include <string>
#include <set>
#include "Time.h"

class TokenParser
{
public:
	static int32 GetToken( const char* str_cur_pos, char* token, int32 nToken_Max, char separator = ' ');
};

#define  MY_GET_TOKEN_RETURN( cur_str_ptr, token, token_max, separator, ret) \
{ \
	int32 nStr_nOffset = TokenParser::GetToken( cur_str_ptr, token, token_max, separator); \
	if(nStr_nOffset == 0) \
		return ret; \
	cur_str_ptr += nStr_nOffset; \
};

class MyPath
{
public:
	MyPath()
	{
		m_MainPath = "./";
	}

	~MyPath(){}

	static const char* MainPath() { return m_MainPath.c_str(); }
	static void SetMainPath(const char* path);
	static void GetDirFiles( const std::string& path, std::set<std::string>& files);
protected:
	static std::string m_MainPath;
};

class GameUtil
{
public:
	GameUtil(void);
	~GameUtil(void);

	static bool GetCommandOpt( const std::string& strOptName, std::string& strOptValue, int32 nArgc, char* argv[]);
	static bool GetCommandOpt( const std::string& strOptName, int32 nArgc, char* argv[]);

	static std::string TrimStr( const std::string& str, const std::string strTrim);
	static void	Sleep(const Time& time);
};

class ParamString
{
public:
	ParamString(){}
	~ParamString(){}

	static bool GetParamString(char* &pCurPos, std::string& sParam, char cFlag = ',')
	{
		sParam = "";
		char token[512];
		MY_GET_TOKEN_RETURN( pCurPos, token, 512, cFlag, false);
		sParam = token;
		return true;
	}

	static bool GetParamValue(char* &pCurPos, std::string& sName, std::string& sValue, char cFlag = ',')
	{
		sName = "";
		sValue = "";

		char token[512];
		MY_GET_TOKEN_RETURN( pCurPos, token, 512, cFlag, false);
		sName = token;
		MY_GET_TOKEN_RETURN( pCurPos, token, 512, cFlag, false);
		sValue = token;
		return true;
	}
};

