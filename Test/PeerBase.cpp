#include "StdAfx.h"
#include "PeerBase.h"
#include <assert.h>
#include "WatchDog.h"
#include "NetManager.h"
#include "BaseType.h"
#include "MyLog.h"
#include "MyPacketProc.h"
#include "PacketProcessor.h"
#include "GameUtil.h"

PeerBase::PeerBase(void)
	: m_bPeerEnableLZO(false)
	, m_nPeerNetEventWaitTime(0)
{
	m_PeerPktProc			= NULL;
	m_pPeerNetManager		= NULL;

	m_PeerSocketRcBufSize	= 4096;
	m_PeerCirRcBufSize		= 4096*1024;
	m_PeerSocketRnBufSize	= 4096;
	m_PeerCirRnBufSize		= 4096*1024;

	m_PeerIP				= "127.0.0.1";
	m_PeerPort				= 10001;

	m_bPeerUseIOCP			= false;

#ifdef USEIOCP
	m_bPeerUseIOCP			= true;
#endif

	m_PeerMaxSockets		= PEER_MIN_SOCKETS;
	m_bStartPeerListen		= true;
}


PeerBase::~PeerBase(void)
{
}

bool PeerBase::PeerInit()
{
	MyLog::message("Begin PeerBase::PeerInit()");

	FillPeerConfig();


	m_PeerPktProc = CreatePeerPktProc();
	if( !m_PeerPktProc )
		return false;

	FunctionBase_Arg1<int32>* pAFunc = new Function_Arg1< PeerBase, int32>( this, &PeerBase::PeerOnAccept);
	FunctionBase_Arg1<int32>* pCFunc = new Function_Arg1< PeerBase, int32>( this, &PeerBase::PeerOnConnect);
	FunctionBase_Arg1<int32>* pDFunc = new Function_Arg1< PeerBase, int32>( this, &PeerBase::PeerOnDisconnect);

	NetManager* pMgr = CreatePeerNetManager( m_bPeerEnableLZO, m_PeerSocketRcBufSize, m_PeerCirRcBufSize, m_PeerSocketRnBufSize, m_PeerCirRnBufSize,
		pAFunc, pCFunc, pDFunc, m_PeerMaxSockets);

	if(!pMgr)
	{
		MyLog::error("Peer Net Manager Create Fail");
		return false;
	}

	pMgr->SetName("PeerBase");

	if( m_bStartPeerListen )
	{
		pMgr->Accept( (char*)m_PeerIP.c_str(), m_PeerPort, false);
		MyLog::message("Peer Listen IP = %s port = %d", m_PeerIP.c_str(), m_PeerPort);
	}

#if PACKET_USE_INDEX_DATA
	pMgr->UseIndexWhenRecv(false);
	pMgr->UseIndexWhenSend(false);
#endif

	pMgr->SetPacketProcessor( m_PeerPktProc );
	pMgr->SetEventWaitTime(m_nPeerNetEventWaitTime);

	m_pPeerNetManager = pMgr;

	RegPeerPktHandle( m_PeerPktProc );

	MyThread* pThread = new MyThread("Peer");
	pThread->Init(new Function_Arg0<PeerBase>( this, &PeerBase::Run));
	m_PeerThreadList.add(pThread);
	pThread->Start();

	// init watch dog
	WatchDog::Instance().RegWatchDog( SRV_THREAD_WATCHDOG_ID, "Srv-Thread");

	MyLog::message("End SrvBase::SrvExit");

	return true;
}

void PeerBase::FillPeerConfig()
{

}

bool PeerBase::PeerRun()
{
	if(!m_pPeerNetManager)
		return false;

	WatchDog::Instance().NextStep(PEER_THREAD_WATCHDOG_ID);
	m_pPeerNetManager->NetRun();
	WatchDog::Instance().NextStep(PEER_THREAD_WATCHDOG_ID);

	return true;
}

bool PeerBase::PeerExit()
{
	for (int32 i = 0; i < THREADMAXCNT; ++i)
	{
		MyThread* pThread = m_PeerThreadList.get(i);
		if(!pThread)
			continue;

		pThread->Exit();
	}

	for (int32 i = 0; i < THREADMAXCNT; ++i)
	{
		MyThread* pThread = m_PeerThreadList.get(i);
		if(!pThread)
			continue;

		pThread->Wait();
		SAFE_DELETE(pThread);
	}

	m_PeerThreadList.Clear();

	SAFE_DELETE(m_pPeerNetManager);
	SAFE_DELETE(m_PeerPktProc);

	return true;
}

int PeerBase::PeerProcPacket()
{
	return m_PeerPktProc->DoProc();
}

void PeerBase::PeerSend(PacketBase* pPkt, int32 nSocketID)
{
	assert( pPkt && m_pPeerNetManager);
	m_pPeerNetManager->SendPacket( *pPkt, nSocketID);
}

void PeerBase::W2LPacketCounter(int32 nPacketType)
{
	if(!W2L_Packet_Counter)
		return;

	PeerSendPacketCounter::iterator itr = m_W2LPacketCounter.find(nPacketType);
	if ( itr == m_W2LPacketCounter.end() )
		m_W2LPacketCounter.insert(std::make_pair< int32, int32>( nPacketType, 1));
	else
		itr->second++;
}

MyPacketProc* PeerBase::CreatePeerPktProc()
{
	MyLog::message("Create Default Peer packet Processor");
	return new MyPacketProc();
}

NetManager* PeerBase::CreatePeerNetManager(
	bool bLZOCompress, int32 nSockRcBufSize, int32 nRcBufferSize, int32 nSockSnBuffSize, 
	int32 SnBufferSize, FunctionBase_Arg1<int32>* funcAccpet /*= NULL*/, FunctionBase_Arg1<int32>* funcCon /*= NULL*/, FunctionBase_Arg1<int32>* funcDiscon /*= NULL*/, int32 MAX_SOCKETS /*= MY_SOCKET_LIST_SIZE*/)
{
	return new BASENETMANAGER( bLZOCompress, nSockRcBufSize, nRcBufferSize, nSockSnBuffSize, 
		SnBufferSize, funcAccpet, funcCon, funcDiscon, MAX_SOCKETS);	
}

void PeerBase::Run()
{
	WatchDog::Instance().NextStep(PEER_THREAD_WATCHDOG_ID);

	uint64 tTimeStart = Time::CurrentTime().MilliSecond();

	PeerRun();

	uint64 tTime = Time::CurrentTime().MilliSecond();

	// less than 2 ms , 
	if( (tTime - tTimeStart) < 2)
	{
		GameUtil::Sleep(1);
	}
}

int32 PeerBase::PeerConnect(char* sIP, int32 nPort, bool bDirty /*= false*/)
{
	if(!m_pPeerNetManager)
		return -1;

	while (true)
	{
		int32 nSocketID = m_pPeerNetManager->Connect( sIP, nPort);
		if(nSocketID >= 0)
			return nSocketID;

		if(!bDirty)
			return -1;

		Console::Yellow();
		MyLog::message("Peer Connect %s:%d Fail, Try Later!", sIP, nPort);
		Console::Reset();

		GameUtil::Sleep(2000);
	}

	return -1;
}

void PeerBase::RegPeerPktHandle(PacketProcessor* pProc)
{
	REG_DEFAULT_PACKET_HANDLER( pProc, PacketBase, PeerBase, DftPeerPktHandle);
}
