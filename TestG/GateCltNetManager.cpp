#include "StdAfx.h"
#include "GateCltNetManager.h"
#include "TimeManager.h"
#include "PacketImpl.h"
#include "MyLog.h"
#include "GateServer.h"
#include "FSMExtendData.h"

GateCltNetManager::GateCltNetManager(bool bLZOCompresss, int32 sockRcBuffSize, int32 rcBuffSize, int32 sockSnBuffSize, int32 snBuffSize, FunctionBase_Arg1<int32>* funcAccept /*= NULL*/, FunctionBase_Arg1<int32>* funcCon /*= NULL*/, FunctionBase_Arg1<int32>* funcDisCon /*= NULL*/, int32 MAX_SOCKETS)
	: BASENETMANAGER( bLZOCompresss, sockRcBuffSize, rcBuffSize, sockSnBuffSize, snBuffSize, funcAccept, funcCon, funcDisCon, MAX_SOCKETS)
{

}

GateCltNetManager::~GateCltNetManager(void)
{

}


NetChannelBase* GateCltNetManager::CreateNewChannel()
{
	return new GateCltNetChannel();
}

void GateCltNetManager::OnReceivedPacket(NetChannelBase* pChannel, PacketBase* pPkt)
{
	const uint32 nPktAttr = pPkt->GetStaticAttribute();

	GateCltNetChannel* pCltChannel = (GateCltNetChannel*)pChannel;

	static const uint32 nPeerMask = PacketBase::PktAttr_IsClt2Node | PacketBase::PktAttr_IsClt2World;

	if( (nPeerMask & nPktAttr) != 0 )
	{
		int32 nPeerSockID = -1;

		if(PacketBase::IsClt2Node(nPktAttr))
		{
			nPeerSockID = pCltChannel->m_nNodeSocketID;
		}
		else
		{
			nPeerSockID = pCltChannel->m_nWorldSocketID;
		}

		if(!Dispatch2PeerChannel( nPeerSockID, pPkt))
			FACTORY_DEL_PACKET(pPkt);

		return;
	}

	if(pCltChannel->ProcessPrivatePakcet(pPkt))
	{
		FACTORY_DEL_PACKET(pPkt);
		return;
	}

	m_pProcessor->OnPacketReceived(pPkt);
}

bool GateCltNetManager::Dispatch2PeerChannel(int32 nChannel, PacketBase* pPkt)
{
	if( nChannel < 0 )
		return false;

	NetChannelBase* pChannel = GateSrv.GetPeerChannel(nChannel);
	if(!pChannel)
		return false;

	pChannel->AppendPacket(pPkt);
	GateSrv.FreePeerChannel(pChannel);

	return true;
}

GateCltNetChannel::GateCltNetChannel()
{
	m_nAvatarID = 0;

	m_nNodeSocketID = -1;
	m_nWorldSocketID = -1;

	m_nCurCltPacketCnt = 0;
	m_nCurCltPacketTime = TimeManager::Instance().CurTime();
}

bool GateCltNetChannel::ProcessFSMExtendDatas(FSMExtendDataList& list)
{
	PacketFSMExtendData* pPkt = NULL;

	for(FSMExtendDataList::iterator itr = list.begin(); itr != list.end(); ++itr)
	{
		FSMExtendData* pData = *itr;

		const int32 nOwnerID = pData->m_OwnerID;

		if(nOwnerID == m_nAvatarID || IsInAOI( nOwnerID ))
		{
			if(!pPkt)
				pPkt = FACTORY_NEW_PACKET(PacketFSMExtendData);

			if( pPkt != NULL)
				pPkt->FillData(pData);
		}
		else
		{

		}
	}

	if(pPkt)
		AppendPacket(pPkt);

	return true;
}

bool GateCltNetChannel::ProcessPrivatePakcet(PacketBase* pPkt)
{
	static const int32 PACKET_ID_OF_PacketAOIChange = PacketAOIListOption::GetClassStatic()->ClassID();
	if( pPkt->GetPacketID() == PACKET_ID_OF_PacketAOIChange )
	{
		OnAOIListOption( (PacketAOIListOption*)pPkt);
		return true;
	}

	return false;
}

void GateCltNetChannel::AppendPacket(PacketBase* pPkt)
{
	NetChannel::AppendPacket(pPkt);

	const int32 nCachedPacket = m_queueSendingPacket.GetCount();
	if( nCachedPacket > GateServer::Instance().GetMaxCachedPacket2Clt())
	{
		MyLog::message("GateCltNetChannel::AppendPacket too many cached on channel=[%d] cachedpacket=[%d]", GetID(), nCachedPacket);
		DisConnect();
		return;
	}

	if( nCachedPacket > GateServer::Instance().GetWarningCachedPacket2Clt() )
	{
		MyLog::message("GateCltNetChannel::OnWritePackets2Stream() Channel=[%d] cachedPacket=[%d]", GetID(), nCachedPacket);;
	}
}

void GateCltNetChannel::OnPacketParsed(PacketBase* pPkt)
{
	if( CheckPacketFrequence() )
	{
		DisConnect();	// 发送太过频繁 断开链接
	}

	pPkt->SetAvatarID( m_nAvatarID );
	NetChannel::OnPacketParsed(pPkt);
}

bool GateCltNetChannel::CheckPacketFrequence()
{
	m_nCurCltPacketCnt++;

	if( m_nCurCltPacketCnt >= Clt_Packet_Send_Freq )
	{
		m_nCurCltPacketCnt = 0;
		uint64 nCurTime = TimeManager::Instance().CurTime();

		if( (nCurTime - m_nCurCltPacketTime) < (Clt_Packet_Check_Time*1000))
		{
			m_nCurCltPacketTime = nCurTime;
			return true;
		}
	}

	return false;
}

void GateCltNetChannel::OnAOIListOption(class PacketAOIListOption* pPkt)
{
	AOISet& set = m_setAOI;

	switch(pPkt->nFlag)
	{
	case PacketAOIListOption::eClear:
		set.clear();
		break;
	case PacketAOIListOption::eAdd:
		set.insert( pPkt->nTargetAvatarID );
		break;
	case PacketAOIListOption::eRemove:
		set.erase( pPkt->nTargetAvatarID );
		break;
	}
}

bool GateCltNetChannel::IsInAOI(int32 nAvatarID)
{
	return m_setAOI.find(nAvatarID) != m_setAOI.end();
}
