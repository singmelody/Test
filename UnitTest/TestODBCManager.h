#pragma once
#include "LoadTemplate.h"
#include "Singleton.h"

class TestODBCManager : public LoadTemplate, public Singleton<TestODBCManager>
{
public:
	TestODBCManager();
	~TestODBCManager();
};

