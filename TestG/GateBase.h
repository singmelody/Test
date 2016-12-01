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

	bool IsServerReady() { return m_bSrvReady; }

	inline int32 GetWarningCachedPacket2Clt() const { return m_nWarningCachedPacket2Clt; }
	inline int32 GetMaxCachedPacket2Clt() const { return m_nMaxCachedPacket2Clt; }
protected:
	virtual void OnRemoveWorldInfo(ServerInfo* pInfo);

	bool	m_bSrvReady;
	int32	m_nGateTickTime;

	int32	m_nWarningCachedPacket2Clt;
	int32	m_nMaxCachedPacket2Clt;
};

