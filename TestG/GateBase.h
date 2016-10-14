#pragma once

#include "PeerSrvModuleBase.h"

class GateCltNetChannel;

class GateBase : public PeerSrvModuleBase
{
public:
	GateBase(void);
	~GateBase(void);

	GateCltNetChannel* GetCltChannel(int32 nSocketID);
	void FreeCltChannel(GateCltNetChannel* pChannel);
protected:

	bool	m_bSrvReady;
	int32	m_nGateTickTime;

	int32	m_nWarningCachedPacket2Clt;
	int32	m_nMaxCachedPacket2Clt;
};

