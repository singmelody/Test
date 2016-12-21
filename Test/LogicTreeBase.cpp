#include "StdAfx.h"
#include "LogicTreeBase.h"
#include "MyLog.h"

LogicTreeBase::LogicTreeBase(void)
{
}


LogicTreeBase::~LogicTreeBase(void)
{
}

void LogicTreeBase::DoProcess()
{
#ifdef _DEBUG
	ProcessSelf();
	ProcessChild();
#else
	try
	{
		ProcessSelf();
		ProcessChild();
	}
	catch (...)
	{
		MyLog::error("Fatal Logic Error:Logic ID[%d]", m_nLogicID);
		SetFinish();
		return;
	}
#endif
}

void LogicTreeBase::ProcessSelf()
{
	if(!m_bActive || m_bSelfFinish || IsFinish() )
		return;

	UpdateInput();
	PreProcess();
	bool bRes = Process();
	if( true == bRes)
	{
		m_bSelfFinish = true;
		m_bActive = false;
	}
}

void LogicTreeBase::ProcessChild()
{

}
