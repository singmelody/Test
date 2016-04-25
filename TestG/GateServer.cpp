#include "StdAfx.h"
#include "GateServer.h"


GateServer::GateServer(void)
{
	m_bUseConnectionThread = true;
}


GateServer::~GateServer(void)
{
}

bool GateServer::Init(int32 argc, char* argv[])
{
	GateAccountStateBase::Init();

	if(!GateBase::Init())
}
