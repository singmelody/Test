#pragma once
#include "Atomic.h"
#include "Time.h"

class NetChannelBase;

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
protected:
	NetChannelBase* CreateNewChannel();

	void ProcNewConnection();
	void ProcPendingClose();
	void ProcPendingDestroy();

	AtomicInt64	m_nBytesSend;
	AtomicInt64 m_nBytesRecv;
	AtomicInt64	m_nPacketsSend;
	AtomicInt64	m_nPacketsRecv;
	std::string	m_strName;
	uint64		m_lstStatisticTime;
};

