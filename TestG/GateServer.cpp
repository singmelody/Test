#include "StdAfx.h"
#include "GateServer.h"
#include "GateAccountStates.h"
#include "GatePktProc.h"
#include "LoadTemplateManager.h"
#include "MyLog.h"
#include "FSMExtendData.h"
#include "GateChatManager.h"
#include "GateSystemManager.h"
#include "GateLoginManager.h"
#include "GateAccount.h"

GateServer::GateServer(void)
{
	m_bUseConnectionThread = true;
}


GateServer::~GateServer(void)
{
}

void GateServer::ProcessLogic(int32 nFrameTime)
{
	GateBase::ProcessLogic(nFrameTime);

	ProcNewConnection(nFrameTime);

	GateSystemManager::Instance().Tick(nFrameTime);
	GateLoginManager::Instance().Tick(nFrameTime);
	GateAccountManager::Instance().Tick(nFrameTime);
}

MyPacketProc* GateServer::CreatePeerPktProc()
{
	return new GatePeerPacketProc();
}

void GateServer::RegPeerPktHandle(class PacketProcessor* pProc)
{

}

void GateServer::DftPeerPktHandle(PacketBase* pkt)
{

}

MyPacketProc* GateServer::CreateCltPktProcessor()
{
	return new GateCltPacketProc();
}

void GateServer::RegCltPktHandle(PacketProcessor* pPkt)
{

}

void GateServer::DftCltPktHandle(PacketBase* pPkt)
{

}

void GateServer::OnCltDisconnect(int32 nSocketID)
{

}

bool GateServer::Init(int32 argc, char* argv[])
{
	GateAccountStateBase::Init();

	if(!GateBase::Init( argc, argv))
		return false;

	Templates.Load("GateServer");
	GateSystemManager::Instance().PrintPackets();

	{
		if( m_SrvMaxSockets > MY_SOCKET_LIST_SIZE )
		{
			MyLog::error("SrvMaxSockets(%d) > MY_SOCKET_LIST_SIZE(%d)", m_SrvMaxSockets, MY_SOCKET_LIST_SIZE);
			return false;
		}
	}

	FSMExtendData data;

	GateChatManager::Instance().Init();

	return true;
}
