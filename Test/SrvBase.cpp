#include "StdAfx.h"
#include "SrvBase.h"
#include "MyLog.h"
#include "MyPacketProc.h"
#include "FunctionBase.h"
#include "MyMutex.h"
#include "PacketImpl.h"
#include "WatchDog.h"
#include "ConfigManager.h"
#include "GameUtil.h"

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
	m_SrvSocketSnBufSize = 2048;
	m_SrvCirSnBufSize = 2048*32;

	m_SrvIP = "127.0.0.1";
	m_SrvPort = 10001;

	m_SrvNetThreadFrameTime = 0;
	m_SrvMaxSockets = 0;
	m_nSrvNetEventWaitTime = 0;
	m_pRecvPacketFilterOfSrvBase = NULL;

	m_bUseSrvNetDelay = NET_TEST_USE_SRV_SEND_DELAY;
	m_SrvNetDelayMin = NET_TEST_USE_SRV_SEND_DELAY_MIN;
	m_SrvNetDelayMax = NET_TEST_USE_SRV_SEND_DELAY_MAX;
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

	NetManager* pMgr = CreateCltNetManager( m_bSrvEnableLZO, m_SrvSocketRcBufSize, m_SrvCirRcBufSize, m_SrvSocketSnBufSize, m_SrvCirSnBufSize,
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
	MyLog::message("SrvIP : %s", m_SrvIP.c_str());

	ConfigManager::GetConfigValue("CommonConfig", "SrvPort", m_SrvPort, true);
	MyLog::message("SrvPort : %d", m_SrvPort);

	ConfigManager::GetConfigValue( "CommonConfig", "SrvSocketSnBufSize", m_SrvSocketSnBufSize);
	if( m_SrvSocketSnBufSize < 2048)
		m_SrvSocketSnBufSize = 2048;
	MyLog::message("Srv SrvSockSNBufSize: %d", m_SrvSocketSnBufSize);

	ConfigManager::GetConfigValue( "CommonConfig", "SrvSnBufSize", m_SrvCirSnBufSize);
	if( m_SrvCirSnBufSize < m_SrvSocketSnBufSize)
		m_SrvCirSnBufSize = m_SrvSocketSnBufSize*2;
	MyLog::message("Srv SrvSockSNBufSize: %d", m_SrvCirSnBufSize);

	ConfigManager::GetConfigValue( "CommonConfig", "SrvSockRcBufSize", m_SrvSocketRcBufSize);
	if( m_SrvSocketRcBufSize < 2048)
		m_SrvSocketRcBufSize = 2048;
	MyLog::message("Srv SrvSockRcBufSize: %d", m_SrvCirSnBufSize);

	ConfigManager::GetConfigValue( "CommonConfig", "SrvRcBufSize", m_SrvCirRcBufSize);
	if( m_SrvCirRcBufSize < m_SrvSocketRcBufSize)
		m_SrvCirRcBufSize = m_SrvSocketRcBufSize*2;
	MyLog::message("Srv SrvSockSNBufSize: %d", m_SrvCirRcBufSize);

	if( ConfigManager::GetConfigValue( "CommonConfig", "SrvDelay", m_bUseSrvNetDelay, true))
		MyLog::message("Use Srv Net Delay");

	if( ConfigManager::GetConfigValue( "CommonConfig", "SrvDelayMin", m_SrvNetDelayMin, true))
		MyLog::message("Use Srv Net Delay Min");

	if( ConfigManager::GetConfigValue( "CommonConfig", "SrvDelayMax", m_SrvNetDelayMax, true))
		MyLog::message("Use Srv Net Delay Max");

	// bla bla bla
}

void SrvBase::CloseCltConnection(int32 nSocketID)
{
	if(!m_SrvNetMgr)
		return;

	m_SrvNetMgr->Disconnect( nSocketID );
}

void SrvBase::SrvSend(int32 nSocketID, PacketBase* pkt)
{
	m_SrvNetMgr->SendPacket( *pkt, nSocketID);
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

void SrvBase::SrvThreadLoop()
{
	uint64 tStart = Time::CurrentTime().MilliSecond();

	WatchDog::Instance().NextStep( SRV_THREAD_WATCHDOG_ID );

	if( m_SrvNetMgr != NULL)
		m_SrvNetMgr->NetRun();

	uint64 tNow = Time::CurrentTime().MilliSecond();
	if( tNow >= tStart )
	{
		uint64 usedTime = tNow - tStart;
		if( usedTime < m_SrvNetThreadMinFrameTime )
		{
			GameUtil::Sleep(m_SrvNetThreadMinFrameTime - usedTime);
		}
	}

	WatchDog::Instance().NextStep( SRV_THREAD_WATCHDOG_ID );

	if(m_bUseSrvNetDelay)
	{
		int32 nDelayTime = m_SrvNetDelayMin + ( rand() % (m_SrvNetDelayMax - m_SrvNetDelayMin));
		GameUtil::Sleep(nDelayTime);
	}

	WatchDog::Instance().UpdateTime(SRV_THREAD_WATCHDOG_ID);
}

void SrvBase::ProcNewConnection(int32 nFrameTime)
{
	std::vector<int32> v2Delete;

	{
		LOCK(&m_listMutex);

		std::map<int32, CltNewConnection*>::iterator itr = m_CltNewConnections.begin();
		for (; itr != m_CltNewConnections.end(); ++itr)
		{
			assert(itr->second);
			if(itr->second->nTimeOffVal < 0)
			{
				// invalid record
			}
			else if( itr->second->nTimeOffVal <= nFrameTime )
			{
				v2Delete.push_back(itr->first);
				itr->second->nTimeOffVal = -1;
			}
			else
			{
				itr->second->nTimeOffVal -= nFrameTime;
			}

		}
	}

	{
		for(auto itr = v2Delete.begin(); itr != v2Delete.end(); ++itr)
			m_SrvNetMgr->Disconnect(*itr);
	}
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

int32 SrvBase::ProcCltPacket()
{
	if(!m_CltPktProc)
		return 0;

	return m_CltPktProc->DoProc();
}
