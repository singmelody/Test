#pragma once

#include "MyNetWork.h"
#include "MyThreadGroup.h"
#include <hash_map>

enum NetServiceState
{
	eNSS_Init		= 0,
	eNSS_Running	= 1,
	eNSS_Shutdown	= 2,
};

class MyNetThread;

class MyNetServiceImpl : public INetService, public MyThreadGroup
{
	typedef stdext::hash_map<uint32, INetConnection*> CONNECTION_MAP;
public:
	MyNetServiceImpl(uint32 nID, const NetServiceParam& param, INetEventListener* pListener);
	~MyNetServiceImpl(void);

	virtual uint32 GetID() const { return m_nID; }
	virtual bool BindAddress(const SockAddr& addr);
	virtual NetConnectionPtr Connect(const SockAddr& addr);
	virtual NetConnectionPtr Connect(uint32 nID, const SockAddr& addr);
	virtual void VisitConnections(ConnectionVisitor& visitor);

	virtual bool Startup();
	virtual void Shutdown();

	virtual void RecycleConnection(INetConnection* pPtr);
	virtual INetConnection* GetListener() const;

	virtual bool AddVerifiedConnection(uint32 nID, INetConnection* pConn);

	virtual NetConnectionPtr GetConnection(uint32 nID);
	virtual const NetServiceParam& GetParam() const;

	NetServiceState GetState() const { return m_nState; }
	void OnNewConnection(Socket& socket, ConnectionType type, uint32 nID = 0);
protected:
	bool	InitSocket(Socket& socket);
	void	ScheduleConnection(INetConnection* pConn);

	uint32						m_nID;
	volatile NetServiceState	m_nState;
	NetServiceParam				m_Param;
	INetEventListener*			m_pListener;
	MyNetThread*				m_pConnectThread;
	uint32						m_nNumOfConnection;
	CONNECTION_MAP				m_Connections;

	Mutex						m_mutex;
};

