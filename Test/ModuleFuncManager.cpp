#include "StdAfx.h"
#include "ModuleFuncManager.h"
#include "ServerManager.h"

ModuleFuncManager::ModuleFuncManager(PeerModuleBase& peerModule)
	: Servers( ServerManager::Instance()), PeerModule( peerModule )
{

}

ModuleFuncManager::~ModuleFuncManager(void)
{
}

void ModuleFuncManager::Send2Login(PacketBase& pkt)
{
	PeerModule.Send2Login(&pkt);
}

void ModuleFuncManager::Send2Login(PacketBase* pkt)
{
	PeerModule.Send2Login(pkt);
}
