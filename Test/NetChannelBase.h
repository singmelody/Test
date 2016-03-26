#pragma once
#include "NetEventHandler.h"
#include "MySocket.h"
#include <WinSock2.h>

class NetEventHandler;
class PacketBase;

class NetChannelBase : public NetEventHandler
{
	friend class NetManager;
	friend class NetManagerReactor;
public:
	NetChannelBase(void);
	~NetChannelBase(void);

	virtual void AppendPacket( PacketBase* pPkt);
	virtual bool InitChannel( NetManager* pMgr, int32 nSockRcBuffSize, int32 nSocketBuffSize, int32 nStreamSize, Socket* pSocket = 0);

	int32 GetID() const { return m_id;}
	void SetID(int32 nID) { m_id = nID; }

protected:
	virtual bool StartChannel();

	int32 m_RefCount;

	volatile bool m_bIsClosing;

	bool Listen(char* sAddr, int32 nPort);
	bool Connect( char* sAddr, int32 nPort);

	virtual void DisConnect();
	virtual void CloseChannel();

	void SetSocket(SOCKET socket);
	SOCKET GetSocket() const { return m_Socket.GetSocket(); }


	int32	m_id;
	Socket  m_Socket;
};

