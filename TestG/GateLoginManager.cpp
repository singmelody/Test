#include "StdAfx.h"
#include "GateLoginManager.h"
#include "PacketImpl.h"
#include "PacketProcessor.h"
#include "GateAccountStates.h"
#include "MyLog.h"
#include "GateServer.h"
#include "GateAccountStates.h"


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
