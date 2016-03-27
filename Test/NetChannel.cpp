#include "StdAfx.h"
#include "NetChannel.h"
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
}

NetChannel::~NetChannel(void)
{
	SAFE_DELETE_ARRAY( m_pRecvBuffer );
}

bool NetChannel::InitChannel(NetManager* pMgr, int32 nSockRcBuffSize, int32 nStreamRcSize, int32 nSockSnBuffSize, int32 nStreamSnSize,Socket* pSocket = 0)
{
	if(!NetChannelBase::InitChannel( pMgr, nSockRcBuffSize, nStreamRcSize, nSockSnBuffSize, nStreamSnSize, pSocket))
		return false;

	m_hExitSending.CreateEvent( true, true);
	m_hExitReceiving.CreateEvent( true, false);
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

	// ��������
}

bool NetChannel::OnAsynRecvComplete(DWORD dwTransed)
{
	if(dwTransed == 0)
		return false;

	m_cbDataRecved += dwTransed;
}



void NetChannel::OnPacketParsed(PacketBase* pPkt)
{

}

void NetChannel::TryStartSending()
{

}

bool NetChannel::TryAysnSendPackets()
{

}

void NetChannel::OnExitSending()
{
	AUTOLOCK( m_MutexSending );


}

void NetChannel::OnExitReceiving()
{

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
