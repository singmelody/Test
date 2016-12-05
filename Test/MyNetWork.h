#pragma once

#include "NetEventHandler.h"
#include "MySmartPtr.h"
#include "MySockAddr.h"

enum ConnectionStatus
{
	CS_CONNECTED = 0,
	CS_DISCONNECTED = 1
};

enum ConnectionType
{
	CT_ACTIVE = 0,
	CT_PASSIVE = 1,
};

class INetService;
class Protocol;

class INetConnection : public NetEventHandler, public IReferable
{
public:
	virtual ~INetConnection(){}

	virtual ConnectionStatus GetStatus() const = 0;
	virtual int32 GetID() const = 0;
	virtual void SetID(uint32 nID) = 0;
	virtual ConnectionType GetType() const = 0;
	virtual void Disconnect() = 0;
	virtual SockAddr GetPeerAddr() const = 0;
	virtual SockAddr GetSockAddr() const = 0;
	virtual void Open(class Socket& sock, ConnectionType type, INetService* pNetService) = 0;
	virtual void Close() = 0;
	virtual bool SendPacket(class Packet* packet);
	virtual int32 SendData(const char* pData, int32 nLen) = 0;
	virtual int32 RecvData(const char* pData, int32 nLen) = 0;

	virtual bool SendProtocol(Protocol* pProtocol){ return false; }
	virtual bool HandleIntervalProtocol(Protocol* pPacket) { return false; }
};

typedef SmartPtr<INetConnection> NetConnectionPtr;

class INetEventListener
{
public:
	virtual ~INetEventListener(){}

	virtual INetConnection* OnCreateConnection() = 0;
	virtual void OnConnectionVerified(uint32 nID) = 0;
	virtual void OnConeectionClosed(uint32 nID) = 0;
};

class ConnectionVisitor
{
public:
	virtual ~ConnectionVisitor() {}
	virtual void operator()(INetConnection* pConn) = 0;
};

struct NetServiceParam
{
	NetServiceParam();

	void Check();

	uint32 nNumOfConnThread;
	uint32 nMaxNumOfConn;
	uint32 nIntervalOfTick;
	uint32 nSizeOfSysBuffer;
	uint32 nMaxSizeOfSysBuffer;
	uint32 nBackLog;
	uint32 nSizeOfConnectThread;
};

class INetService
{
public:
	virtual ~INetService() {}

	virtual uint32 GetID() const = 0;
	virtual bool BindAddress(const SockAddr& addr) = 0;
	virtual NetConnectionPtr Connect(const SockAddr& addr) = 0;
	virtual NetConnectionPtr Connect(uint32 nID, const SockAddr& addr) = 0;
	virtual void VisitConnections(ConnectionVisitor& visitor) = 0;

	virtual bool Startup() = 0;
	virtual void Shutdown() = 0;

	virtual void RecycleConnection(INetConnection* pPtr) = 0;
	virtual INetConnection* GetListener() const = 0;

	virtual bool AddVerifiedConnection(uint32 nID, INetConnection* pConn) = 0;

	virtual NetConnectionPtr GetConnection(uint32 nID) = 0;
	virtual const NetServiceParam& GetParam() const = 0;

	INetService* CreateNetService(uint32 nID, const NetServiceParam& param, INetEventListener* pListener);

	void DestroyNetService(INetService* pService);
};

INetService* CreateNetService(uint32 nID, const NetServiceParam& param, INetEventListener* pListener);
void DestroyNetService(INetService* pService);