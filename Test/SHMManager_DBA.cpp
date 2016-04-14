#include "StdAfx.h"
#include "SHMManager_DBA.h"


SHMManager_DBA::SHMManager_DBA(void)
{
}


SHMManager_DBA::~SHMManager_DBA(void)
{
}

void SHMManager_DBA::Sync2DB()
{
	LOCK_THIS_AVATAR();
	if( m_state != EState_Gaming )
		return;

	IParamPool2SqlProcessor* pProcessor = DBAServer::Instance().GetParam2SqlProcessor(Param_avatar);
	assert(pProcessor);

	std::string strSql;
	if(pProcessor->GetSqlString(*m_pPool, strSql))
	{
		ScheduleSavingTask(strSql);

		SParamPoolExInfo_DBA_Avatar* pExInfo = (SParamPoolExInfo_DBA_Avatar*)m_pPool->SHMGetExtraMemory();
		assert(pExInfo);
		pExInfo->SetLoaded();
	}

	for (int32 i = CommonData_Min + 1; i < CommonData_Max; ++i)
	{
		Idx2CommonDataMap& theMap = m_arrMapIdx2ComonData[i];
	}
}

void SHMManager_DBA::InitParamPool2SqlProcessors(bool bUseSHM)
{
	if(bUseSHM)
	{
		// avatar data
		AddParam2SqlProcessor( eParamType_Avatar, new SHMParamPool2SqlProcessor_DBA_Avatar);
	}
	else
	{
		// avatar data
		AddParam2SqlProcessor( eParamType_Avatar, new ParamPool2SqlProcessor_DBA_Avatar);
	}

}
