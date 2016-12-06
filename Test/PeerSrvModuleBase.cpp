#include "StdAfx.h"
#include "PeerSrvModuleBase.h"


PeerSrvModuleBase::PeerSrvModuleBase(SrvType nModuelType)
	: PeerModuleBase(nModuelType)
{
}


PeerSrvModuleBase::~PeerSrvModuleBase(void)
{
}

bool PeerSrvModuleBase::Init(int32 nArgc, char* argv[])
{
	if(!PeerModuleBase::Init( nArgc, argv))
		return false;

	if(!SrvInit())
		return false;

	return true;
}

void PeerSrvModuleBase::Exit()
{
	SrvExit();

	PeerModuleBase::Exit();
}

bool PeerSrvModuleBase::ProcessPacket()
{
	int32 nPacketProcessed = 0;

	nPacketProcessed += PeerProcPacket();
	nPacketProcessed += ProcCltPacket();

	return nPacketProcessed > 0;
}
