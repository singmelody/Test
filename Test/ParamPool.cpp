#include "StdAfx.h"
#include "ParamPool.h"
#include "ParamDefManager.h"

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
