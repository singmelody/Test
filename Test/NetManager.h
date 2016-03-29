#pragma once
#include "Atomic.h"
#include "Time.h"
#include "FunctionBase.h"
#include <string>
#include <deque>
#include "CircularListEx.h"

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

class NetManager
{
public:
	NetManager(void);
	~NetManager(void);

	void NetRun();

	void Accept( char* sAddr, int32 nPort, bool bSrv);
	int32 Connect( char* sAddr, int32 nPort, bool bSingleThread = false);

	AtomicInt64& BytesSend() { return m_nBytesSend; }
	AtomicInt64& BytesRecv() { return m_nBytesRecv; }
	AtomicInt64& PacketsSend() { return m_nPacketsSend; }
	AtomicInt64& PacketsRecv() { return m_nPacketsRecv; }
protected:
	NetChannelBase* CreateNewChannel();

	void ProcNewConnection();
	void ProcPendingClose();
	void ProcPendingDestroy();

	bool				m_bLzoCompress;
	Time				m_timer;
	Mutex				m_MutexChannels;
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

	std::deque<NetChannelBase*>	m_newSocketQueue;

	AtomicInt64			m_nBytesSend;
	AtomicInt64			m_nBytesRecv;
	AtomicInt64			m_nPacketsSend;
	AtomicInt64			m_nPacketsRecv;
	std::string			m_strName;
	uint64				m_lstStatisticTime;
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

