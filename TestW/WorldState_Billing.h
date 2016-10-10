#pragma once

#include "WorldState.h"

class WorldState_Billing : public WorldState, public Singleton<WorldState_Billing>
{
public:
	WorldState_Billing(void);
	~WorldState_Billing(void);

	virtual int32 GetState() { return eWS_Billing; }
	virtual void RegPeerPktHandle( class PacketProcessor* pProc);

	virtual void OnEnterState(WorldAvatar* pAvatar);
	virtual void Tick(int32 nFrameTime);

	void PktLogin_BillingResponseForLeaveGame(class PacketBillingResponseForLeaveGame* pPkt);
};

