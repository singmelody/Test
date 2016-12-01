#include "StdAfx.h"
#include "GateFuncManager.h"
#include "GateServer.h"

GateFuncManager::GateFuncManager(void) :
	ModuleFuncManager(GateServer::Instance()),
	GateSrv(GateServer::Instance())
{
}


GateFuncManager::~GateFuncManager(void)
{
}

void GateFuncManager::SrvSend(int32 nSocketID, PacketBase& pkt)
{
	GateSrv.SrvSend( nSocketID, &pkt);
}

void GateFuncManager::SrvSend(int32 nSocketID, PacketBase* pPkt)
{
	GateSrv.SrvSend( nSocketID, pPkt);
}
