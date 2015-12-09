#include "StdAfx.h"
#include "ParamDefManager.h"
#include "ParamSet.h"
#include <float.h>
#include "NpcParamDef.h"
#include <sstream>

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
	if(bRes)
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
	int32 nDefMaxIndex = 0;
	
	ParamMapItr itr = m_paramDefMap.begin();

	for (; itr != m_paramDefMap.end(); ++itr)
	{
		ParamDef* pDef = itr->second;
		if(!pDef)
			continue;

		if( pDef->Index() >= nDefMaxIndex)
			nDefMaxIndex = pDef->Index();
	}

	std::stringstream paramValString;
	for (int32 i = 0; i <= nDefMaxIndex; ++i)
	{
		ParamDef* pDef = GetParamDef(i);
		if(!pDef)
			continue;

		paramValString << pDef->Name();

		int32 nParamCount = pDef->MaxParamIndex();
		for (;;)
		{
		}
	}
}

ParamBase* ParamDefManager::CreateParam(const char* sParamType, const char* sDft, const char* sMax, const char* sMin)
{
	if(sParamType == "int64")
	{
		Param_Int64* pParam = new Param_Int64();
		InitValue_A2Num<int64>( pParam, sDft, sMax, 0x7fffffffffffffff, sMin, ~0x7fffffffffffffff);
		return pParam;
	}
// 	else if(sParamType == "uint64")
// 	{
// 		Param_UInt64* pParam = new Param_UInt64();
// 		InitValue_A2Num<uint64>( pParam, sDft, sMax, 0x7fffffffffffffff, sMin, ~0x7fffffffffffffff);
// 	}
	else if(sParamType == "int32")
	{
		Param_Int32* pParam = new Param_Int32();
		InitValue_A2Num<int32>( pParam, sDft, sMax, MAXINT32, sMin, MININT32);
		return pParam;
	}
	else if(sParamType == "int16")
	{
		Param_Int16* pParam = new Param_Int16();
		InitValue_A2Num<int16>( pParam, sDft, sMax, MAXINT16, sMin, MININT16);
		return pParam;
	}

	return NULL;
}

bool ParamDefManager_LoadHelper::LoadFromDB(ParamDefManager* pMgr, DBInterface* pDBI)
{
	InitParamDefine( pDBI);
	InitParamColumn( pDBI);
	InitParamData(pDBI);

	ParamSet10 set10;
	ParamSet20 set20;

	return true;
}

bool ParamDefManager_LoadHelper::InitParamDefine(DBInterface* pDBI)
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

		ParamDef* pDef = ParamDefManager::Instance().GetParamDef( nParamID, true);
		if(!pDef)
			continue;

		pDef->LoadParamDef( **itr );
		ParamDefManager::Instance().AddParamDef( pDef->Index(), pDef);

		(*itr)->Release();
	}

	return true;
}

bool ParamDefManager_LoadHelper::InitParamColumn(DBInterface* pDBI)
{
	DBTable table;

	if(!pDBI->ExecuteSql("selct * from ParamColumn", table))
		return false;

	static int32 nCol_ParamID = table.GetColumnIdx("ParamID");
	static int32 nCol_MemberIdx = table.GetColumnIdx("MemberID");
	static int32 nCol_Name = table.GetColumnIdx("Name");
	static int32 nCol_Type = table.GetColumnIdx("Type");
	static int32 nCol_Dft = table.GetColumnIdx("Default");
	static int32 nCol_Min = table.GetColumnIdx("Min");
	static int32 nCol_Max = table.GetColumnIdx("Max");

	DBRowList::iterator itr = table.m_rowList.begin();
	for (; itr != table.m_rowList.end(); ++itr)
	{
		if(!(*itr))
			continue;

		DBRow& row = **itr;
		
		int32 nParamID;
		row.Fill( nParamID, nCol_ParamID, 0);

		ParamDef* pDef = ParamDefManager::Instance().GetParamDef( nParamID );
		if(!pDef)
			continue;

		int32 nMemberIdx;
		std::string sParamName, sType, sDft, sMax, sMin;
		row.Fill( nMemberIdx, nCol_MemberIdx, -1);
		row.Fill( sParamName, nCol_Name, "");
		row.Fill( sType, nCol_Type, "");
		row.Fill( sDft, nCol_Dft, "");
		row.Fill( sMax, nCol_Min, "");
		row.Fill( sMin, nCol_Max, "");

		ParamBase* pBase = ParamDefManager::Instance().CreateParam( sType.c_str(), sDft.c_str(), sMax.c_str(), sMin.c_str());

		int32 nParamUID = UtilID::CreateFromString(sParamName.c_str());
		pBase->CID(nParamUID);
		pBase->ID(nMemberIdx);
		pBase->Index(nMemberIdx);
		pBase->Name(sParamName.c_str());

		pDef->AddParam(pBase);

		(*itr)->Release();
	}

	// sort process
	const ParamMap& map = ParamDefManager::Instance().GetDefMap();
	for (auto itr = map.begin(); itr != map.end(); ++itr)
	{
		ParamDef* pDef = itr->second;
		if(!pDef)
			continue;

		pDef->RefreshOffset();
		pDef->MatchClassType();
	}

	return true;
}

bool ParamDefManager_LoadHelper::InitParamData(DBInterface* pDBI)
{
	return true;
}
