#pragma once

#include "BaseType.h"
#include "Singleton.h"
#include "DBAFuncManager.h"
#include "MyMutex.h"
#include <deque>
#include <hash_map>

class AvatarSHM;
class DBTaskAvatar;

enum DBATaskLevel
{
	eDBATL_0 = 0,
	eDBATL_1,
	eDBATL_2,
	eDBATL_MAX
};

class DBAAvatarManagerEx : public DBAFuncManager, public Singleton<DBAAvatarManagerEx>
{
public:
	DBAAvatarManagerEx(void);
	~DBAAvatarManagerEx(void);

	void SyncAll2DBAndClearAll();
	void SnapshotAllAvatars();
	bool ScheduleAvatarTask( DBTaskAvatar* pDbTask, DBATaskLevel taskLevel);
	uint32 GetAvatarCount() { return (int32)m_avatarHashMap.size(); }

	void OnAvatarExited(AvatarSHM& avatar);
	void FillConfig();

 	Mutex* GetMutex() { return &m_AvatarHashTableLock; }
 	AvatarSHM* GetAvatar(int64 nDID) const;
	void ProcExitedAvatars();
protected:
	static const uint32 MAX_AVATAR_HASH_TABLE = 4096;

	Mutex				m_AvatarHashTableLock;
	typedef	std::hash_map< int64, AvatarSHM*> AvatarHashMap;
	AvatarHashMap		m_avatarHashMap;

	int32				m_nAccTimeSyncPlayer;
	std::deque<int64>	m_syncQueue;
	int32				DBAConfig_Sync_Avatars_Per_Min;

	// 销毁的角色列表
	Mutex				m_lockQueueExitedAvatars;
	std::deque<int64>	m_queueExitedAvatars;
};

