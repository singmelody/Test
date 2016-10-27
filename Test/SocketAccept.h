#pragma once

#include "Thread.h"
#include "MySocket.h"

class NetManager;

class SocketAccept : public MyThread
{
public:
	SocketAccept(void) : m_pMgr(NULL), m_bShouldExit(false)
	{

	}

	~SocketAccept(void) { m_socket.Close(); }

	void Init(NetManager* pMgr, int32 nSockRcBuffSize, int32 nRcBufferSize, int32 nSockSnBufSize, int32 nSnBufferSize)
	{
		m_pMgr = pMgr;
		m_nSockRcBuffSize = nSockRcBuffSize;
		m_nSockSnBuffSize = nSockSnBufSize;
		m_nRcBuffSize = nRcBufferSize;
		m_nSnBuffSize = nSnBufferSize;
	}

	SOCKET GetSocket() const
	{
		return m_socket.GetSocket();
	}

	void HandleClose()
	{
		m_socket.Close();
	}

	void Accept(char* sAddr, int32 nPort, bool bSrv = false);
	void Exit() { m_bShouldExit = false; }
protected:
	void Run();

	Socket			m_socket;
	SockAddr		m_addr;

	int32			m_nSockRcBuffSize;
	int32			m_nSockSnBuffSize;

	int32			m_nRcBuffSize;
	int32			m_nSnBuffSize;

	NetManager*		m_pMgr;
	volatile bool	m_bShouldExit;
};

