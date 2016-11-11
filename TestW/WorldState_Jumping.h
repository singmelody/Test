#pragma once

#include "Singleton.h"
#include "WorldStateGame.h"


class WorldState_Jumping : public WorldStateGameNode, public Singleton<WorldState_Jumping>
{
public:
	WorldState_Jumping(void);
	virtual ~WorldState_Jumping(void);

	virtual int32 GetState() { return eWS_Jumping; }
	virtual void OnEnterState(WorldAvatar* pAvatar);
	virtual void Tick(int32 nFrameTime);

	virtual void DestroyAvatar(WorldAvatar* pAvatar);
	virtual void OnNodeCrashed(int32 nSrvID, bool bUseSHM, WorldAvatar* pAvatar, bool bWaitData);

	virtual void RegPeerPktHandle(PacketProcessor* pProc);

	void PktNode_ChangeSceneNotify( class PacketChangeSceneNotify* pkt);
	void PktNode_DestroyAvatar( class PacketDestroyAvatar* pkt);
};

