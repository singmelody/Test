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

}

void SHMManager_DBA::InitParamPool2SqlProcessors(bool bUseSHM)
{
	if(bUseSHM)
	{
		// avatar data
		AddParam2SqlProcessor( eParam_Avatar, new SHMParamPool2SqlProcessor_DBA_Avatar);

		// avatar commondata
		AddParam2SqlProcessor( eParam_Skill, new SHMParamPool2SqlProcessor_DBA_CommonData);
		AddParam2SqlProcessor( eParam_Coin, new SHMParamPool2SqlProcessor_DBA_CommonData);
	}
	else
	{
		// avatar data
		AddParam2SqlProcessor( eParam_Avatar, new ParamPool2SqlProcessor_DBA_Avatar);

		// avatar commondata
		AddParam2SqlProcessor( eParam_Skill, new SHMParamPool2SqlProcessor_DBA_CommonData);
		AddParam2SqlProcessor( eParam_Coin, new SHMParamPool2SqlProcessor_DBA_CommonData);
	}

}
