#include "StdAfx.h"
#include "GateLoginManager.h"
#include "PacketImpl.h"
#include "PacketProcessor.h"
#include "GateAccountStates.h"
#include "MyLog.h"
#include "GateServer.h"
#include "GateAccountStates.h"
#include "PacketImpl.h"

FINISH_FACTORY_ARG0(PasskeyInfo);
PasskeyInfo::PasskeyInfo()
{
	nExpireTime = 0;
	nSocketID = 0;
}

GateLoginManager::GateLoginManager(void)
{
}


GateLoginManager::~GateLoginManager(void)
{
}

void GateLoginManager::RegPeerPktHandle(class PacketProcessor* pProc)
{
// 	REG_PACKET_HANDLER();
// 	REG_PACKET_HANDLER();
}

void GateLoginManager::RegCltPktHandle(class PacketProcessor* pProc)
{
	REG_PACKET_HANDLER( pProc, PacketConnectGateSrv, GateLoginManager, PktClt_ConnectGateSrv);
	REG_PACKET_HANDLER( pProc, PacketCltReady, GateLoginManager, PktClt_CltReady);

	REG_PACKET_HANDLER( pProc, PacketCreateAvatar, GateLoginManager, PktClt_CreateAvatar);
	REG_PACKET_HANDLER( pProc, PacketDelCharacter, GateLoginManager, PktClt_DelCharacter);
	REG_PACKET_HANDLER( pProc, PacketCltSelectAvatar, GateLoginManager, PktClt_CltSelectAvatar);
	REG_PACKET_HANDLER( pProc, PacketLogout2Login, GateLoginManager, PktClt_Logout2Login);
}

void GateLoginManager::PktClt_ConnectGateSrv(class PacketConnectGateSrv* pPkt)
{
	assert(pPkt);
	HandleCltPacket(*pPkt);
}

void GateLoginManager::PktClt_CltReady(class PacketCltReady *pPkt)
{
	assert(pPkt);
	HandleCltPacket(*pPkt);
}

void GateLoginManager::PktClt_CreateAvatar(class PacketCreateAvatar *pPkt)
{
	assert(pPkt);
	HandleCltPacket(*pPkt);
}

void GateLoginManager::PktClt_DelCharacter(class PacketDelCharacter *pPkt)
{
	assert(pPkt);
	HandleCltPacket(*pPkt);
}

void GateLoginManager::PktClt_CltSelectAvatar(class PacketCltSelectAvatar *pPkt)
{
	assert(pPkt);
	HandleCltPacket(*pPkt);
}

void GateLoginManager::PktClt_Logout2Login(class PacketLogout2Login *pPkt)
{
	assert(pPkt);
	HandleCltPacket(*pPkt);
}

void GateLoginManager::Tick(int32 nFrameTime)
{
	ProcPassKeyInfo(nFrameTime);
}

void GateLoginManager::ProcPassKeyInfo(int32 nFrameTime)
{
	std::vector<UtilID> expireWaits;

	PasskeyInfoMap& map = m_PasskeyInfos;

	PasskeyInfoMap::iterator itrEnd = map.end();

	for (auto itr = map.begin(); itr != itrEnd; ++itr)
	{
		PasskeyInfo* pPasskey = itr->second;

		pPasskey->nExpireTime -= nFrameTime;

		if( pPasskey->nExpireTime <= 0 )
		{
			OnLoseClt();
			FACTORY_DELOBJ(pPasskey);
			itr->second = NULL;
			expireWaits.push_back(itr->first);
		}
	}
}

void GateLoginManager::OnLoseClt()
{
	PacketGateLoseClt pkt;
	pkt.nGateID = GateSrv.GetSrvID();
	Send2Login(&pkt);
}

void GateLoginManager::ProcessCltDisconnect(int32 nSocketID)
{
	GateAccount* pAccount = GateAccountManager::Instance().GetAccount(nSocketID);
	if(!pAccount)
		return;

	pAccount->OnCltDisconnect();
}
