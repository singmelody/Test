#pragma once
#include "BaseType.h"
#include "Singleton.h"
#include "DBAAvatarManager.h"
#include <assert.h>
#include "ODBCInterface.h"
#include "Thread.h"
#include "MyListNode.h"

class DBTask;
class DBConnectionManager;
class ODBCThread;
class DBThreadPool;

struct TaskQueueWithLevel
{
public:
	void PushTask(DBTask* pTask, DBATaskLevel taskLevel);
	DBTask* PopTask();
	uint32 GetTaskCount();

	inline uint32 GetTaskLen(int32 nTaskLevel)
	{
		assert( nTaskLevel < eDBATL_MAX && nTaskLevel >= eDBATL_0);
		return m_tasks[nTaskLevel].size();
	}

	inline uint32 GetAllTaskLen()
	{
		uint32 nTotal = 0;
		for (int32 i = eDBATL_0; i < eDBATL_MAX; ++i)
			nTotal += GetTaskLen(i);

		return nTotal;
	}

protected:
	typedef MyListNode<DBTask*>		DBListNode;
	typedef std::list<DBListNode*>	DBTaskList;
	DBTaskList		m_tasks[eDBATL_MAX];
	Mutex			m_mutex;
};

class ODBCThread : public MyThread
{
public:
	ODBCThread(DBThreadPool& threadPool, const std::string& dbName, const std::string& user, const std::string& pwd);
	~ODBCThread();

	ODBCInterface& GetODBCInterface() { return m_Interface; }
	TaskQueueWithLevel& GetTaskQueue() { return m_taskQueue; }
protected:
	void Thread_Update();

	DBThreadPool&		m_threadPool;
	ODBCInterface		m_Interface;
	int64				m_deltaTime;
	TaskQueueWithLevel	m_taskQueue;
};

class DBThreadPool : public Singleton<DBThreadPool>
{
public:
	DBThreadPool(void);
	~DBThreadPool(void);

	bool Open(uint32 numOfThread, const std::string& dbname, const std::string& user,const std::string& pwd);
	void PrintDBStaticAndReset();
	void Close();

	bool ScheduleTaks( DBTask* pTask, DBATaskLevel taskLevel = eDBATL_1);
	void SetDBInterface(DBConnectionManager* db) { m_pDBConMgr = db; }

	TaskQueueWithLevel& GetTaskQueue() { return m_taskQueue; }

	bool IsDBStaticEnable() const { return DBAConfig_Enable_Db_Statics; }
	uint32 GetTaskLen(int32 nTaskLv);
	uint32 GetAllTaskLen();
	void FillConfig();
protected:
	TaskQueueWithLevel			m_taskQueue;
	std::vector<ODBCThread*>	m_vThreads;

	bool						m_bRun;
	DBConnectionManager*		m_pDBConMgr;
	DB_DS						m_sqlStatis;
	bool						DBAConfig_Enable_Db_Statics;
};

