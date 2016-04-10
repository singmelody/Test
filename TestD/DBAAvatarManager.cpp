#include "StdAfx.h"
#include "DBAAvatarManager.h"
#include "MyMutex.h"
#include "TaskImpl.h"
#include "MyLog.h"
#include "SHMAvatar.h"
#include "PacketImpl.h"

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

void DBAAvatarManagerEx::ProcExitedAvatars()
{
	std::deque<int64> tmp;
	{
		AUTOLOCK(m_lockQueueExitedAvatars);
		tmp.swap(m_queueExitedAvatars);
	}

	AutoLock tmpLock(GetMutex());
	while(!tmp.empty())
	{
		int64 nDid = tmp.front();
		tmp.pop_front();

		AvatarSHM* pAvatar = GetAvatar(nDid);
		assert(pAvatar);

		// destroy and notify world
		{
			PacketAvatarDecommision pkt;
			pkt.SetAvatarID(pAvatar->GetAvatarID());
			DBAServer::Instance().Send2LocalWorld(&pkt);

			auto itr = m_avatarHashMap.find(pAvatar->GetAvatarDID());
			if( itr != m_avatarHashMap.end() )
				m_avatarHashMap.erase(itr);

			FACTORY_DELOBJ(pAvatar);
		}
	}
}

