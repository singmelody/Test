#include "StdAfx.h"
#include "ParamPool.h"
#include "ParamDefManager.h"
#include "MyLog.h"
#include <assert.h>
#include "DataBuffer.h"

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
