#pragma once

#include "WorldState.h"

class PacketProcessor;

class WorldState_Logining : public WorldState, public Singleton<WorldState_Logining>
{
public:
	WorldState_Logining(void);
	~WorldState_Logining(void);

	virtual void OnEnterState(class WorldAvatar* pAvatar);
	virtual void OnLeaveState(class WorldAvatar* pAvatar);

	virtual void Tick(int32 nFrameTime);

	virtual void RegPeerPktHandle( PacketProcessor* pProc);
	virtual void DestroyAvatar(class WorldAvatar* pAvatar);
protected:
	void PktDBA_UserData(class PacketUserData* pPkt);
	void PktDBA_UserDataReqFinish(class PacketUserDataRegFinish* pPkt);

};

