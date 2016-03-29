#pragma once

#include "Thread.h"

class NetManager;

class SocketAccept : public Thread
{
public:
	SocketAccept(void) : m_pMgr(NULL), m_bShouldExit(false)
	{

	}

	~SocketAccept(void) { m_Socket.Close(); }

	void Init(NetManager* pMgr, int32 nSockRcBuffSize, int32 nRcBufferSize, int32 nSockSnBufSize, int32 nSnBufferSize)
	{
		m_pMgr = pMgr;

	}


protected:


	NetManager*		m_pMgr;
	bool			m_bShouldExit;
};

