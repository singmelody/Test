#include "StdAfx.h"
#include "ParamDef.h"
#include "ParamEx.h"

ParamDef::ParamDef(void)
{
	m_maxIdx = -1;
	m_count = 0;
}


ParamDef::~ParamDef(void)
{
}

void ParamDef::LoadParamDef(DBRow& row)
{
	static int32 nCol_ParamID = row.GetColumnIdx("ParamID");
	static int32 nCol_ParamName = row.GetColumnIdx("ParamName");

	row.Fill( m_index, nCol_ParamID, -1);
	row.Fill( m_name, nCol_ParamName, "");
}

bool ParamDef::AddParam(ParamBase* pBase)
{
	if(!pBase || !pBase->Name())
		return false;

	// tool idx
	int32 nIdx = pBase->Index();
	if( (int32)m_paramList.size() <= nIdx )
		m_paramList.resize(nIdx+1);

	m_paramList[nIdx] = pBase;

	// sort idx
	int32 nId = pBase->ID();
	if( (int32)m_paramListByID.size() <= nId)
		m_paramListByID.resize(nId+1);

	m_paramListByID[nId] = pBase;

	if(GetParam(pBase->Name()) != NULL)
		return false;

	m_paramMap[pBase->Name()] = pBase;
	if(nIdx > m_maxIdx)
		m_maxIdx = nIdx;

	++m_count;

	m_poolSize += pBase->ParamSize();

	return true;
}

ParamBase* ParamDef::GetParam(int32 nIdx)
{
	if( nIdx < 0 || nIdx >= (int32)m_paramList.size() )
		return NULL;

	ParamBase* pBase = m_paramList[nIdx];
	return pBase;
}

ParamBase* ParamDef::GetParam(const char* pName)
{
	if(!pName)
		return NULL;

	auto itr = m_paramMap.find(pName);
	if(itr != m_paramMap.end())
		return itr->second;

	return NULL;
}

ParamBase* ParamDef::GetParamByID(int32 nIdx)
{
	if( nIdx < 0 || nIdx >= (int32)m_paramListByID.size())
		return NULL;

	ParamBase* pBase = m_paramListByID[nIdx];
	return pBase;
}
