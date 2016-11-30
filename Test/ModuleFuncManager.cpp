#include "StdAfx.h"
#include "ModuleFuncManager.h"
#include "ServerManager.h"
#include "PeerModuleBase.h"

ModuleFuncManager::ModuleFuncManager(PeerModuleBase& peerModule)
	: Servers( ServerManager::Instance()), PeerModule( peerModule )
{

}

ModuleFuncManager::~ModuleFuncManager(void)
{
}

void ModuleFuncManager::Send2Node(PacketBase& pkt, int32 nSrvID)
{
	PeerModule.Send2Node( &pkt, nSrvID);
}

void ModuleFuncManager::Send2Node(PacketBase* pkt, int32 nSrvID)
{
	PeerModule.Send2Node( pkt, nSrvID);
}

void ModuleFuncManager::Send2Gate(PacketBase& pkt, int32 nSrvID, bool bGateProc /*= false*/)
{
	PeerModule.Send2Gate( &pkt, nSrvID, bGateProc);
}

void ModuleFuncManager::Send2Gate(PacketBase* pkt, int32 nSrvID, bool bGateProc /*= false*/)
{
	PeerModule.Send2Gate( pkt, nSrvID, bGateProc);
}

void ModuleFuncManager::Send2Login(PacketBase& pkt)
{
	PeerModule.Send2Login(&pkt);
}

void ModuleFuncManager::Send2Login(PacketBase* pkt)
{
	PeerModule.Send2Login(pkt);
}

void ModuleFuncManager::Send2DBA(PacketBase& pkt)
{
	PeerModule.Send2DBA(&pkt);
}

void ModuleFuncManager::Send2DBA(PacketBase* pkt)
{
	PeerModule.Send2DBA(pkt);
}

void ModuleFuncManager::PeerSend(int32 nSocketID, PacketBase& pkt)
{
	PeerModule.PeerSend( &pkt, nSocketID);
}

void ModuleFuncManager::PeerSend(int32 nSocketID, PacketBase* pkt)
{
	PeerModule.PeerSend( pkt, nSocketID);
}
