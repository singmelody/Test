#include "StdAfx.h"
#include "NetManager.h"
#include "NetChannel.h"

NetManager::NetManager(void)
{
}


NetManager::~NetManager(void)
{
}

void NetManager::NetRun()
{
	ProcNewConnection();
	ProcPendingClose();
	ProcPendingDestroy();
}

void NetManager::Accept( char* sAddr, int32 nPort, bool bSrv)
{

}

int32 NetManager::Connect( char* sAddr, int32 nPort, bool bSingleThread)
{
	NetChannelBase* pBase = CreateNewChannel();
	if(!pBase)
		return -1;



	return -1;
}

NetChannelBase* NetManager::CreateNewChannel()
{
	return new NetChannel();
}

void NetManager::ProcNewConnection()
{

}

void NetManager::ProcPendingClose()
{

}

void NetManager::ProcPendingDestroy()
{

}
