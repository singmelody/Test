#include "StdAfx.h"
#include "ServerManager.h"


ServerManager::ServerManager(void)
{
}


ServerManager::~ServerManager(void)
{
}

void ServerManager::SetPeerModule(PeerModuleBase* pPeerModule)
{
	m_pPeerModule = pPeerModule;
}
