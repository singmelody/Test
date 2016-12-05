#include "StdAfx.h"
#include "GateSystemManager.h"
#include "MyLog.h"
#include "PacketFactory.h"
#include "PacketBase.h"
#include "GateAccount.h"
#include "PacketImpl.h"
#include "GateServer.h"

GateSystemManager::GateSystemManager(void)
	: m_nLastSocketCnt(0)
{
}


GateSystemManager::~GateSystemManager(void)
{
}

void GateSystemManager::PrintPackets()
{
	MyLog::message("******************************");

	int32 nCnt = PacketFactory::Instance().GetFactoryCount();

	for (int32 i = 0; i < nCnt; ++i)
	{
		FactoryBase_Arg0* pFac = PacketFactory::Instance().GetFactory(i);
		if(!pFac)
			continue;

		const char* pName = pFac->ClassName();
		int32 nPacketID = -1;
		if( pName != NULL )
			nPacketID = UtilID::CreateFromString(pName);

		PacketBase* pPkt = (PacketBase*)pFac->New();
		if(!pPkt)
			continue;

		if( pFac->ClassSize() != pPkt->PacketClassSize() )
			MyLog::error("!!!!<Packet[%s] id[%d] Size Error>", pName, nPacketID);

		pFac->Free(pPkt);
	}
	 
	MyLog::message("******************************");

}

void GateSystemManager::Tick(int32 nFrameTime)
{
	ProcGateStressInfo(nFrameTime);
}

void GateSystemManager::ProcGateStressInfo(int32 nFrameTime)
{
	const uint32 nCurSocketCnt = int32(GateAccountManager::Instance().GetAccountCount());
	if( nCurSocketCnt == m_nLastSocketCnt )
		return;

	m_nLastSocketCnt = nCurSocketCnt;

	PacketGateStressInfo pkt;
	pkt.nGateID = GateSrv.GetSrvID();
	pkt.nSocketCnt = m_nLastSocketCnt;

	Send2Login(&pkt);
}
