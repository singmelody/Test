#include "StdAfx.h"
#include "WorldAccount.h"
#include "PacketImpl.h"

WorldAccount::WorldAccount(void)
	: m_bIsFCMAcount(false)
	, m_nMilliSeconds(0)
	, m_nAccTimeAfterLastHeartBeat(0)
	, m_bNeedNoticeFCMStage(false)
	, m_bIsBillingOl(false)
	, m_bHasBillingOlOnce(false)
{
	memset( m_pRoleDataSet, 0, sizeof(m_pRoleDataSet));
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
}

bool WorldAccount::SetRoleSet(int32 nIdx, ParamPool* pPool)
{

}
