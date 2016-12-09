#include "StdAfx.h"
#include "TaskImpl.h"
#include "MyMutex.h"
#include "MyLog.h"
#include "DBAAvatarManager.h"
#include "SHMAvatar.h"

FINISH_FACTORY_ARG0(DBTaskAvatar);

DBTaskAvatar::~DBTaskAvatar()
{
	if(m_bScheduled)
	{
		m_bScheduled = false;

		AutoLock tmpLock(DBAAvatarManagerEx::Instance().GetMutex());
		AvatarSHM* pAvatar = DBAAvatarManagerEx::Instance().GetAvatar(nAvatarDID);

		if(!pAvatar)
		{
			MyLog::error("DBTaskAvatar::DoAction Failed execute sql=[%s] for avatar not find did = [%d] id = [%d]", 
			sqlTemplate.c_str(), nAvatarDID, nAvatarID);
		}
		else
		{
			pAvatar->OnPendingTaskFinish();
		}
	}
}

bool DBTaskAvatar::DoAction(DBInterface* pInterface)
{
	if(!IsAvatarInvalid())
	{
		MyLog::error( "DBTaskAvatar::DoAction() Failed! Avatar not valid ! Sql=[%s] avatardid=[%lld]", sqlTemplate.c_str(), nAvatarDID);
		return true;
	}

	return DBTask::DoAction(pInterface);
}

bool DBTaskAvatar::IsAvatarInvalid()
{
	AutoLock tmpLock(DBAAvatarManagerEx::Instance().GetMutex());
	AvatarSHM* pAvatar = DBAAvatarManagerEx::Instance().GetAvatar(nAvatarDID);
	if(!pAvatar)
	{
		MyLog::error("DBTaskAvatar::IsAvatarValid() Can not find avatar by did=[%lld]", nAvatarDID);
		return false;
	}

	if(m_nAvatarStatesNeededBeforeQueue)
	{
		uint8 nAvatarState = pAvatar->GetState();
		return ( m_nAvatarStatesNeededBeforeQueue & nAvatarState) != 0;
	}

	return true;
}
