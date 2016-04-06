operator=
operator=
operator=
#include "StdAfx.h"
#include "NetChannelBase.h"
#include "MyNetWork.h"
#include "MyLog.h"
#include "PacketFactory.h"
#include <assert.h>
#include "NetManager.h"

NetChannelBase::NetChannelBase(void)
{
}


NetChannelBase::~NetChannelBase(void)
{
}

bool NetChannelBase::InitChannel(NetManager* pMgr, int32 nSockRcBuffSize, int32 nStreamRcSize, int32 nSockSnBuffSize, int32 nStreamSnSize,Socket* pSocket /*= 0*/)
{
	assert( pMgr );
	m_pMgr = pMgr;

	m_StreamIn.Allocate(nStreamRcSize);
	m_StreamOut.Allocate(nStreamSnSize);

	assert(!m_pLZOCompressor);
	if(m_pMgr->Compressable())
		m_pLZOCompressor = FACTORY_NEWOBJ(LZOCompressor);

	m_sockRcBuffSize = nSockRcBuffSize;
	m_sockRnBuffSize = nSockSnBuffSize;

	if(!pSocket)
		m_socket.Shift(*pSocket);

	m_socket.SetRecvBuffSize(nSockRcBuffSize);
	m_socket.SetSendBuffSize(nSockSnBuffSize);
	m_socket.SetNonBlocking();
	m_socket.SetNonDelay();

	return true;
}

bool NetChannelBase::OnWritePackets2Stream()
{
	bool bGood = true;
	while (bGood && m_queueSendingPacket.GetCount() > 0)
	{
		PacketBase* pPacket = m_queueSendingPacket.Pop_Head();
		assert( pPacket );

		bGood&= OnWritePacket2Stream(*pPacket);
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

	if( nSpace <= 0 )
		return true;

	char* pBuffer = (char*)m_StreamIn.GetBuffer();
	int32 nRet = RecvData( pBuffer, nSpace);

	if(nRet < 0)
	{
		DisConnect();
		return false;
	}

	m_StreamIn.ReserveForWrite(nRet);
	return true;
}

int32 NetChannelBase::RecvData(char* buffer, int32 nLen)
{
	int32 nRet = m_socket.Recv( buffer, nLen, 0);
	if(nRet < 0)
	{
		int32 nError = Socket::GetSysError();
		if( MY_EWOULDBLOCK == nError || MY_EAGAIN == nError )
			return 0;
		else if( MY_ECONNRESET == nError )
		{
			SockAddr addr;
			m_socket.GetPeerAddr(addr);
			MyLog::error("NetChannelBase::recvData() [%s:%u][%u]  An existing connection was forcibly closed by thre remote host!",  addr.GetIP().c_str(), addr.GetPort(), GetID());
			return -1;
		}
		else
		{
			SockAddr addr;
			m_socket.GetSockAddr(addr);
			MyLog::error("[%s:%u][%u] IO error [%d] when sending data !",
				addr.GetIP().c_str(), addr.GetPort(), GetID(), nError);

			return -1;
		}
	}

	if(nRet == 0)
	{
		SockAddr addr;
		m_socket.GetPeerAddr(addr);
		MyLog::error("NetChannelBase::recvData() [%s:%u][%u] remote connection is closed!", 
			addr.GetIP().c_str(), addr.GetPort(), GetID());
		return -1;
	}

	m_pMgr->BytesRecv().Add(nRet);
	return nRet;
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

	if( !OnWritePackets2Stream() )
		return;

	FlushStream();
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