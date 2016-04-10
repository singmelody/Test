#pragma once

#include "DBTask.h"
#include "Factory.h"
#include "Allocator.h"
#include "ParamDef.h"
#include "DBAAvatarManager.h"

class ParamPool;
class DBTaskAvatar;

enum DBAAvatarState
{
	eDAS_Init		= 1,
	eDAS_Loading	= 1 << 1,
	eDAS_Gaming		= 1 << 2,
	eDAS_Exiting	= 1 << 3,
	eDAS_Exited		= 1 << 4
};

class AvatarSHM
{
	DECLARE_FACTORY_ARG0( AvatarSHM, -1, new PoolAllocatorEx);
public:
	AvatarSHM();
	~AvatarSHM();

	bool InitParamSet(int32 nDataID);
	inline ParamPool* GetParamSet() { return m_pPool; }

	void Sync2DB(bool bExit = false);
	bool ScheduleTask( DBTaskAvatar* pDBTask, DBATaskLevel taskLevel);

	inline void SetAvatarID(int32 nID) { m_avatarID = nID; }
	inline int32 GetAvatarID() const { return m_avatarID; }
protected:

	long DecPendingTaskCount();
	long IncPendingTaskCount();

	void ScheduleLoadingCommonDataTask(const std::string& strSql, int32 commonDataType);
	void ScheduleSavingTask( const std::string& strSql);

	int32				m_avatarID;
	int64				m_avatarDID;

	volatile long		m_pendingTaskCount;
	volatile uint8		m_state;
	ParamPool*			m_pPool;
	typedef std::map<int32, ParamPool*> Idx2CommonDataMap;
	Idx2CommonDataMap	m_arrMapIdx2CommonData[eParam_Max];
};