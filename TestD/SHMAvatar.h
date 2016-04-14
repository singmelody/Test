#pragma once

#include "DBTask.h"
#include "Factory.h"
#include "Allocator.h"
#include "ParamDef.h"

class ParamPool;

class AvatarSHM
{
	DECLARE_FACTORY_ARG0( AvatarSHM, -1, new PoolAllocatorEx);
public:
	AvatarSHM();
	~AvatarSHM();

	bool InitParamSet(int32 nDataID);
	inline ParamPool* GetParamSet() { return m_pPool; }

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