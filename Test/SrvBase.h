#pragma once
#include "IRecvPacketFilter.h"
#include "MySockAddr.h"
#include "FunctionBase.h"
#include "Thread.h"

class NetManager;

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

protected:
	void SrvThreadLoop();	

	MyThread		m_SrvNetThread;
	NetManager*		m_SrvNetMgr;
	bool			m_SrvUseIOCP;

	std::string		m_SrvIP;
	int32			m_SrvPort;
	int32			m_SrvSocketRcBufSize;
	int32			m_SrvSocketRnBufSize;

	int32			m_SrvCirRcBufSize;
	int32			m_SrvCirRnBufSize;
	
	bool			m_bSrvEnableLZO;
	bool			m_bSrvPacketUseIndexWhenSend;
	bool			m_bSrvPacketUseIndexWhenRecv;

	int32			m_SrvNetThreadFrameTime;
};

