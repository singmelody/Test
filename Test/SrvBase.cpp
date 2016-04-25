#include "StdAfx.h"
#include "SrvBase.h"

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
{
}


SrvBase::~SrvBase(void)
{
}

NetManager* SrvBase::CreateCltNetManager(bool bLZOCompress, int32 nSockRcBufSize, int32 nRcBufferSize, int32 nSockSnBuffSize, int32 nSnBufferSize, FunctionBase_Arg1<int32>* funcAccpet /*= NULL*/, FunctionBase_Arg1<int32>* funcCon /*= NULL*/, FunctionBase_Arg1<int32>* funcDiscon /*= NULL*/, int32 MAX_SOCKETS /*= MY_SOCKET_LIST_SIZE*/)
{
	return new BASENETMANAGER( bLZOCompress, nSockRcBufSize, nRcBufferSize, nSockSnBuffSize, nSnBufferSize, funcAccpet, funcCon, funcDiscon, MAX_SOCKETS);
}


void SrvBase::RegCltPktHandle(PacketProcessor* pProc)
{
	REG_DEFAULT_PACKET_HANDLER( pProc, PacketBase, SrvBase, DftCltPktHandle);
}