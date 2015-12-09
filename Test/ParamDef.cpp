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

void ParamDef::OrderBySize()
{
	std::vector<ParamBase*> vSize8, vSize4, vSize2, vSize1, vSizeOther;

	for (int32 i = 0; i <= m_maxIdx; ++i)
	{
		ParamBase* pParam = m_paramList[i];
		if(!pParam)
			continue;

		int32 nPSize = pParam->ParamSize();
		if(nPSize == 8)
			vSize8.push_back(pParam);
		else if(nPSize == 4)
			vSize4.push_back(pParam);
		else if(nPSize == 2)
			vSize1.push_back(pParam);
		else if(nPSize == 1)
			vSize1.push_back(pParam);
		else
			vSizeOther.push_back(pParam);
	}

	for (int32 i = 0; i <= m_maxIdx; ++i)
	{
		ParamBase* pParam = m_paramList[i];
		if(!pParam)
			continue;
		m_paramList[i] = NULL;
	}

	m_maxIdx = -1;	// 设置-1,让AddParam的时候不断更新maxidx
	m_poolSize = 0;
	m_count = 0;
	m_paramMap.clear();

	int32 nParamIdx = 0;
	for (int32 i = 0; i < (int32)vSize8.size(); ++i)
	{
		ParamBase* pParam = vSize8[i];
		pParam->Index(nParamIdx);
		AddParam(pParam);
		++nParamIdx;
	}

	for (int32 i = 0; i < (int32)vSize4.size(); ++i)
	{
		ParamBase* pParam = vSize4[i];
		pParam->Index(nParamIdx);
		AddParam(pParam);
		++nParamIdx;
	}

	for (int32 i = 0; i < (int32)vSize2.size(); ++i)
	{
		ParamBase* pParam = vSize2[i];
		pParam->Index(nParamIdx);
		AddParam(pParam);
		++nParamIdx;
	}

	for (int32 i = 0; i < (int32)vSize1.size(); ++i)
	{
		ParamBase* pParam = vSize1[i];
		pParam->Index(nParamIdx);
		AddParam(pParam);
		++nParamIdx;
	}

	for (int32 i = 0; i < (int32)vSizeOther.size(); ++i)
	{
		ParamBase* pParam = vSizeOther[i];
		pParam->Index(nParamIdx);
		AddParam(pParam);
		++nParamIdx;
	}
}

void ParamDef::RefreshOffset() const
{
	OrderBySize();

	int32 nCurOff = 0;
	for (int32 i = 0; i <= m_maxIdx; ++i)
	{
		ParamBase* pBase = m_paramList[i];
		if(!pBase)
			continue;

		pBase->Offset(nCurOff);
		nCurOff += pBase->ParamSize();
	}
}

void ParamDef::MatchClassType()
{
	if( CheckClassType( 10, "ParamSet10", "ParamSetEx10"))
		return;

	if( CheckClassType( 20, "ParamSet20", "ParamSetEx20"))
		return;

	if( CheckClassType( 30, "ParamSet30", "ParamSetEx30"))
		return;
}

void ParamDef::InitDftValues(void* pClassObj)
{
	for (int32 i = 0; i <= m_maxIdx; ++i)
	{
		ParamBase* pParam = m_paramList[i];
		if(!pParam)
			continue;

		pParam->SetDftValue( pClassObj );
	}
}

char* ParamDef::MakeBuffer()
{
	if(!m_pBufferAlloc)
		return NULL;

	return (char*)m_pBufferAlloc->TMalloc(Size() + ExtraSize());
}

void ParamDef::FreeBuffer(char* pBuffer)
{
	if(!m_pBufferAlloc)
		return;
	
	m_pBufferAlloc->TFree(pBuffer);
}

DataBufferBase* ParamDef::GetItem(int32 nDataID)
{
	auto itr = m_bufferMap.find(nDataID);
	if( itr == m_bufferMap.end() )
		return NULL;

	DataBufferBase* ptr = itr->second;
	return ptr;
}

void ParamDef::SetItem(int32 nDataID, DataBufferBase* pBase)
{
	m_bufferMap[nDataID] = pBase;
}

bool ParamDef::CheckClassType(int32 nValue, const char* pClassType, char* pClassTypeEx)
{
	if( m_maxIdx >= nValue )
		return false;

	m_classParamSet = pClassType;
	m_classParamSetEx = pClassTypeEx;

	return true;
}
