#include "StdAfx.h"
#include "DBThreadPool.h"
#include "DBTask.h"
#include <assert.h>
#include "DBThreadPool.h"
#include "FunctionBase.h"
#include "MyLog.h"


ODBCThread::ODBCThread(DBThreadPool& threadPool, const std::string& dbName, const std::string& user, const std::string& pwd)
	: MyThread(std::string("ODBC")), m_threadPool(threadPool)
{
	m_deltaTime = TimeManager::Instance().CurTime();
	bool bFlag = true;
	while (bFlag)
	{
		if(!m_Interface.ConnectDB( dbName.c_str(), user.c_str(), pwd.c_str()))
		{
			MyLog::error("DBA Open ODBC DB Failed [%s]", dbName.c_str());
			continue;
		}

		if(m_threadPool.IsDBStaticEnable())
			m_Interface.GetDBDS().EnableRecord();

		bFlag = false;
	}

	Init( new Function_Arg0<ODBCThread>( this, &ODBCThread::Thread_Update));
}

ODBCThread::~ODBCThread()
{

}



void ODBCThread::Thread_Update()
{

}

DBThreadPool::DBThreadPool(void)
{
}


DBThreadPool::~DBThreadPool(void)
{
}

bool DBThreadPool::Open(uint32 numOfThread, const std::string& dbname, const std::string& user,const std::string& pwd)
{
	assert(!m_bRun);
	m_bRun = true;
	for (uint32 i = 0; i < numOfThread; ++i)
	{
		ODBCThread* pThread = new ODBCThread( *this, dbname, user, pwd);
		if(!pThread)
			continue;

		pThread->Start();
		m_vThreads.push_back(pThread);
	}

	m_bRun = !m_vThreads.empty();
	return m_bRun;
}

void TaskQueueWithLevel::PushTask(DBTask* pTask, DBATaskLevel taskLevel)
{
	assert(pTask);
	assert( taskLevel < eDBATL_MAX && taskLevel >= eDBATL_0);
	AUTOLOCK(m_mutex);
	m_tasks[taskLevel].push_back(pTask->GetListNode());
}

void DBThreadPool::Close()
{
	m_bRun = false;

	for (auto itr = m_vThreads.begin(); itr != m_vThreads.end(); ++itr)
	{
		assert( *itr );
		(*itr)->Exit();
		(*itr)->Wait();
		SAFE_DELETE(*itr);
	}

	m_vThreads.clear();
}

bool DBThreadPool::ScheduleTaks(DBTask* pTask, int32 taskLevel /*= eDTL_1*/)
{
	assert( pTask );
	assert( taskLevel < eDBATL_MAX && taskLevel >= eDBATL_0);

	if(!m_bRun || !pTask)
		return false;

	if(pTask->GetAvatarDID() != 0)
	{
		uint32 nThreadID = uint32( uint64(pTask->GetAvatarDID()) % m_vThreads.size() );
		assert( nThreadID <= m_vThreads.size() );
		ODBCThread* pThread = m_vThreads[nThreadID];
		assert( pThread );

		pThread->GetTaskQueue().PushTask( pTask, taskLevel);
	}
	else
	{
		GetTaskQueue().PushTask( pTask, taskLevel);
	}

	return true;
}

