#include "StdAfx.h"
#include "GatePktProc.h"
#include "GateServer.h"
#include "GateAccount.h"
#include "PacketFactory.h"
#include "PacketImpl.h"
#include "GateCltNetManager.h"

GatePeerPacketProc::GatePeerPacketProc()
{

}

bool GatePeerPacketProc::OnPacketReceived(PacketBase* pPkt)
{
	if( pPkt->CheckPacketType( ePacketType_GateProc) || pPkt->CheckPacketType(ePacketType_BroadCast) )
	{
		MyPacketProc::OnPacketReceived(pPkt);
		return true;
	}

	static const int32 PACKET_ID_OF_PacketMulticast2Channel = PacketMulticast2Channel::GetClassStatic()->ClassID();
	if( pPkt->GetPacketID() == PACKET_ID_OF_PacketMulticast2Channel )
	{
		PacketMulticast2Channel* pPktBC = (PacketMulticast2Channel*)pPkt;

		PacketBase* pPktInside = pPktBC->GetPacket();

		int16 channelList[PACKET_SEND_LIST_BUFFER_SIZE/sizeof(int16)];
		uint16 channelCnt = pPktBC->GetCastList( channelList );

		static const int32 PACKET_ID_OF_PacketFSMExtendData = PacketFSMExtendData::GetClassStatic()->ClassID();
		if( pPktInside->GetPacketID() == PACKET_ID_OF_PacketFSMExtendData )
			ProcessFSMExtendDataPacket( (PacketFSMExtendData*)pPktInside, channelCnt, channelList);
		else
		{
			for (int32 i = 0; i < channelCnt; ++i)
			{
				int32 nChannelID = channelList[i];
				if( nChannelID >= 0 )
					GateSrv.SrvSend( nChannelID, pPktInside);
			}
		}

		PacketFactory::Instance().Delete( pPkt );
		return true;
	}

	int32 nAvatarID = pPkt->GetAvatarID();
	if( nAvatarID > 0 )
	{
		GateCltNetChannel* pChannel = AccountMgr.GetCltChannelByAvatarID(nAvatarID);
		if( pChannel)
		{
			pChannel->AppendPacket(pPkt);
			GateSrv.FreeCltChannel(pChannel);
			return true;
		}
	}

	return MyPacketProc::OnPacketReceived(pPkt);
}

GateCltPacketProc::GateCltPacketProc()
{

}

int32 GateCltPacketProc::DoProc()
{
	int32 nCnt = 0;
	for (; nCnt < PACKET_PROCESS_COUNT_PER_TICK; ++nCnt)
	{
		PacketBase* pPkt = m_receivingPktList.Pop_Head();
		if(!pPkt)
			break;

		ProcessPacket( pPkt );

		PacketFactory::Instance().Delete(pPkt);
	}

	return nCnt;
}
