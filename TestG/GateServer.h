#pragma once

#include "GateBase.h"
#include "SrvPacketTestManager.h"
#include "MyPacketProc.h"

class GateServer : public GateBase, public SrvPacketTestManager, public Singleton<GateServer>
{
public:
	GateServer(void);
	~GateServer(void);

	virtual void ProcessLogic(int32 nFrameTime);

	virtual MyPacketProc* CreatePeerPktProc();

	virtual void RegPeerPktHandle(class PacketProcessor* pProc);
	virtual void DftPeerPktHandle(PacketBase* pkt);

	virtual MyPacketProc* CreateCltPktProcessor();
	virtual void RegCltPktHandle(PacketProcessor* pPkt);
	virtual void DftCltPktHandle(PacketBase* pPkt);

	virtual void OnCltDisconnect(int32 nSocketID);

	bool Init(int32 nArgc, char* argv[]);
};

