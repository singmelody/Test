#pragma once
#include "Time.h"
#include <WinSock2.h>

class NetReactor;

class NetEventHandler
{
public:
	NetEventHandler(void) : m_pReactor(NULL), m_NetEvent(0), m_Timeout(0){}
	virtual ~NetEventHandler(void);

	virtual SOCKET GetSocket() const = 0;
	virtual void HandleInput() = 0;
	virtual void HandleOutput() = 0;
	virtual void HandleException() = 0;

	virtual void HandleTick(const Time& now) = 0;
	virtual void HandleClose() = 0;

	void SetReactor(NetReactor* pReacter) { m_pReactor = pReacter; }
	NetReactor* GetReactor() { return m_pReactor; }

	void SetEventMask(uint32 mask)
	{
		m_NetEvent = mask;
	}

	uint32 GetEventMask() const
	{
		return m_NetEvent;
	}


	uint64 GetTimeout() const { return m_Timeout; }
	void SetTimeout(uint64 nTime) { m_Timeout = nTime; }
protected:
	NetReactor* m_pReactor;
	int32		m_NetEvent;
	uint64		m_Timeout;
};

