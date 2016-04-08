#include "StdAfx.h"
#include "SHMManager_DBA.h"


SHMManager_DBA::SHMManager_DBA(void)
{
}


SHMManager_DBA::~SHMManager_DBA(void)
{
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
