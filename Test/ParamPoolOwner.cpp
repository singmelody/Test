#include "StdAfx.h"
#include "ParamPoolOwner.h"
#include "ParamPool.h"


ParamPoolOwner::ParamPoolOwner(void)
{
	m_pParamPool = NULL;

	m_nParamTypeID = 0;
	m_nParamDataID = 0;

	m_pParamCallBackPost = NULL;
}


ParamPoolOwner::~ParamPoolOwner(void)
{
	FACTORY_DELOBJ( m_pParamPool );
	SAFE_DELETE( m_pParamCallBackPost );
}

bool ParamPoolOwner::CreateParamPool(int32 nTypeID)
{
	return CreateParamPool( PARAM_ID( nTypeID), PARAM_DATA_ID(nTypeID));
}

bool ParamPoolOwner::CreateParamPool(int32 nDefID, int32 nDataID)
{
	ParamPool* pPool = ParamSet::CreateNew( nDefID, nDataID);
	if(!pPool)
		return false;

	SetParamPool( pPool );
	return true;
}

void ParamPoolOwner::SetParamPool(ParamPool* pPool)
 {
	if(!pPool)
		return;
	
	m_pParamPool = pPool;

	m_nParamDefID = pPool->GetParamDefineIndex();
	m_nParamDataID = pPool->GetDataID();
	m_nParamTypeID = pPool->GetParamTypeID();
}