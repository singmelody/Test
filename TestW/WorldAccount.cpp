#include "StdAfx.h"
#include "WorldAccount.h"
#include "PacketImpl.h"
#include "ParamPool.h"

WorldAccount::WorldAccount(void)
	: m_bIsFCMAcount(false)
	, m_nMilliSeconds(0)
	, m_nAccTimeAfterLastHeartBeat(0)
	, m_bNeedNoticeFCMStage(false)
	, m_bIsBillingOl(false)
	, m_bHasBillingOlOnce(false)
{
	memset( m_pRoleDataSet, 0, sizeof(m_pRoleDataSet));

	m_pRecentRoleSet = NULL;
}


WorldAccount::~WorldAccount(void)
{
	DestroyAllRoleSet();
}

void WorldAccount::DestroyAllRoleSet()
{
	for (int32 i = 0; i < MAX_AVATAR_COUNT_ONE_USER; ++i)
	{
		FACTORY_DELOBJ(m_pRoleDataSet[i]);
	}
}

void WorldAccount::AddRoleSet(class PacketUserData* pPkt)
{
	ParamPool* pPool = GetRoleSet( pPkt->nIndex );
	if(!pPool)
	{
		if(!m_pRecentRoleSet)
		{
			int64 nRecentDID = PARAM_GET_VALUE( m_pRecentRoleSet, avatardid, int64(0));
			if( nRecentDID == pPkt->nAvatarDID)
			{
				SetRoleSet( pPkt->nIndex, m_pRecentRoleSet);
				m_pRecentRoleSet = NULL;
				return;
			}
		}

		pPool = CreateRoleSet( pPkt->nIndex, PARAM_ID( pPkt->m_ParamType), PARAM_DATA_ID( pPkt->m_ParamType));
		if(!pPool)
			return;
	}

	pPkt->UpdateParamPool( pPool );

	if(pPkt->nAvatarDID != -1)
		pPool->SetValue( "avatardid", pPkt->nAvatarDID);
}

bool WorldAccount::SetRoleSet(int32 nIdx, ParamPool* pPool)
{
	if( nIdx < 0 || nIdx >= MAX_AVATAR_COUNT_ONE_USER)
		return false;

	ParamPool*& pSet = m_pRoleDataSet[nIdx];
	if(!pSet)
		return false;

	pSet = pPool;
	return true;
}

void WorldAccount::SetRecentRoleSet(ParamPool* pPool)
{
	if( m_pRecentRoleSet != pPool )
		FACTORY_DELOBJ( m_pRecentRoleSet );

	m_pRecentRoleSet = pPool;
}
