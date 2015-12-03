#include "StdAfx.h"
#include "ParamDefManager.h"
#include "ParamSet.h"
#include <float.h>
#include "NpcParamDef.h"

ParamDefManager::ParamDefManager(void)
{
	m_paramDefClassType[eParam_Avatar] = "NpcParamDef";
	m_paramDefClassType[eParam_Monster] = "MonsterParamDef";
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

ParamDef* ParamDefManager::GetParamDef(int32 nParamID, bool bCreate /*= false*/)
{
	auto itr = m_paramDefMap.find(nParamID);
	if(itr == m_paramDefMap.end())
	{
		if(!bCreate)
			return NULL;

		ParamDef* pDef = NULL;

		std::string& className = m_paramDefClassType[nParamID];
		if(!className.empty())
			pDef = (ParamDef*)FactoryManager::Instance().New( className.c_str() );

		if(!pDef)
			pDef = new ParamDef();

		return pDef;
	}

	ParamDef* pDef = itr->second;
	return pDef;
}

void ParamDefManager::AddParamDef(int32 nIdx, ParamDef* pDef)
{
	m_paramDefMap[nIdx] = pDef;
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

		ParamDef* pDef = pMgr->GetParamDef( nParamID, true);
		if(!pDef)
			continue;

		pDef->LoadParamDef( **itr );

		pMgr->AddParamDef( pDef->Index(), );
	}

	return true;
}
