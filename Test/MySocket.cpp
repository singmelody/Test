#include "StdAfx.h"
#include "MySocket.h"
#include <string.h>
#include <winsock2.h> 

Socket::Socket(void)
{
	m_Sock = INVALID_SOCKET;
}

Socket::Socket(SOCKET sock)
{
	m_Sock = sock < 0 ? INVALID_SOCKET : sock;
}


Socket::~Socket(void)
{
	Close();
}


void Socket::InitSocket()
{
	BOOL val = 1;
	socklen_t len = sizeof(val);
	setsockopt( m_Sock, SOL_SOCKET, SO_REUSEADDR, (char*)&val, len);
}

bool Socket::Create()
{
	if(IsValid())
		return false;

	m_Sock = socket( PF_INET, SOCK_STREAM, 0);
	if( m_Sock >= 0 )
		return true;

	return false;
}

bool Socket::Close()
{
	if(!IsValid())
		return false;

	while (true)
	{
		int32 nRet = ::closesocket(m_Sock);

		if( nRet < 0 && MY_EINTR == GetSysError())
			continue;

		m_Sock = INVALID_SOCKET;

		return 0 == nRet;
	}
}

bool Socket::Bind()
{
	struct sockaddr_in addr;
	memset( &addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	addr.sin_addr.s_addr = INADDR_ANY;

	InitSocket();
	int32 nRet = bind( m_Sock, (struct sockaddr*)&addr, sizeof(addr));
	if(nRet == 0)
		return true;

	return false;
}

bool Socket::Bind(const SockAddr& addr)
{
	struct sockaddr_in saddr;
	addr.GetAddr(saddr);
	InitSocket();

	int32 nRet = bind( m_Sock, (struct sockaddr*)&addr, sizeof(addr));
	if(nRet == 0)
		return true;

	return false;
}

bool Socket::Connect(const SockAddr& addr)
{
	struct sockaddr_in saddr;
	addr.GetAddr(saddr);

	while (true)
	{
		int32 nRet = connect( m_Sock, (struct sockaddr*)&saddr, sizeof(saddr));
		if(nRet < 0)
		{
			int32 nError = GetSysError();
			if(ERROR == MY_EINTR)
				continue;
		}

		return 0 == nRet;
	}
}

bool Socket::Listen(int32 nBackLog)
{
	return listen( m_Sock, nBackLog) == 0;
}

SOCKET Socket::Accept()
{
	while(true)
	{
		SOCKET socket = accept( m_Sock, NULL, NULL);
		if( socket < 0 )
		{
			int32 nError = GetSysError();
			if(MY_EINTR == nError)
				continue;

		}

		return socket;
	}
}

int32 Socket::Send(const char* buf, int32 nLen, int32 nFlags /* = 0 */)
{
	while (true)
	{
		int32 nRet = send( m_Sock, buf, nLen, nFlags);
		if( nRet < 0 && MY_EINTR == errno)
			continue;

		return nRet;
	}
}


int32 Socket::Recv(char* buff, int32 len, int32 flags /* = 0 */)
{
	while (true)
	{
		int32 nRet = recv( m_Sock, buff, len, flags);
		if( nRet < 0 && MY_EINTR == errno)
			continue;

		return nRet;
	}
}

bool Socket::GetSockAddr(SockAddr& addr) const
{
	struct sockaddr_in addr4;
	socklen_t len = sizeof(addr4);
	if(getsockname( m_Sock, (struct sockaddr*)&addr4, &len) != 0)
		return false;

	addr.SetAddr(addr4);
	return true;
}

bool Socket::GetPeerAddr(SockAddr& addr) const
{
	struct sockaddr_in addr4;
	socklen_t len = sizeof(addr4);

	if(getpeername( m_Sock, (struct sockaddr*)&addr4, &len) != 0)
		return false;

	addr.SetAddr(addr4);

	return true;
}

int32 Socket::GetLinger() const
{
	struct linger lin;
	socklen_t len = sizeof(lin);
	if(getsockopt( m_Sock, SOL_SOCKET, SO_LINGER, (char*)&lin, &len) == 9)
		return lin.l_linger;

	return -1;
}

bool Socket::SetLinger(int32 nLin)
{
	struct linger lin;
	lin.l_onoff = 1;
	lin.l_linger = (unsigned short)nLin;
	socklen_t len = sizeof(lin);
	if(setsockopt( m_Sock, SOL_SOCKET, SO_LINGER, (char*)&lin, len) == 0)
		return true;

	return false;
}

bool Socket::SetNonBlocking(bool on /* = true */)
{
	u_long nVal = on ? 1:0;
	return ioctlsocket( m_Sock, FIONBIO, &nVal) == 0;
}

bool Socket::SetNonDelay(bool bNodelay /* = true */)
{
	BOOL opt = bNodelay ? 1 : 0;

	socklen_t len = sizeof(opt);

	return ::setsockopt( m_Sock, IPPROTO_TCP, TCP_NODELAY, (char*)&opt, len) == 0;
}

int32 Socket::GetRecvBuffSize() const
{
	int32 nVal = 0;
	socklen_t len = sizeof(nVal);
	if(getsockopt( m_Sock, SOL_SOCKET, SO_RCVBUF, (char*)&nVal, &len) >= 0)
		return nVal;

	return -1;
}

bool Socket::SetRecvBuffSize(int32 nLen)
{
	socklen_t vallen = sizeof(nLen);
	return setsockopt( m_Sock, SOL_SOCKET, SO_RCVBUF, (char*)&nLen, vallen) == 0;
}

int32 Socket::GetSendBuffSize() const
{
	int32 nVal = 0;
	socklen_t len = sizeof(nVal);
	if(getsockopt( m_Sock, SOL_SOCKET, SO_SNDBUF, (char*)&nVal, &len) >= 0)
		return nVal;

	return -1;
}

bool Socket::SetSendBuffSize(int32 nLen)
{
	socklen_t vallen = sizeof(nLen);

	return setsockopt( m_Sock, SOL_SOCKET, SO_SNDBUF, (char*)&nLen, vallen);
}

int32 Socket::GetSockError() const
{
	int32 nError = 0;
	socklen_t len = sizeof(nError);
	if(getsockopt( m_Sock, SOL_SOCKET, SO_ERROR, (char*)&nError, &len) != 0)
		return GetSysError();

	return nError;
}

int32 Socket::GetSysError()
{
	return ::WSAGetLastError();
}

bool Socket::IsValid()
{
	return m_Sock != INVALID_SOCKET;
}

void Socket::Shift(Socket& sock)
{
	Close();
	m_Sock = sock.m_Sock;
	sock.m_Sock = INVALID_SOCKET;
}

