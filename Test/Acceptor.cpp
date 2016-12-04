#include "StdAfx.h"
#include "Acceptor.h"
#include "MySocket.h"
#include "MyLog.h"
#include "MyNetWork.h"
#include "MyNetServiceImpl.h"

Acceptor::Acceptor(Socket& socket, const SockAddr& addr, MyNetServiceImpl* pNetService)
{
	m_socket.Shift(socket);
	m_pNetService = pNetService;
}

Acceptor::~Acceptor(void)
{
	m_socket.Close();
}

SOCKET Acceptor::GetSocket() const
{
	return m_socket.GetSocket();
}

void Acceptor::HandleInput()
{
	while (true)
	{
		SOCKET socket = m_socket.Accept();
		if( socket == INVALID_SOCKET)
		{
			int32 nError = Socket::GetSysError();
			if( nError != MY_EAGAIN && nError != MY_EWOULDBLOCK )
			{
				MyLog::message("accpet error:%d", nError);
			}
			break;
		}

		Socket sock(socket);
		m_pNetService->OnNewConnection( sock, CT_PASSIVE);
	}
	return;
}

void Acceptor::HandleOutput()
{
	return;
}

void Acceptor::HandleException()
{
	return;
}

void Acceptor::HandleClose()
{
	return;
}
