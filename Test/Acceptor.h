#pragma once
#include "NetEventHandler.h"
#include "MySocket.h"
#include "MySockAddr.h"

class MyNetServiceImpl;

class Acceptor : public NetEventHandler
{
public:
	Acceptor(Socket& socket, const SockAddr& addr, MyNetServiceImpl* pNetService);
	virtual ~Acceptor(void);

	virtual SOCKET GetSocket() const;

	virtual void HandleInput();
	virtual void HandleOutput();
	virtual void HandleException();
	virtual void HandleClose();

protected:
	Socket				m_socket;
	SockAddr			m_addr;
	MyNetServiceImpl*	m_pNetService;
};

