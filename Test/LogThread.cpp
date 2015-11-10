#include "StdAfx.h"
#include "LogThread.h"
#include "Time.h"
#include "FunctionBase.h"
#include "MyLog.h"
#include "Log.h"
#include <assert.h>

#define LOG_FLUSH_TIME 30000

LogThread::LogThread(void) 
	: Thread("Log Flush"), m_pUpdateFunc(NULL),m_bExit(false)
{
	m_frameTime = 0;
	m_logTime = 0;
	m_lastTime = Time::CurrentTime().MilliSecond();
	Init(new Function_Arg0<LogThread>( this, &LogThread::Thread_Update));
}


LogThread::~LogThread(void)
{
	SAFE_DELETE(m_pUpdateFunc);
}


void LogThread::Thread_Update()
{
	uint64 nowTime = Time::CurrentTime().MilliSecond();
	m_frameTime = (int32)(nowTime - m_lastTime);
	m_logTime += m_frameTime;
	if(m_logTime > LOG_FLUSH_TIME )
	{
		LogSystem_Flush();
		m_logTime = 0;
	}
	GSleep(1000);
}

void LogThread::Init(FunctionBase_Arg0* pFunc)
{
	assert(!m_pUpdateFunc);
	m_pUpdateFunc = pFunc;
}

void LogThread::Run()
{
	if(!m_pUpdateFunc)
	{
		MyLog::message("LogThread[%s] Start Error No Update Func", m_strName.c_str());
		return;
	}

	while(!m_bExit)
	{
		(*m_pUpdateFunc)();
	}
}

void LogThread::Stop()
{
	Exit();
	Wait();
	LogSystem_Flush();
}
