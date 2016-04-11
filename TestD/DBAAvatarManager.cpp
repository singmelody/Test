#include "StdAfx.h"
#include "DBAAvatarManager.h"
#include "MyMutex.h"
#include "TaskImpl.h"
#include "MyLog.h"
#include "SHMAvatar.h"

DBAAvatarManagerEx::DBAAvatarManagerEx(void)
{
}


DBAAvatarManagerEx::~DBAAvatarManagerEx(void)
{
}

void DBAAvatarManagerEx::SyncAll2DBAndClearAll()
{
	m_syncQueue.clear();

	SnapshotAllAvatars();

	while(!m_syncQueue.empty())
	{
		const int64 avatardid = m_syncQueue.front();
		m_syncQueue.pop_front();

		AvatarSHM* pAvatar = NULL;
		{
			AutoLock tmp(GetMutex());
			pAvatar = GetAvatar(avatardid);
			if(!pAvatar)
				continue;

			auto itr = m_avatarHashMap.find(avatardid);
			m_avatarHashMap.erase(itr);
		}

		assert(pAvatar);
		pAvatar->Sync2DB(true);
	}

	return;
}

void DBAAvatarManagerEx::SnapshotAllAvatars()
{
	AutoLock tmpLock(GetMutex());
	for (auto itr = m_avatarHashMap.begin(); itr != m_avatarHashMap.end(); ++itr)
	{
		m_syncQueue.push_back(itr->first);
	}
}

bool DBAAvatarManagerEx::ScheduleAvatarTask(DBTaskAvatar* pDbTask, DBATaskLevel taskLevel)
{
	AutoLock tmpLock(GetMutex());

	AvatarSHM* pAvatar = GetAvatar(pDbTask->GetAvatarDID());
	if(!pAvatar)
	{
		MyLog::error("DBAAvatarManagerEx::ScheduleAvatarTask can not find avatar by did=[%lld], drop task [%s]", 
			pDbTask->GetAvatarDID(),  pDbTask->GetSqlTemplate());
		return false;
	}

	return pAvatar->ScheduleTask( pDbTask, taskLevel);
}

AvatarSHM* DBAAvatarManagerEx::GetAvatar(int64 nDID) const
{
	auto itr = m_avatarHashMap.find(nDID);
	AvatarSHM* pAvatarSHM = itr->second;
	return pAvatarSHM;
}

