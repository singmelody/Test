#pragma once

#include "GateBase.h"

class GateServer : public GateBase, public SrvPacketTestManager, public Singleton<GateServer>
{
public:
	GateServer(void);
	~GateServer(void);

	virtual void ProcessLogic(int32 nFrameTime);

	virtual MyPacketProc* CreatePeerPktProc();
	virtual void RegPeerPktHandle(MyPacketProcessor* pProc);
	virtual void DftPeerPktHandle(PacketBase* pkt);

};

