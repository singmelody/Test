#include "StdAfx.h"
#include "MyClt.h"
#include "PacketImpl.h"

MyClt::MyClt(void)
{
	m_pManager = NULL;

}


MyClt::~MyClt(void)
{
}

void MyClt::Logout()
{
	if(m_nSocketID < 0)
		return;

	m_pManager->m_pNetManager->Disconnect(m_nSocketID);
	m_nSocketID = -1;

	m_bIsLogin = false;
}

void MyClt::SendPacket(PacketBase* pPkt)
{
	if( m_nSocketID >= 0 && m_pManager )
		m_pManager->SendPacket( m_nSocketID, pPkt);
}

void MyClt::InitAvatarData(class PacketInitAvatarData* pPkt)
{
	DWORD nParamID = pPkt->nParamType;
	int32 nSrcAvatarID = pPkt->nSrvAvatarID;

	if( nSrcAvatarID == m_nAvatarID )
	{
		InitAvatarData_LocalPlayer(pPkt);
		return;
	}

	GameDummy* pDummy = m_pScene->GetGameDummy(nSrcAvatarID);
	if(!pDummy)
		return;

	ParamPool* pParamSet = pDummy->m_pParamSet;
	if(!pParamSet)
	{
		pParamSet = ParamSet::CreateNew( nParamID );
		pDummy->SetParamSet(ParamSet);
	}

	pPkt->UpdateParamPool(pParamSet);

	if(pPkt->IsLastPacket())
	{
		m_pScene->RemoveGameDummy(nSrcAvatarID);

		CltGameActor* pObj = m_pScene->CreateGameActor( pPkt->nSrcAvatarID, pParamSet, pDummy);

		if(!pObj)
		{
			pObj->nParamTypeID = PARAM_ID(nParamID);
			pObj->nParamDataID = PARAM_DATA_ID(nParamID);
		}

		SAFE_DELETE(pDummy);

		// drop data from server
		PacketCltCommonDataRequest pkt;

		pkt.TargetAvatarID = pObj->GetAvatarID();
		pkt.SetAvatarID(m_nAvatarID);

		SendPacket(&pkt);
	}
}

void MyClt::UpdateAvatarData(class PacketUpdateAvatarData* pPkt)
{
	DWORD nParamID = pPkt->nParamType;

	if(m_nAvatarID == pPkt->nSrcAvatarID)
	{
		if(m_pLocalDummy != NULL)
		{
			ParamPool* pParamSet = m_pLocalDummy->m_pParamSet;
			if(!pParamSet)
			{
				pParamSet = ParamSet::CreateNew(nParamID);
				m_pLocalDummy->m_pParamSet = pParamSet;
			}

			pPkt->UpdateParamPool(pParamSet);
			return;
		}
	}

	CltGameActor* pActor = m_pScene->GetGameActor(pPkt->nSrcAvatarID);
	if(!pActor)
	{
		pPkt->UpdateParamPool(pActor);
		return;
	}
}
