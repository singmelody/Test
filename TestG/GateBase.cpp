#include "StdAfx.h"
#include "GateBase.h"
#include "GateCltNetManager.h"
#include "MyLog.h"

GateBase::GateBase(void)
	: PeerSrvModuleBase(eSrv_Gate)
	, m_bSrvReady(false)
	, m_nWarningCachedPacket2Clt(100)
	, m_nMaxCachedPacket2Clt(5000)
{
	m_nGateTickTime = 20;
}


GateBase::~GateBase(void)
{
}


GateCltNetChannel* GateBase::GetCltChannel(int32 nSocketID)
{
	GateCltNetChannel* pChannel = (GateCltNetChannel*)m_SrvNetMgr->GetChannel(nSocketID);
	return pChannel;
}

void GateBase::FreeCltChannel(GateCltNetChannel* pChannel)
{
	m_SrvNetMgr->FreeChannel(pChannel);
}

void GateBase::OnRemoveWorldInfo(ServerInfo* pInfo)
{
	MyLog::message("World Srv Lose ! will Exit !");
	StopMainLoop();
}
