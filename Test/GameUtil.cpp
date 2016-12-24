#include "StdAfx.h"
#include "GameUtil.h"
#include "Thread.h"
#include <Windows.h>
#include <string.h>

int32 TokenParser::GetToken(const char* str_cur_pos, char* token, int32 nToken_Max, char separator /*= ' '*/)
{
	*token = '\0';

	char* tData = const_cast<char*>(str_cur_pos);
	char* c_begin = tData;

	int32 nIdx_Offset = 0;
	while (c_begin)
	{
		if(*c_begin == '\0')
			return 0;

		if(*c_begin != separator)
			break;

		++nIdx_Offset;
		++c_begin;
	}

	char* c_end = strchr( c_begin, separator);
	if(c_end)
		*c_end = '\0';

	int32 nLen = (int32)strlen(c_begin);
	char* ret = strncpy(token, c_begin, nLen);
	token[nLen] = '\0';
	if(c_end)
		*c_end = separator;

	return nIdx_Offset + (int32)strlen(token);
}


std::string MyPath::m_MainPath = "";

void MyPath::SetMainPath(const char* path)
{
	if(!path)
		return;

	m_MainPath = path;
}


void MyPath::GetDirFiles(const std::string& path, std::set<std::string>& files)
{
	files.clear();

	WIN32_FIND_DATA fd;
	HANDLE h = FindFirstFile( (path + "/*.*").c_str(), &fd);
	if( h == INVALID_HANDLE_VALUE)
		return;

	do 
	{
		if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			continue;

		files.insert( std::string(fd.cFileName) );
	} while (FindNextFile( h, &fd));
}


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

		strOptValue = argv[i+1];
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

std::string GameUtil::TrimStr(const std::string& str, const std::string strTrim)
{
	std::string::size_type pStart = str.find_first_not_of(strTrim);
	if( pStart == std::string::npos )
		return std::string();

	std::string strRet = str.substr( pStart );
	std::string::size_type pEnd = str.find_last_not_of(strTrim);
	if( pEnd == std::string::npos )
		return strRet;

	return strRet.substr( 0, pEnd + 1);
}

void GameUtil::Sleep(const Time& time)
{
	MyThread::Sleep(time.MilliSecond());
}

