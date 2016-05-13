#pragma once

#include "WorldState.h"
#include "Singleton.h"

class WorldState_Logined : public WorldState, public Singleton<WorldState_Logined>
{
public:
	WorldState_Logined(void);
	~WorldState_Logined(void);

	virtual int32 GetState() { return eWS_Logined; }
	virtual void RegPeerPktHandle(PacketProcessor* pProc);
	
	virtual void OnEnterStage(WorldAvatar* pAvatar);
	virtual void OnLeaveStage(WorldAvatar* pAvatar);
	virtual void DestroyAvatar(WorldAvatar* pAvatar);

	void PktGate_CreateAvatar( class PacketCreateAvatar* pPkt);
	void PktGate_CreateAvatarRes( class PacketCreateAvatarRes* pPkt);

	void PktGate_DelAvatar( class PacketDelAvatar* pPkt);
	void PktGate_DelAvatarRes( class PacketDelAvatarRes* pPkt);
	
	void PktGate_CltSelectAvatar( class PacketCltSelectAvatar* pPkt);
};

