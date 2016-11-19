#include "StdAfx.h"
#include "ParamPool.h"
#include "ParamDefManager.h"
#include "MyLog.h"
#include <assert.h>
#include "DataBuffer.h"
#include "ParamTypeDef.h"
#include "SHMNodeExtraInfo.h"

ParamPool::ParamPool(void)
{
}


ParamPool::~ParamPool(void)
{
}

void ParamPool::PreSetValue(ParamBase* pBase, bool bSaveOldValue)
{
	if(!pBase)
		return;

	if(!pBase->CheckFlag(ePF_Callback))
		return;

	if( bSaveOldValue && m_callback != NULL)
		SaveOldValue(pBase);
}

void ParamPool::PostSetValue(ParamBase* pBase)
{
	if(!pBase)
		return;

	if( m_callback && pBase->CheckFlag(ePF_Callback) )
	{
		(*m_callback)( this, pBase);
	}
}

bool ParamPool::InitParamPool(int32 nParamID, int32 nDataID, char* pBuff /*= NULL*/)
{
	ParamDef* pDef = ParamDefManager::Instance().GetParamDef(nParamID);
	if(!pDef)
		return false;

	return InitParamPool( pDef, nDataID, pBuff);
}

bool ParamPool::InitParamPool(ParamDef* pDef, int32 nDataID, char* pBuff /*= NULL*/)
{
	if(!pDef)
		return false;

	SetParamDefine( pDef );

	if(!pBuff)
		SetParamBuffer( pBuff );
	else
	{
		if(!AllocParamBuffer())
		{
			MyLog::error("ParamPool::InitParamPool Failed");
			return false;
		}
	}

	return InitParamPool(nDataID);
}

bool ParamPool::InitParamPool(int32 nDataID)
{
	if(!m_pDef)
		return false;

	if( nDataID == -1)
		return true;

	if(m_pDef->IsLoadData())
	{
		DataBufferBase* pData = m_pDef->GetItem( nDataID );
		if(!pData)
			return false;

		m_pTemplateBuffer = pData->GetBuffer();
		ParamDebugUion DebugBuffer;
		DebugBuffer.ParamBuffer = m_pTemplateBuffer;

		if(!Buffer2Data( m_pTemplateBuffer ))
			return false;

		SetAllDefault();
	}

	SetDataID(nDataID);
	return true;

}

void ParamPool::NodeServer_WriteSHM_AvatarDID(int64 nAvatarDID)
{
	ParamPoolExInfo_Node_CommonData* pExtraInfo = (ParamPoolExInfo_Node_CommonData*)SHMGetExtraMemory();
	assert(pExtraInfo);
	pExtraInfo->m_nAvatarDID = nAvatarDID;
}

void ParamPool::NodeServer_WriteSHM_CommonDataIdx(int32 nCommonDataIdx)
{
	assert(m_pDef && m_pParamBuffer);
	ParamPoolExInfo_Node_CommonData* pExtraInfo = (ParamPoolExInfo_Node_CommonData*)SHMGetExtraMemory();
	assert(pExtraInfo);
	pExtraInfo->m_nCommonDataIdx = nCommonDataIdx;
}

void ParamPool::NodeServer_WriteSHM_CommonDataFlag(int32 nCommonDataFlag)
{
	assert(m_pDef && m_pParamBuffer);
	ParamPoolExInfo_Node_CommonData* pExtraInfo = (ParamPoolExInfo_Node_CommonData*)SHMGetExtraMemory();
	assert(pExtraInfo);
	pExtraInfo->m_nCommonDataFlag = nCommonDataFlag;
}

void ParamPool::SaveOldValue(ParamBase* pBase)
{
	if(!pBase)
		return;

	switch(pBase->TypeID())
	{
	case eTB_Int16:
		*((int16*)m_oldDataBuff) = GetValue<int16>(pBase);
		break;
	case eTB_Int32:
		*((int32*)m_oldDataBuff) = GetValue<int32>(pBase);
		break;
	case eTB_Int64:
		*((int64*)m_oldDataBuff) = GetValue<int64>(pBase);
		break;
	}
}

void ParamPool::SetParamDefine(ParamDef* pDef)
{
	m_pDef = pDef;
	m_paramBuffSize = pDef->Size();
	int32 nMaxIndex = pDef->GetMaxIndex();
	m_paramBlockCount= (nMaxIndex / MAX_PARAM_COUNT_IN_BLOCK) + 1;
}

void ParamPool::SetParamBuffer(char* pBuff)
{
	m_bUserBuff = true;
	m_pParamBuffer = pBuff;
	m_debugBuffer.ParamBuffer = m_pParamBuffer;
}

bool ParamPool::AllocParamBuffer()
{
	if(!m_pDef)
		return false;

	m_pParamBuffer = m_pDef->MakeBuffer();
	if(!m_pParamBuffer)
		return false;

	assert( IsValidMemory(m_pParamBuffer));
	memset( m_pParamBuffer, 0, m_pDef->Size() + m_pDef->ExtraSize());
	return true;
}

ParamDef* ParamPool::GetParamDef() const
{
	return m_pDef;
}

int32 ParamPool::GetParamDefineIndex()
{
	if(!m_pDef)
		return -1;

	return m_pDef->Index();
}

int32 ParamPool::GetParamTypeID()
{
	return MAKE_TYPE_ID( GetParamDefineIndex(), GetDataID() );
}

void* ParamPool::SHMGetExtraMemory()
{
	assert( m_pDef && m_pParamBuffer);
	if( NULL == m_pDef->GetIParamPool2SqlProcessor())
		return NULL;

	if( NULL == m_pDef->ExtraSize())
		return NULL;

	return m_pParamBuffer + m_paramBuffSize;
}

// |count|idx 0|flag 0|data 0|idx 1|flag 1|data 1|........|idx n|flag n|data n|
bool ParamPool::Write(char* pBuffer, int32& nBufferSize, int32& nStartParamPos, uint32 paramFlag, uint32 nSyncFlag, uint32 nParamFlagExclude /*= 0*/)
{
	assert( IsValidMemory(m_pParamBuffer) );
	assert( pBuffer && nBufferSize > 0);

	const int32 RESERVE_BUFF_SIZE = 4;

	const char* const buffer_start = pBuffer;
	const char* const buffer_end = pBuffer + nBufferSize;
	nBufferSize = 0;

	bool bAll = nSyncFlag & eParam_Sync_All;
	bool bClearDirty = nSyncFlag & eParam_Sync_ClearDirty;
	bool bIgnoreDft = nSyncFlag & eParam_Sync_IgnoreDft;

	int32 nStartBlock = nStartParamPos / MAX_PARAM_COUNT_IN_BLOCK;
	int32 nStartPosOfBlock = nStartParamPos % MAX_PARAM_COUNT_IN_BLOCK;

	uint8& nCountBlocks = *(uint8*)buffer_start;
	nCountBlocks = 0;
	nBufferSize += sizeof(uint8);

	int32 nBlockIdx = nStartBlock;
	for (; nBlockIdx < m_paramBlockCount; ++nBlockIdx, nStartPosOfBlock = 0)
	{
		if( buffer_start + nBufferSize + ParamStreamInfo::Size() >= buffer_end)
			break;

		const int32 nOriBufferSize = nBufferSize;
		ParamStreamInfo* pInfo = (ParamStreamInfo*)(buffer_start + nBufferSize);
		nBufferSize += ParamStreamInfo::Size();

		pInfo->nBlockFlag = nBlockIdx;
		pInfo->nBlockFlag = 0;

		int32 nParamIdxOfBlock = nStartPosOfBlock;
		for (; nParamIdxOfBlock < MAX_PARAM_COUNT_IN_BLOCK; ++nParamIdxOfBlock)
		{
			nStartParamPos = nBlockIdx*MAX_PARAM_COUNT_IN_BLOCK + nParamIdxOfBlock;

			ParamBase* pBase = m_pDef->GetParam( nStartParamPos );

			if(!pBase)
				continue;

			if(!pBase->CheckFlag(paramFlag))
				continue;

			if( nParamFlagExclude != 0 && pBase->CheckFlag( nParamFlagExclude) )
				continue;

			if( bIgnoreDft && DefaultCheck(nStartParamPos))
				continue;

			if(!bAll && !ParamDirtyCheck(nStartParamPos))
				continue;

			if( buffer_start + nBufferSize + pBase->ParamSize() + RESERVE_BUFF_SIZE >= buffer_end )
				break;

			char* pTemp = pBase->ParamWrite( m_pParamBuffer, (char*)buffer_start + nBufferSize);
			assert( pTemp < buffer_end );

			pInfo->nBlockFlag |= (uint32(1) << nParamIdxOfBlock);
			nBufferSize = pTemp - buffer_start;

			// debug code
			{
				std::string strP = m_pDef->Name();
				strP += "_";
				strP += pBase->Name();
				ParamSyncCounter::Instance().AddCount( strP.c_str(), paramFlag);
			}
		}

		if(pInfo->nBlockFlag == 0)	// no param, minute the size just add (nBufferSize += ParamStreamInfo::Size())
		{
			nBufferSize -= ParamStreamInfo::Size();
			assert( nOriBufferSize == nBufferSize);
		}
		else
			nCountBlocks++;

		if(nParamIdxOfBlock < MAX_PARAM_COUNT_IN_BLOCK) // NO SPACE FOR WRITE
			break;
	}

	if(nCountBlocks == 0)
	{
		nStartParamPos = -1;
		nBufferSize = 0;
		return true;
	}

	return nBlockIdx >= m_paramBlockCount;	// is write already
}

char* ParamPool::Data2Buffer(char* pBuffer)
{
	if(!pBuffer)
		return NULL;

	assert( IsValidMemory(m_pParamBuffer) );

	memcpy( pBuffer, m_pParamBuffer, m_paramBuffSize);
	return pBuffer + m_paramBuffSize;
}

char* ParamPool::Buffer2Data(char* pBuffer)
{
	if(!pBuffer)
		return NULL;

	assert( IsValidMemory(m_pParamBuffer) );

	memcpy( m_pParamBuffer, pBuffer, m_paramBuffSize);
	return pBuffer + m_paramBuffSize;
}

void ParamPool::UpdateParambit(ParamBase* pBase, bool bUpdateDirtyBit)
{
	if(!pBase)
		return;

	int32 nIdx = pBase->Index();

	if( pBase->CheckFlag( ePF_Server) && bUpdateDirtyBit )
		SetParamDirty(nIdx);

	if( m_pTemplateBuffer != NULL )
	{
		bool bDefault = pBase->ParamCompare( m_pParamBuffer, m_pTemplateBuffer);
		SetDefault( nIdx, bDefault);
	}
}

FINISH_FACTORY_ARG0(ParamSet)
ParamPool* ParamSet::CreateNew(int32 nParamType, char* pBuff /*= 0*/)
{
	int32 nDefID = PARAM_ID( nParamType );
	int32 nDataID = PARAM_DATA_ID( nParamType );

	return CreateNew( nDefID, nDataID, pBuff);
}

ParamPool* ParamSet::CreateNew(int32 nDefID, int32 nDataID, char* pBuff /*= 0*/)
{
	ParamDef* pDef = ParamDefManager::Instance().GetParamDef(nDefID);
	if(!pDef)
		return NULL;

	ParamPool* pSet = (ParamPool*)FactoryManager::Instance().New( pDef->ParamSetName().c_str());
	if(!pSet)
		return NULL;

	if(!pSet->InitParamPool( pDef, nDataID, pBuff))
	{
		FACTORY_DELOBJ(pSet);
		return NULL;
	}

	return pSet;
}


void ParamPool::SetValueString(int32 nIdx, const char* Val, bool bDirty /*= true*/)
{
	if(!m_pDef)
		return;

	ParamBase* pBase = m_pDef->GetParam(nIdx);
	SetValueString( pBase, Val, bDirty);
}

void ParamPool::SetValueString(const char* sName, const char* Val, bool bDirty /*= true*/)
{
	if(!m_pDef)
		return;

	ParamBase* pBase = m_pDef->GetParam(sName);
	SetValueString( pBase, Val, bDirty);
}

void ParamPool::SetValueString(ParamBase* pParam, const char* Val, bool bDirty /*= true*/)
{
	if(!pParam)
		return;

	ParamStr* pVal = static_cast<ParamStr*>(pParam);
	if(pVal)
	{
		PreSetValue( pParam, false);
		pVal->SetValue( m_pParamBuffer, Val);
		PostSetValue( pParam );
	}

	UpdateParambit( pParam, bDirty);
}

const char* ParamPool::GetValueString(int32 nIdx, const char* defVal)
{
	if(!m_pDef)
		return defVal;

	ParamBase* pParam = m_pDef->GetParam( nIdx );
	return GetValue( pParam, defVal);
}

const char* ParamPool::GetValueString(const char* sName, const char* defVal)
{
	if(!m_pDef)
		return defVal;

	ParamBase* pParam = m_pDef->GetParam(sName);
	return GetValue( pParam, defVal);
}

const char* ParamPool::GetValueString(ParamBase* pParam, const char* defVal)
{
	assert( IsValidMemory(pParam) );

	if(!m_pDef || !defVal)
		return defVal;

	return pParam->GetValueString(m_pParamBuffer);
}

void ParamSyncCounter::AddCount(const char* sName, int32 flag)
{
	if(!sName)
		return;

	if ( (( flag & eParam_Flag_CltSelf) != 0) && ((flag & eParam_Flag_CltAll) == 0) )
	{
		ParamCountMap::iterator itr = m_CltSelfMap.find(sName);
		if( itr == m_CltSelfMap.end() )
			m_CltSelfMap.insert(std::make_pair<std::string, int32>( sName, 1));
		else
			itr->second++;
	}

	if( (( flag & eParam_Flag_CltSelf) == 0) && ((flag & eParam_Flag_CltAll) != 0) )
	{
		ParamCountMap::iterator itr = m_CltAllMap.find(sName);
		if( itr == m_CltAllMap.end() )
			m_CltAllMap.insert(std::make_pair<std::string, int32>( sName, 1));
		else
			itr->second++;
	}
}

void ParamSyncCounter::Clear()
{
	m_CltSelfMap.clear();
	m_CltAllMap.clear();
}

void ParamSyncCounter::Print()
{
	ParamCountMap::iterator itr = m_CltSelfMap.begin();
	for (; itr != m_CltSelfMap.end(); ++itr)
	{
		MyLog::message("Param Client Self Sync Name[%s] Count[%d]", itr->first.c_str(), itr->second);
	}

	ParamCountMap::iterator itr1 = m_CltAllMap.begin();
	for (; itr1 != m_CltAllMap.end(); ++itr1)
	{
		MyLog::message("Param Client All Sync Name[%s] Count[%d]", itr1->first.c_str(), itr1->second);
	}
}
