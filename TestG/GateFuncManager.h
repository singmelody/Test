#pragma once

#include "ModuleFuncManager.h"

class GateFuncManager : public ModuleFuncManager
{
public:
	GateFuncManager(void);
	~GateFuncManager(void);

protected:
	void SrvSend( int32 nSocketID, PacketBase& pkt);
	void SrvSend( int32 nSocketID, PacketBase* pPkt);

	class GateServer& GateSrv;
};

