#include "StdAfx.h"
#include "MyPacketProc.h"
#include "PacketFactory.h"

MyPacketProc::MyPacketProc(void)
{
}


MyPacketProc::~MyPacketProc(void)
{
}

int32 MyPacketProc::DoProc()
{
	int32 nProcCnt = 0;

	PacketBase* pPkt = m_receivingPktList.Pop_Head();
	while(pPkt)
	{
		ProcessPacket(pPkt);

		PacketFactory::Instance().Delete( pPkt );
		nProcCnt++;

		if( nProcCnt >= PACKET_PROCESS_COUNT_PER_TICK )
			break;

		pPkt = m_receivingPktList.Pop_Head();
	}

	return nProcCnt;
}
