#pragma once
#include "Atomic.h"
#include "Time.h"
#include "FunctionBase.h"
#include <string>
#include <deque>
#include "CircularListEx.h"
#include "SocketAccept.h"
#include "PacketBase.h"
#include "MyMutex.h"

#define MY_SOCKET_LIST_SIZE		2048

#if defined(_WINDOWS)
#define USEIOCP
#endif

#ifdef USEIOCP
#define	BASENETMANAGER NetManagerIOCP
#else
#define BASENETMANAGER NetManagerReactor
#endif

class NetChannelBase;
class NetReactor;
class PacketProcessor;
struct IRecvPacketFilter;

class NetManager
{
public:
	NetManager(
		bool bLZOCompress,
		int32 nSockRcBufSize,
		int32 nRcBufSize,
		int32 nSockSnBufSize,
		int32 nSnBufSize,
		FunctionBase_Arg1<int32>* funcAccept,
		FunctionBase_Arg1<int32>* funcCon,
		FunctionBase_Arg1<int32>* funcDiscon,
		int32 MAX_SOCKETS);
	~NetManager(void);

	void NetRun();

	void Accept( char* sAddr, int32 nPort, bool bSrv);
	int32 Connect( char* sAddr, int32 nPort, bool bSingleThread = false);

	void CloseChannel(int32 nChannel);
	virtual void OnDisconnect(NetChannelBase* s);
	virtual void OnChannelStart(NetChannelBase* pChannel){}

	int32 AddChannel(NetChannelBase* pBase);
	int32 AddNewConnection(NetChannelBase* pBase, bool bSocketIsAccepted, bool bWait);
	void FreeChannel(NetChannelBase* pBase);

	NetChannelBase* GetChannel(int32 nSocketID);

	AtomicInt64& BytesSend() { return m_nBytesSend; }
	AtomicInt64& BytesRecv() { return m_nBytesRecv; }
	AtomicInt64& PacketsSend() { return m_nPacketsSend; }
	AtomicInt64& PacketsRecv() { return m_nPacketsRecv; }

#if PACKET_USE_INDEX_DATA
	inline void UseIndexWhenSend(bool b) { m_bUseIndexWhenSend = b; }
	inline void UseIndexWhenRecv(bool b) { m_bUseIndexWhenRecv = b; }

	inline bool UseIndexWhenSend() const { return m_bUseIndexWhenSend; }
	inline bool UseIndexWhenRecv() const { return m_bUseIndexWhenRecv; }
#endif

	void SetDisconnectFun(FunctionBase_Arg1<int32>* funcDisconn) { m_DisconnectCallBack = funcDisconn; }
	
	bool SendPacket( PacketBase& pkt, int32 nSocketID);

	virtual void OnReceivedPacket( NetChannelBase* pChannel, PacketBase* pPkt);
	bool Compressable() const { return m_bLzoCompress; }
	NetChannelBase* CreateNewChannel();

	static PacketBase* DuplicatePacket( PacketBase& pkt);
protected:

	void ProcNewConnection();
	void ProcPendingClose();
	void ProcPendingDestroy();

	Mutex				m_mutexChannels;
	Mutex				m_destroyQueueLock;
	Mutex				m_closingQueueLock;

	std::deque<NetChannelBase*>	m_newSocketQueue;
	std::deque<NetChannelBase*> m_destroyQueue;
	std::deque<int32>			m_closingQueue;

	bool				m_bLzoCompress;
	Time				m_timer;

	CircularListEx<NetChannelBase*> m_ChannelList;


	SocketAccept		m_Accept;
	PacketProcessor*	m_pProcessor;
	int32				m_nSockRcBufSize;
	int32				m_nRcBufSize;
	int32				m_nSockRnBufSize;
	int32				m_nSnBufSize;

	FunctionBase_Arg1<int32>*	m_AcceptCallBack;
	FunctionBase_Arg1<int32>*	m_ConnectCallBack;
	FunctionBase_Arg1<int32>*	m_DisconnectCallBack;



	AtomicInt64			m_nBytesSend;
	AtomicInt64			m_nBytesRecv;
	AtomicInt64			m_nPacketsSend;
	AtomicInt64			m_nPacketsRecv;
	std::string			m_strName;
	uint64				m_lstStatisticTime;

	bool				m_bUseIndexWhenSend;
	bool				m_bUseIndexWhenRecv;


	IRecvPacketFilter*	m_pIRecvPacketFilter;


};


// windows
class NetManagerIOCP : public NetManager
{
public:
	NetManagerIOCP(
		bool bLZOCompress,
		int32 nSockRcBuffSize,
		int32 nRcBufferSize,
		int32 nSnBufferSize,
		FunctionBase_Arg1<int32>* funcAccept = NULL,
		FunctionBase_Arg1<int32>* funcCon = NULL,
		FunctionBase_Arg1<int32>* funcDiscon = NULL,
		int32 MAX_SOCKETS = MY_SOCKET_LIST_SIZE
		);

	virtual ~NetManagerIOCP()
	{

	}
};

// Linux
class NetManagerReactor : public NetManager
{
public:
	NetManagerReactor(
		bool bLZOCompress,
		int32 nSockRcBuffSize,
		int32 nRcBufferSize,
		int32 nSnBufferSize,
		FunctionBase_Arg1<int32>* funcAccept = NULL,
		FunctionBase_Arg1<int32>* funcCon = NULL,
		FunctionBase_Arg1<int32>* funcDiscon = NULL,
		int32 MAX_SOCKETS = MY_SOCKET_LIST_SIZE
		);

	virtual ~NetManagerReactor()
	{

	}

	virtual void NetRun();
	virtual void OnDisconnect(NetChannelBase* s);
	virtual void OnChannelStart(NetChannelBase* pChannel);

protected:
	NetReactor*	m_pReactor;
};

