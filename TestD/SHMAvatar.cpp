#include "StdAfx.h"
#include "SHMAvatar.h"
#include "MyMutex.h"
#include "ParamPool2Sql_DBA.h"
#include "DBAServer.h"
#include "ParamPool2SqlProcessor.h"
#include "ParamPool.h"
#include "MyLog.h"
#include "TaskImpl.h"
#include "DBThreadPool.h"
#include "DBTask.h"

const uint32 _MAX_LOCKS = 32;
static Mutex s_lock[_MAX_LOCKS];
static Mutex& GetLock(int64 nDid) { return s_lock[uint32(nDid) % _MAX_LOCKS]; }

#define LOCK_THIS_AVATAR() AUTOLOCK(GetLock(m_avatarDID))

AvatarSHM::AvatarSHM()
	: m_avatarID(0)
	, m_avatarDID(0)
	, m_pPool(NULL)
	, m_state(eState_Init)
	, m_pendingTaskCount(0)
{

}

void AvatarSHM::Sync2DB(bool bExit /*= false*/)
{
	LOCK_THIS_AVATAR();

	if( m_state != eDAS_Gaming )
		return;

	IParamPool2SqlProcessor* pProcessor = DBAServer::Instance().GetParam2SqlProcessor(eParam_Avatar);
	assert(pProcessor);

	std::string strSql;
	if(pProcessor->GetSqlString(*m_pPool, strSql))
	{
		ScheduleSavingTask(strSql);

		ParamPoolExInfo_DBA_Avatar* pExInfo = (ParamPoolExInfo_DBA_Avatar*)m_pPool->SHMGetExtraMemory();
		assert(pExInfo);
		pExInfo->SetLoaded();
	}

	for (int32 i = eParam_Min + 1; i < eParam_Max; ++i)
	{
		Idx2CommonDataMap& theMap = m_arrMapIdx2CommonData[i];
		for (auto itr = theMap.begin(); itr != theMap.end(); ++itr)
		{
			ParamPool* pParamSet = itr->second;
			assert(pParamSet);

			pProcessor = DBAServer::Instance().GetParam2SqlProcessor(pParamSet->GetParamDefineIndex());
			assert(pProcessor);

			if(pProcessor->GetSqlString( *pParamSet, strSql))
			{
				ScheduleSavingTask(strSql);

				ParamPoolExInfo_DBA_CommonData* pExInfo = (ParamPoolExInfo_DBA_CommonData*) pParamSet->SHMGetExtraMemory();
				assert(pExInfo);
				pExInfo->SetLoaded();
				pExInfo->SetIndexChanged(false);
			}
		}
	}

	if(bExit)
		m_state = eDAS_Exiting;
}

bool AvatarSHM::ScheduleTask(DBTaskAvatar* pDBTask, DBATaskLevel taskLevel)
{
	assert(pDBTask);
	assert(taskLevel >= eDBATL_0 && taskLevel < eDBATL_MAX);

	if( m_state != eState_Gaming && m_state != eState_Loaiding )
	{
		MyLog::error("AvatarSHM::ScheduleTask Failed! Bad State=[%d] strSql=[%s]", m_state, pDBTask->GetSqlTemplate());
		FACTORY_DEL_DBTASK(pDBTask);
		return false;
	}

	pDBTask->nAvatarID = GetAvatarID();
	pDBTask->nAvatarDID = GetAvatarDID();

	pDBTask->SetScheduled();
	IncPendingTaskCount();

	if(!DBThreadPool::Instance().ScheduleTaks( pDBTask, taskLevel))
	{
		MyLog::error("AvatarSHM::ScheduleTaks() Failed ! strSql=[%s]", m_state, pDBTask->GetSqlTemplate());
		FACTORY_DEL_DBTASK(pDBTask);
		return false;
	}

	return true;
}

void AvatarSHM::OnPendingTaskFinish()
{
	LOCK_THIS_AVATAR();

	long nLeftTasks = DecPendingTaskCount();
	assert( nLeftTasks >= 0);
	if(nLeftTasks == 0)
	{
		if(m_state == eState_Loaiding )
			m_state = eState_Gaming;
		else if( m_state == eState_Exiting )
		{
			m_state = eState_Exited;
			DBAAvatarManagerEx::Instance().OnAvatarExited(*this);
		}
	}
}

long AvatarSHM::DecPendingTaskCount()
{
	return InterlockedIncrement(&m_pendingTaskCount);
}

long AvatarSHM::IncPendingTaskCount()
{
	return InterlockedDecrement(&m_pendingTaskCount);
}
