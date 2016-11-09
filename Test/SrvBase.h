#pragma once
#include "Factory.h"
#include "PoolAllocator.h"

#include "IRecvPacketFilter.h"
#include "MySockAddr.h"
#include "Thread.h"
#include "NetManager.h"
#include "MyListNode.h"
#include <map>

class NetManager;
class MyPacketProc;
class CltNewConnection;

class CltNewConnection
{
	DECLARE_FACTORY_ARG0(CltNewConnection, -1, new PoolAllocator);

	friend class MyList<CltNewConnection>;
public:
	CltNewConnection()
	{
		nSocketID = -1;
		nTimeOffVal = 10000;
		_next = _prev = NULL;
	}

	~CltNewConnection(){}

	inline CltNewConnection* prev() { return _prev; }
	inline void prev(CltNewConnection* _p) { _prev = _p; }

	inline CltNewConnection* next() { return _next; }
	inline void next(CltNewConnection* _p) { _next = _p; }

	int32 nSocketID;
	int32 nTimeOffVal;
private:

	CltNewConnection* _prev;
	CltNewConnection* _next;
	MyList<CltNewConnection>* pList;
};

class SrvBaseRecvPacketFilter;

class SrvBase
{
public:
	SrvBase(void);
	virtual ~SrvBase(void);

	virtual bool SrvInit();
	virtual bool SrvExit();

	void FillSrvConfig();
	bool GetAddr( SockAddr& addr, int32 nSocketID);

	void CloseCltConnection(int32 nSocketID);
	void SrvSend(int32 nSocketID, PacketBase* pkt);

	virtual NetManager* CreateCltNetManager( bool bLZOCompress, int32 nSockRcBufSize, int32 nRcBufferSize, int32 nSockSnBuffSize, int32 SnBufferSize,
		FunctionBase_Arg1<int32>* funcAccpet = NULL, FunctionBase_Arg1<int32>* funcCon = NULL, FunctionBase_Arg1<int32>* funcDiscon = NULL, int32 MAX_SOCKETS = MY_SOCKET_LIST_SIZE);

	virtual MyPacketProc* CreateCltPktProcessor();

	virtual bool Thread_CheckCltPktTypeValid(class PacketBase& pkt) { return true; }

protected:
	void SrvAccept(int32 nSocketID);
	void SrvConnect(int32 nSocketID){}
	void SrvDisconnect(int32 nSocketID);

	virtual void OnCltDisconnect(int32 nSocketID){}

	void CltNetOperation(int32 nSocketID, int32 nNetEvent);

	void SrvThreadLoop();

	virtual void RegCltPktHandle(PacketProcessor* pPkt);
	virtual void DftCltPktHandle(PacketBase* pPkt) {}
	void PktCltNetEvent( class PacketNetEvent* pPkt);


	std::map<int32, CltNewConnection*>	m_CltNewConnections;

	MyThread		m_SrvNetThread;
	NetManager*		m_SrvNetMgr;
	MyPacketProc*	m_CltPktProc;
	Mutex			m_listMutex;

	bool			m_SrvUseIOCP;
	uint64			m_SrvNetThreadMinFrameTime;

	std::string		m_SrvIP;
	int32			m_SrvPort;

	int32			m_SrvSocketRcBufSize;
	int32			m_SrvCirRcBufSize;
	int32			m_SrvSocketSnBufSize;
	int32			m_SrvCirSnBufSize;

	int32			m_bUseSrvNetDelay;
	int32			m_SrvNetDelayMin;
	int32			m_SrvNetDelayMax;
	
	bool			m_bSrvEnableLZO;
	bool			m_bSrvPacketUseIndexWhenSend;
	bool			m_bSrvPacketUseIndexWhenRecv;

	int32			m_nSrvNetEventWaitTime;

	int32			m_SrvNetThreadFrameTime;
	int32			m_SrvMaxSockets;	// 最多支持的socket连接数
	static const int32 SERVER_MIN_SOCKETS = 8;

	SrvBaseRecvPacketFilter*	m_pRecvPacketFilterOfSrvBase;
};