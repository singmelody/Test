#pragma once

#include "WorldStateGame.h"

class WorldState_WritingDBA : public WorldStateGame, public Singleton<WorldStateGame> 
{
public:
	WorldState_WritingDBA(void);
	virtual ~WorldState_WritingDBA(void);

	virtual int32 GetState() { return eWS_WritingDBA; }
	virtual void RegPeerPktHandle(PacketProcessor* pProc);

	virtual void OnEnterState(WorldAvatar* pAvatar);

	virtual void Tick(int32 nFrameTime);
	virtual void DestroyAvatar( WorldAvatar* pAvatar);
protected:
	void PktDBADecommision(class PacketAvatarDecommision* pPkt);
	const static int32 MAX_WAIT_TIME = 60*1000;
};

