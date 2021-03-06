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
	
	virtual void OnEnterState(WorldAvatar* pAvatar);
	virtual void OnLeaveState(WorldAvatar* pAvatar);
	virtual void DestroyAvatar(WorldAvatar* pAvatar);

	void PktGate_CreateAvatar( class PacketCreateAvatar* pPkt);
	void PktGate_CreateAvatarRes( class PacketCreateAvatarRes* pPkt);

	void PktGate_DelCharacter( class PacketDelCharacter* pPkt);
	void PktGate_DelCharacterRes( class PacketDelCharacterFinish* pPkt);
	
	void PktGate_CltSelectAvatar( class PacketCltSelectAvatar* pPkt);
};

