#include "StdAfx.h"
#include "SrvBase.h"
#include "MyLog.h"
#include "MyPacketProc.h"

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
	m_SrvNetManager = NULL;

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
		return NULL;

	MyLog::message("End SrvBase::SrvExit");
}

bool SrvBase::SrvExit()
{

}

void SrvBase::FillSrvConfig()
{

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

void SrvBase::RegCltPktHandle(PacketProcessor* pProc)
{
	REG_DEFAULT_PACKET_HANDLER( pProc, PacketBase, SrvBase, DftCltPktHandle);
	REG_PACKET_HANDLER( pProc, PacketNetEvent, SrvBase, PktCltNetEvent);
}
