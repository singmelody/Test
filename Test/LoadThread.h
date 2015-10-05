#pragma once

#include <list>

class LoadInfo;

class LoadInfoList
{
protected:
	std::list<LoadInfo*> m_list;
};

class LoadThread
{
public:
	LoadThread(void);
	~LoadThread(void);
};

