#pragma once
#include "LoadTemplate.h"
#include "Singleton.h"

class TestODBCManager : public LoadTemplate, public Singleton<TestODBCManager>
{
protected:
	TestODBCManager();
	virtual ~TestODBCManager();
	friend class Singleton<TestODBCManager>;

	virtual bool LoadDataFromDB(DBInterface* ptr);
};

