#pragma once

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

	void SetEventMask();
protected:
	NetReactor* m_pReactor;
	int32		m_NetEvent;
	uint64		m_Timeout;
};

