#include "StdAfx.h"
#include "ParamDefManager.h"
#include <float.h>

ParamDefManager::ParamDefManager(void)
{
}


ParamDefManager::~ParamDefManager(void)
{
}

bool ParamDefManager::LoadDataFromDB(DBInterface* pDBI)
{
	if(!pDBI)
		return false;

	ParamDefManager_LoadHelper helper;

	bool bRes = helper.LoadFromDB( this, pDBI);
	if(res)
	{
		InitParamMD5();

		
	}
	

	return true;
}

void ParamDefManager::InitParamMD5()
{
	uint32 nDefMaxIndex = 0;
	
	ParamMapItr itr = m_paramDefMap.begin();

	for (; itr != m_paramDefMap.end(); ++itr)
	{
		ParamDef* pDef = itr->second;
		if(!pDef)
			continue;

		if( pDef->Index() >= nDefMaxIndex)
			nDefMaxIndex = pDef->Index();
	}

	std::stringstream 
}

ParamBase* ParamDefManager::CreateParam(const char* sParamType, const char* sDft, const char* sMax, const char* sMin)
{
	if(sParamType == "int64")
	{
		Param_Int64* pParam = new Param_Int64();
		InitValue_atoi<int64>( pParam, sDft, sMax, 0x7fffffffffffffff, ~0x7fffffffffffffff)
	}
}
