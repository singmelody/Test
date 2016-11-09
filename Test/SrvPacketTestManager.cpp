#include "StdAfx.h"
#include "SrvPacketTestManager.h"
#include "PacketFactory.h"
#include "PacketBase.h"
#include "MyRandom.h"

SrvPacketTestManager::SrvPacketTestManager(void)
{
	m_bPeerSend = m_bCltSend = false;
	m_InvTime = 5000;
	m_CurTime = 0;
}


SrvPacketTestManager::~SrvPacketTestManager(void)
{
}

void SrvPacketTestManager::TestSend2Clt(class PacketBase* pkt)
{
	if(!m_bPeerSend && !m_bCltSend)
		return;


}

void SrvPacketTestManager::TestSend2Peer(class PacketBase* pkt)
{

}

void SrvPacketTestManager::TickSendTestPacket(int32 nFrameTime)
{

}

void SrvPacketTestManager::DoSend()
{
	char pktBuff[PACKET_MAX_SIZE];

	for(int32 i = 0; i < PACKET_MAX_SIZE; ++i)
	{
		pktBuff[i] = char(MyRandom::Instance().RandInt(-128,128));
	}

	int32 nCount = PacketFactory::Instance().GetFactoryCount();
	for(int32 i = 0; i < nCount; ++i)
	{
		FactoryBase_Arg0* pFac = PacketFactory::Instance().GetFactory(i);
		if(!pFac)
			continue;

		const char* sName = pFac->ClassName();

		int32 nPacketID = -1;
		if(sName != NULL)
			nPacketID = UtilID::CreateFromString(sName);

		PacketBase* pPkt = (PacketBase*)pFac->New();
		if(!pPkt)
			continue;

		pPkt->ReadPacket(pktBuff);
		pPkt->SetPacketID(nPacketID);

		if(m_bCltSend)
			TestSend2Clt(pPkt);

		if(m_bPeerSend)
			TestSend2Peer(pPkt);

		pFac->Free(pPkt);
	}
}
