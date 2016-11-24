#include "StdAfx.h"
#include "NetChannel.h"
#include "NetManager.h"
#include "PacketFactory.h"
#include "LZOCompressor.h"
#include "MyLog.h"
#include <assert.h>

NetChannel::NetChannel(void)
{
	m_nIndexOfSend = 0;
	m_nIndexOfRecv = 0;

	m_pRecvBuffer = NULL;
	m_cbData2Recv = 0;
	m_cbDataRecved = 0;

	m_pSendBuffer = NULL;
	m_cbData2Send = 0;
	m_cbDataSended = 0;

	m_BlockHead = 0;
	m_bHasStartSending = false;

	m_totalSendByte = 0;
	m_totalRecvByte = 0;
}

NetChannel::~NetChannel(void)
{
	SAFE_DELETE_ARRAY( m_pRecvBuffer );
}

bool NetChannel::InitChannel(NetManager* pMgr, int32 nSockRcBuffSize, int32 nStreamRcSize, int32 nSockSnBuffSize, int32 nStreamSnSize,Socket* pSocket)
{
	if(!NetChannelBase::InitChannel( pMgr, nSockRcBuffSize, nStreamRcSize, nSockSnBuffSize, nStreamSnSize, pSocket))
		return false;

	m_hExitSending.CreateEvent( true, true);
	m_hExitReceiving.CreateEvent( true, false);

	return true;
}

bool NetChannel::StartChannel()
{
	if(!NetChannelBase::StartChannel())
		return false;

	assert( m_hExitSending.IsValid() && m_hExitReceiving.IsValid() );

	m_socket.SetNonBlocking(false);

	if(!BindIoCompletionCallback( (HANDLE)m_socket.GetSocket(), AsynIoCompleteProc, 0))
		return false;

	m_pRecvBuffer = new BYTE[m_sockRcBuffSize];
	if( !m_pRecvBuffer)
		return false;

	{
		MYOVERLAPPED& olp = m_OLPSend;
		memset( &olp, 0, sizeof(MYOVERLAPPED));

		olp.pChannel = this;
		olp.bSend = true;
	}

	{
		MYOVERLAPPED& olp = m_OLPRecv;
		memset( &olp, 0, sizeof(MYOVERLAPPED));
		olp.pChannel = this;
		olp.bSend = false;
	}

	if(!StartNewRecv( sizeof( BlockHeadT)))
		return false;

	return true;
}

void NetChannel::CloseChannel()
{
	m_socket.Close();

	m_hExitReceiving.Wait();
	m_hExitSending.Wait();
}

bool NetChannel::FillPackets2Block(DataBufferArg& arg)
{
	const uint32 cbHeader = sizeof(BlockHeadT);

	uint32 cbBuffer = CBBUFF;
	if( arg.cbBuffer < cbBuffer )
		cbBuffer = arg.cbBuffer;


	if( cbBuffer > BLOCK_SIZE_MASK )
		cbBuffer = BLOCK_SIZE_MASK;

	if( cbBuffer > SOCKET_MY_MAX_DATA_BLOCK_SIZE )
		cbBuffer = SOCKET_MY_MAX_DATA_BLOCK_SIZE;

	char* pBuffer = m_sendPacketBuffer + cbHeader;
	char* pBuffer2 = m_SendPacketBuffer2 + cbHeader;
	cbBuffer -= cbHeader;

	uint32 cbData = 0;

	// 开始合并数据包处理
	bool bFirstPacket = true;
	bool bEncrypt = true;

	PacketList& list = m_queueSendingPacket;

	int64 nPacketSend = 0;
	PacketBase* pPkt = list.Pop_Head();
	while ( pPkt != NULL )
	{
		pPkt->IsUseIndex( m_pMgr->UseIndexWhenSend() );
		pPkt->SetPacketIndex( m_nIndexOfSend );

		const uint32 PacketAttr = pPkt->GetStaticAttribute();
		if( PacketBase::IsDisconnectCommand( PacketAttr ) )
		{
			DisConnect();
			FACTORY_DEL_PACKET( pPkt );
			return false;
		}

		if( bFirstPacket )
		{
			bEncrypt = PacketBase::ShouldEncrypt( PacketAttr );
			bFirstPacket = false;
		}
		else
		{
			if( bEncrypt != PacketBase::ShouldEncrypt( PacketAttr ) )
				break;
		}

		char pktContent[PACKET_MAX_SIZE];
		size_t nPktSize = pPkt->WritePacket( pktContent )  - pktContent;
		assert( nPktSize < PACKET_MAX_SIZE && nPktSize > 0);

		if(cbData + nPktSize > cbBuffer)
			break;

		memcpy( pBuffer + cbData, pktContent, nPktSize);
		nPacketSend++;

		++m_nIndexOfSend;

		cbData += (uint32)nPktSize;
		FACTORY_DEL_PACKET( pPkt );
		pPkt = list.Pop_Head();
	}

	m_pMgr->PacketsSend().Add(nPacketSend);

	if(pPkt != NULL)
		list.Push_Head(pPkt);

	if( cbData == 0 )
		return false;

	bool bCompress = false;
	BlockHeadT blockFlag = 0;

#if LZO_COMPRESS
	if( m_pGLZOCompressor )
	{
		if( bCompress && cbData > 56 )
		{
			uint32 dstLen = cbBuffer;
			if(!m_pGLZOCompressor->Compress( pBuffer, cbData, pBuffer2, dstLen) )
			{
				assert(false);
				MyLog::error("Socket send packet size error, Compress Fail! sockid=[%d] size=[%u]!Will disconnect", GetID(), cbData);
				DisConnect();
				return false;
			}

			if( dstLen < cbData )
			{
				blockFlag |= BLOCK_FLAG_LZO;
				std::swap( pBuffer, pBuffer2);
				cbData = dstLen;
			}
		}
	}
#endif

#if USE_MY_BLOWFISH
	if(m_pBlowFishCipher)
	{
		if(bEncrypt)
		{
			blockFlag |= Block_FLAG_ENC;
			m_pBlowFishCipher->Encrypt( pBuffer, pBuffer2, nDataLen);
			std::swap( pBuffer, pBuffer2);
		}
	}
#endif

	if( cbData > BLOCK_SIZE_MASK )
	{
		MyLog::error("NetChannel::FillPackets2Block() Error , cbData > BLOCK_SIZE_MASK\n");
	}

	arg.pBuffer = pBuffer - cbHeader;
	arg.cbData = cbHeader + cbData;

	BlockHeadT blockHead = (BlockHeadT)(cbData);
	blockHead |= blockFlag;

	memcpy( arg.pBuffer, &blockHead, cbHeader);

	return true;
}

bool NetChannel::OnAsynSendComplete(DWORD dwTransed)
{
	if( dwTransed > 0 )
	{
		m_cbDataSended += dwTransed;
		if( m_cbDataSended < m_cbData2Send )
		{
			DWORD cbLeft = m_cbData2Send - m_cbDataSended;
			PBYTE pBuffer = m_pSendBuffer + m_cbDataSended;
			AsynSend( pBuffer, cbLeft);
			return true;
		}
	}

	// 发送正常
	m_pSendBuffer = NULL;
	m_cbData2Send = 0;
	m_cbDataSended = 0;

	if( !TryAsynSendPackets() )
		return false;

	return true;
}

bool NetChannel::OnAsynRecvComplete(DWORD dwTransed)
{
	if(dwTransed == 0)
		return false;

	m_cbDataRecved += dwTransed;

	MYOVERLAPPED* pMyOLP = &m_OLPRecv;
	if( m_cbDataRecved < m_cbData2Recv )
	{
		DWORD cbLeft = m_cbData2Recv - m_cbDataRecved;
		PBYTE pBuffer = m_pRecvBuffer + m_cbDataRecved;
		AsynRecv( pBuffer, cbLeft);
	}
	else
	{
		if (m_BlockHead == 0)
		{
			m_BlockHead = *((BlockHeadT*)m_pRecvBuffer);
			uint32 nBlockSize = ( m_BlockHead & BLOCK_SIZE_MASK);

			if( nBlockSize > (uint32)m_sockRcBuffSize)
				return false;

			if( nBlockSize == 0 )
				return false;

			StartNewRecv( nBlockSize );
		}
		else
		{
			ParsePacketsInBlock( m_BlockHead, (char*)m_pRecvBuffer, m_cbData2Recv);
			m_BlockHead = 0;
			StartNewRecv( sizeof(BlockHeadT) );
		}
	}

	return true;
}



void NetChannel::OnPacketParsed(PacketBase* pPkt)
{
	if( !m_pMgr)
		return;

	m_pMgr->OnReceivedPacket( this, pPkt);
}

void NetChannel::AsynSend(PBYTE pData, long lDataSize)
{
	MYOVERLAPPED& olp = m_OLPSend;

	WSABUF wsaBuf;
	wsaBuf.buf = LPSTR(pData);
	wsaBuf.len = lDataSize;

	DWORD dwTransed = 0;
	DWORD dwFlags = 0;

	int32 nResult = WSASend( m_socket.GetSocket(), &wsaBuf, 1, &dwTransed, dwFlags, (LPWSAOVERLAPPED)&olp, NULL);
	if( nResult != 0)
	{
		if( WSAGetLastError() != WSA_IO_PENDING )
		{
			OnExitSending();
			DisConnect();
			return;
		}
	}

	m_pMgr->BytesSend().Add(int64(lDataSize));
}

bool NetChannel::AsynRecv(PBYTE pData, long lDataSize)
{
	if( pData == NULL || lDataSize <= 0)
		return false;

	MYOVERLAPPED& olp = m_OLPRecv;

	WSABUF wsabuf;
	wsabuf.buf = LPSTR( pData );
	wsabuf.len = lDataSize;

	DWORD dwTransed = 0;
	DWORD dwFlags = 0;

	if ( WSARecv( m_socket.GetSocket(), &wsabuf, 1, &dwTransed, &dwFlags, (LPWSAOVERLAPPED)&olp, NULL ) != 0)
	{
		int32 nError = WSAGetLastError();
		if( nError != WSA_IO_PENDING )
		{
			OnExitReceiving();
			return false;
		}
	}

	m_pMgr->BytesRecv().Add(lDataSize);
	return true;
}

void NetChannel::TryStartSending()
{
	AUTOLOCK( m_MutexSending );

	if(m_queueSendingPacket.GetCount() > 0)
	{
		if(!m_bHasStartSending)
		{
			m_bHasStartSending = true;
			m_hExitSending.ResetEvent();
			AsynSend( NULL, 0);
		}
	}
}

bool NetChannel::TryAsynSendPackets()
{
	DataBufferArg arg;
	arg.cbBuffer = CBBUFF;

	if( !FillPackets2Block(arg) )
		return false;

	m_pSendBuffer = (PBYTE)arg.pBuffer;
	m_cbData2Send = arg.cbData;
	m_cbDataSended = 0;

	AsynSend( m_pSendBuffer, m_cbData2Send);

	m_totalSendByte += arg.cbData;

	return true;
}

bool NetChannel::OnParsePacketsFromStream()
{
	while(FetchBlockFromStreamAndProcess());

	return false;
}

void NetChannel::OnExitSending()
{
	AUTOLOCK( m_MutexSending );

	m_bHasStartSending = false;
	m_hExitSending.SetEvent();
}

void NetChannel::OnExitReceiving()
{
	m_hExitReceiving.SetEvent();
	DisConnect();
}

void CALLBACK NetChannel::AsynIoCompleteProc( DWORD dwErrCode, DWORD dwTransed, LPOVERLAPPED param)
{
	MYOVERLAPPED* pMYOLP = (MYOVERLAPPED*)param;
	assert( pMYOLP != NULL );

	NetChannel* pChannel = pMYOLP->pChannel;

	if(!pChannel)
	{
		if( pMYOLP->bSend )
		{
			if( dwErrCode != 0 || !pChannel->OnAsynSendComplete(dwTransed) )
			{
				pChannel->OnExitSending();

				// 有小概率发生发送队列还有数据包的情况，再检查一次
				pChannel->TryStartSending();
			}
		}
		else
		{
			if( dwErrCode != 0 || !pChannel->OnAsynRecvComplete(dwTransed))
			{
				pChannel->OnExitReceiving();
			}
		}
	}
}
