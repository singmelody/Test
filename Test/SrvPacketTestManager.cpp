#include "StdAfx.h"
#include "SrvPacketTestManager.h"
#include "PacketFactory.h"

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
		pktBuff[i] = char(Random::Instance().Rand(-128,128));
	}

	int32 nCount = PacketFactory::Instance().GetFactoryCount();
	for(int32 i = 0; i < nCount; ++i)
	{

	}
}
