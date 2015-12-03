#include "StdAfx.h"
#include "ParamDefManager.h"
#include <float.h>
#include "NpcParamDef.h"

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

ParamDef* ParamDefManager::GetParamDef(int32 nParamID)
{
	auto itr = m_paramDefMap.find(nParamID);
	if(itr == m_paramDefMap.end())
		return NULL;

	ParamDef* pDef = itr->second;
	return pDef;
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

bool ParamDefManager_LoadHelper::LoadFromDB(ParamDefManager* pMgr, DBInterface* pDBI)
{
	InitParamDefine( pMgr, pDBI);
	InitParamColumn( pMgr, pDBI);
	InitParamData( pMgr, pDBI);

	ParamSet10 set10;
	ParamSet20 set20;
}

bool ParamDefManager_LoadHelper::InitParamDefine(ParamDefManager* pMgr, DBInterface* pDBI)
{
	NpcParamDef npcDef;

	DBTable table;
	if( !pDBI->ExecuteSql("select * from ParamDefine", table))
		return false;

	int32 nCol_ParamID = table.GetColumnIdx("ParamID");

	auto itr = table.m_rowList.begin();

	for (; itr != table.m_rowList.end(); ++itr)
	{
		if(!(*itr))
			continue;

		DBRow& row = **itr;

		int32 nParamID;
		row.Fill( nParamID, nCol_ParamID, -1);

		ParamDef* pDef = pMgr->CreateParamDef( nParamID );
	}
}
