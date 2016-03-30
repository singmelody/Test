#include "StdAfx.h"
#include "SocketAccept.h"
#include "MyLog.h"
#include "NetChannelBase.h"
#include "NetManager.h"

void SocketAccept::Accept(char* sAddr, int32 nPort, bool bSrv /*= false*/)
{
	SockAddr addr(sAddr, nPort);
	if(!m_socket.Create())
		return;

	if(!m_socket.Bind(addr))
	{
		MyLog::error("Bind To(%s:%d)Failed", sAddr, nPort);
		m_socket.Close();
		return;
	}

	if(!m_socket.Listen())
	{
		m_socket.Close();
		return;
	}

	Thread::Start();
}

void SocketAccept::Run()
{
	while (!m_bShouldExit)
	{
		SOCKET socket = m_socket.Accept();
		if(socket < 0)
		{
			int32 nError = m_socket.Accept();
			if(nError != MY_EAGAIN && nError != MY_EWOULDBLOCK)
				MyLog::error("accept error:%d !", nError);

			continue;
		}

		Socket sock(socket);

		NetChannelBase* pChannel = m_pMgr->CreateNewChannel();
		if(!pChannel)
			continue;

		pChannel->InitChannel( m_pMgr, m_nSockRcBuffSize, m_nRcBuffSize, m_nSockSnBuffSize, m_nSnBuffSize, &sock);
		m_pMgr->AddNewConnection( pChannel, true, false);
		m_pMgr->FreeChannel(pChannel);
	}
}
