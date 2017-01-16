#include "StdAfx.h"
#include "MyClt.h"


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
	DWORD nParam = pPkt->nParamType;
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


}

void MyClt::UpdateAvatarData(class PacketUpdateAvatarData* pPkt)
{

}
