#pragma once

#include "PacketSender.h"
#include <map>
#include "FunctionBase.h"
#include "CircularList.h"

class NetManager;
class MyPacketProc;
class MyThread;

class PeerBase : public PacketSender
{
	typedef std::map< int32, int32> PacketSendCounter;
	typedef std::map< int32, int32> PeerSendPacketCounter;

public:
	PeerBase(void);
	~PeerBase(void);

	bool PeerInit();
	void FillPeerConfig();

	virtual bool PeerRun();
	virtual bool PeerExit();
	virtual int PeerProcPacket();

	void PeerSend( PacketBase* pPkt, int32 nSocketID);

	void W2LPacketCounter( int32 nPacketType);

	virtual MyPacketProc* CreatePeerPktProc();
	
	virtual NetManager* CreatePeerNetManager( bool bLZOCompress, int32 nSockRcBufSize, int32 nRcBufferSize, int32 nSockSnBuffSize, int32 SnBufferSize,
		FunctionBase_Arg1<int32>* funcAccpet = NULL, FunctionBase_Arg1<int32>* funcCon = NULL, FunctionBase_Arg1<int32>* funcDiscon = NULL, int32 MAX_SOCKETS = MY_SOCKET_LIST_SIZE);

protected:
	void Run();

	int32 PeerConnect(char* sIP, int32 nPort, bool bDirty = false);

	void PeerOnAccept(int32 nSocketID);
	void PeerOnConnect(int32 nSocketID);
	void PeerOnDisconnect(int32 nSocketID);

	virtual void RegPeerPktHandle(class PacketProcessor* pProc);
	virtual void DftPeerPktHandle(class PacketBase* pkt) {}

	MyPacketProc*		m_PeerPktProc;
	NetManager*			m_pPeerNetManager;

	int32				m_PeerSocketRcBufSize;
	int32				m_PeerCirRcBufSize;
	int32				m_PeerSocketRnBufSize;
	int32				m_PeerCirRnBufSize;

	int32				m_PeerMaxSockets;
	static const int32 PEER_MIN_SOCKETS = 32;

	std::string			m_PeerIP;
	int32				m_PeerPort;
	bool				m_bPeerUseIOCP;

	PacketSendCounter	m_PeerSendPacketCounter;

	PacketSendCounter	m_W2GPacketCounter;
	PacketSendCounter	m_G2WPacketCounter;

	PacketSendCounter	m_N2GPacketCounter;
	PacketSendCounter	m_G2NPacketCounter;

	PacketSendCounter	m_N2WPacketCounter;
	PacketSendCounter	m_W2NPacketCounter;

	PacketSendCounter	m_L2WPacketCounter;
	PacketSendCounter	m_W2LPacketCounter;

	bool				m_bPeerEnableLZO;
	int32				m_nPeerNetEventWaitTime;
	bool				m_bStartPeerListen;

	#define THREADMAXCNT 4
	CircularList<MyThread*, THREADMAXCNT>	m_PeerThreadList;
};

