#include "StdAfx.h"
#include "GateServer.h"
#include "GateAccountStates.h"

GateServer::GateServer(void)
{
	m_bUseConnectionThread = true;
}


GateServer::~GateServer(void)
{
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

}

void GateServer::RegCltPktHandle(PacketProcessor* pPkt)
{

}

void GateServer::DftCltPktHandle(PacketBase* pPkt)
{

}

bool GateServer::Init(int32 argc, char* argv[])
{
	GateAccountStateBase::Init();

	if(!GateBase::Init( argc, argv))
		return false;
}
