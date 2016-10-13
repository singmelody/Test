#include "StdAfx.h"
#include "SrvBase.h"
#include "MyLog.h"
#include "MyPacketProc.h"
#include "FunctionBase.h"
#include "MyMutex.h"
#include "PacketImpl.h"
#include "WatchDog.h"
#include "ConfigManager.h"

FINISH_FACTORY_ARG0(CltNewConnection);

class SrvBaseRecvPacketFilter : public IRecvPacketFilter
{
public:
	explicit SrvBaseRecvPacketFilter(SrvBase& srv) : m_server(srv) {}

private:
	virtual bool Thread_CheckPacketTypeValid(PacketBase& pkt)
	{
		return m_server.Thread_CheckCltPktTypeValid(pkt);
	}

	SrvBase& m_server;
};



SrvBase::SrvBase(void)
	: m_SrvNetThread("SrvNetWork")
	, m_SrvNetThreadMinFrameTime(-1)
	, m_bSrvEnableLZO(false)
	, m_bSrvPacketUseIndexWhenRecv(false)
	, m_bSrvPacketUseIndexWhenSend(false)
{
	m_CltPktProc = NULL;
	m_SrvNetMgr = NULL;

	m_SrvUseIOCP = false;

#ifdef USEIOCP
	m_SrvUseIOCP = true;
#endif

	m_SrvSocketRcBufSize = 2048;
	m_SrvCirRcBufSize = 2048*32;
	m_SrvSocketRnBufSize = 2048;
	m_SrvCirRnBufSize = 2048*32;

	m_SrvIP = "127.0.0.1";
	m_SrvPort = 10001;

	m_SrvNetThreadFrameTime = 0;
	m_SrvMaxSockets = 0;
	m_nSrvNetEventWaitTime = 0;
	m_pRecvPacketFilterOfSrvBase = NULL;
}


SrvBase::~SrvBase(void)
{
}

bool SrvBase::SrvInit()
{
	MyLog::message("Begin SrvBase::SrvInit");

	FillSrvConfig();

	m_CltPktProc = CreateCltPktProcessor();
	if( !m_CltPktProc )
		return false;

	FunctionBase_Arg1<int32>* pAFunc = new Function_Arg1< SrvBase, int32>( this, &SrvBase::SrvAccept);
	FunctionBase_Arg1<int32>* pCFunc = new Function_Arg1< SrvBase, int32>( this, &SrvBase::SrvConnect);
	FunctionBase_Arg1<int32>* pDFunc = new Function_Arg1< SrvBase, int32>( this, &SrvBase::SrvDisconnect);

	NetManager* pMgr = CreateCltNetManager( m_bSrvEnableLZO, m_SrvSocketRcBufSize, m_SrvCirRcBufSize, m_SrvSocketRnBufSize, m_SrvCirRnBufSize,
		pAFunc, pCFunc, pDFunc, m_SrvMaxSockets);

	if(!pMgr)
	{
		MyLog::error("Srv Net Manager Create Fail");
		return false;
	}

	pMgr->SetName("SrvBase");

#if PACKET_USE_INDEX_DATA
	pMgr->UseIndexWhenRecv(m_bSrvPacketUseIndexWhenRecv);
	pMgr->UseIndexWhenSend(m_bSrvPacketUseIndexWhenSend);
#endif

	pMgr->SetEventWaitTime(m_nSrvNetEventWaitTime);
	pMgr->SetRecvPacketFilter(m_pRecvPacketFilterOfSrvBase);

	pMgr->Accept( (char*)m_SrvIP.c_str(), m_SrvPort, true);

	MyLog::message("Srv Listen IP = %s port=%d", m_SrvIP.c_str(), m_SrvPort);

	pMgr->SetPacketProcessor( m_CltPktProc );

	m_SrvNetMgr = pMgr;

	RegCltPktHandle( m_CltPktProc );

	// init watch dog
	WatchDog::Instance().RegWatchDog( SRV_THREAD_WATCHDOG_ID, "Srv-Thread");

	m_SrvNetThread.Init( new Function_Arg0<SrvBase>( this, &SrvBase::SrvThreadLoop));
	m_SrvNetThread.Start();

	MyLog::message("End SrvBase::SrvExit");

	return true;
}

bool SrvBase::SrvExit()
{
	m_SrvNetThread.Exit();
	m_SrvNetThread.Wait();

	SAFE_DELETE(m_CltPktProc);
	SAFE_DELETE(m_SrvNetMgr);

	return true;
}

void SrvBase::FillSrvConfig()
{
	MyLog::message("Srv Use IOCP Net Mode: %d", m_SrvUseIOCP);

	ConfigManager::GetConfigValue( "CommonConfig", "SrvIP", m_SrvIP, true);
}

NetManager* SrvBase::CreateCltNetManager(bool bLZOCompress, int32 nSockRcBufSize, int32 nRcBufferSize, int32 nSockSnBuffSize, int32 nSnBufferSize, FunctionBase_Arg1<int32>* funcAccpet /*= NULL*/, FunctionBase_Arg1<int32>* funcCon /*= NULL*/, FunctionBase_Arg1<int32>* funcDiscon /*= NULL*/, int32 MAX_SOCKETS /*= MY_SOCKET_LIST_SIZE*/)
{
	return new BASENETMANAGER( bLZOCompress, nSockRcBufSize, nRcBufferSize, nSockSnBuffSize, nSnBufferSize, funcAccpet, funcCon, funcDiscon, MAX_SOCKETS);
}


MyPacketProc* SrvBase::CreateCltPktProcessor()
{
	MyLog::message("Create Default Srv Packet Processor");
	return new MyPacketProc();
}

void SrvBase::SrvAccept(int32 nSocketID)
{
	CltNetOperation( nSocketID, eNetEvent_Accept);

	LOCK(&m_listMutex);
	MyLog::message("Accept Clt Connection. SocketID = %d", nSocketID);

	CltNewConnection* pCon = FACTORY_NEWOBJ(CltNewConnection);
	if(!pCon)
		return;

	pCon->nSocketID = nSocketID;
	pCon->nTimeOffVal = SERVER_NEW_CONNECTION_TIMEOUT;

	m_CltNewConnections.insert(std::make_pair( nSocketID, pCon));
}

void SrvBase::SrvDisconnect(int32 nSocketID)
{
	CltNetOperation( nSocketID, eNetEvent_Disconnect);

	MyLog::message("SrvBase::SrvDisconnect(). SocketID=%d", nSocketID);

	LOCK(&m_listMutex);
	std::map< int32, CltNewConnection*>::iterator itr = m_CltNewConnections.find(nSocketID);
	if( itr != m_CltNewConnections.end() )
	{
		FACTORY_DELOBJ(itr->second);
		m_CltNewConnections.erase(itr);
	}
}

void SrvBase::CltNetOperation(int32 nSocketID, int32 nNetEvent)
{
	PacketNetEvent* pkt = (PacketNetEvent*)PacketFactory::Instance().New( PacketNetEvent::GetClassStatic()->ClassID() );
	pkt->nSocketID = nSocketID;
	pkt->nFlag = nNetEvent;

	m_CltPktProc->PushPacket(pkt);
}

void SrvBase::RegCltPktHandle(PacketProcessor* pProc)
{
	REG_DEFAULT_PACKET_HANDLER( pProc, PacketBase, SrvBase, DftCltPktHandle);
	REG_PACKET_HANDLER( pProc, PacketNetEvent, SrvBase, PktCltNetEvent);
}

void SrvBase::PktCltNetEvent(PacketNetEvent* pPkt)
{
	if(!pPkt)
		return;

	int32 nSocketID = pPkt->nSocketID;
	if( pPkt->nFlag == eNetEvent_Disconnect )
	{
		MyLog::message("Clt Disconnect Srv because receive packetnetevent with flag = eNetEvent_Disconnect. SocketID = %d", nSocketID);
		OnCltDisconnect(nSocketID);
	}
}
