#include "StdAfx.h"
#include "SHMAvatar.h"
#include "MyMutex.h"
#include "ParamPool2Sql_DBA.h"
#include "DBAServer.h"

const uint32 _MAX_LOCKS = 32;
static Mutex s_lock[_MAX_LOCKS];
static Mutex& GetLock(int64 nDid) { return s_lock[uint32(nDid) % _MAX_LOCKS]; }

#define LOCK_THIS_AVATAR AUTOLOCK(GetLock(m_avatarDID));

void AvatarSHM::Sync2DB(bool bExit /*= false*/)
{
	LOCK_THIS_AVATAR;

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

}
