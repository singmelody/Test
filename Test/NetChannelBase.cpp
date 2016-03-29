#include "StdAfx.h"
#include "NetChannelBase.h"
#include "MyNetWork.h"
#include "MyLog.h"

NetChannelBase::NetChannelBase(void)
{
}


NetChannelBase::~NetChannelBase(void)
{
}

bool NetChannelBase::InitChannel(NetManager* pMgr, int32 nSockRcBuffSize, int32 nStreamRcSize, int32 nSockSnBuffSize, int32 nStreamSnSize,Socket* pSocket /*= 0*/)
{

}

bool NetChannelBase::OnWritePackets2Stream(PacketBase& pkt)
{
	bool bGood = true;
	while (bGood && m_queueSendingPacket.GetCount() > 0)
	{
		PacketBase* pPacket = m_queueSendingPacket.Pop_Head();
		assert( pPacket );

		bGood&= OnWritePackets2Stream(*pPacket);
		FACTORY_DEL_PACKET(pPacket);
		m_pMgr->PacketsSend().Add(1);
	}

	return bGood;
}

bool NetChannelBase::Fetch2Stream()
{
	assert( m_StreamIn.GetSpaceBeforeData() == 0 && m_StreamIn.GetSpaceAfterData() == m_StreamIn.GetSpace());

	int32 nSpace = (int32)m_StreamIn.GetSpaceAfterData();
	assert( nSpace >= 0 );
}

void NetChannelBase::HandleInput()
{
	if( m_state ==  CS_DISCONNECTED )
		return;

	for (;;)
	{
		m_StreamIn.MoveData2Head();
		if(!Fetch2Stream())
			break;

		assert( m_StreamIn.GetSpaceBeforeData() == 0);
		bool bNeedSpace( m_StreamIn.GetSpace() == 0);

		if(!OnParsePacketsFromStream())
			break;

		break;
	}
}

void NetChannelBase::HandleOutput()
{
	if( m_state == CS_DISCONNECTED )
		return;

	if( !OnWritePacktes2Stream() )
		return;

	flushStream();
}

void NetChannelBase::HandleException()
{
	MyLog::error("NetChannelBase::HandleException() channelid = %d", m_ID);
	DisConnect();
}

void NetChannelBase::HandleClose()
{
	MyLog::error("NetChannelBase::HandleClose() ChannelID = %d", m_ID);
	DisConnect();
}

bool NetChannelBase::Connect(char* sAddr, int32 nPort)
{
	SockAddr addr( sAddr, nPort);

	if(!m_socket.Create())
	{
		MyLog::error("Create Connect Socket Error");
		return false;
	}

	m_socket.SetNonBlocking(false);
	if(!m_socket.Connect(addr))
		return false;

	if(!m_socket.SetNonBlocking())
	{
		MyLog::error("SetNoneBlocking Failed!");
		return false;
	}

	m_state = CS_CONNECTED;
	return true;
}