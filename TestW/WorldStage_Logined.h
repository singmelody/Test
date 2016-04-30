#pragma once

#include "WorldStage.h"
#include "Singleton.h"

class WorldStage_Logined : public WorldStage, public Singleton<WorldStage_Logined>
{
public:
	WorldStage_Logined(void);
	~WorldStage_Logined(void);

	void OnEnterStage(WorldAvatar* pAvatar);
	void OnLeaveStage(WorldAvatar* pAvatar);

	void RegPeerPktHandle(PacketProcessor* pProc);
	void PktGate_CreateAvatar( class PacketCreateAvatar* pPkt);
	void PktGate_CreateAvatarRes( class PacketCreateAvatarRes* pPkt);

	void PktGate_DelAvatar( class PacketDelAvatar* pPkt);
	void PktGate_DelAvatarRes( class PacketDelAvatarRes* pPkt);
	
	void PktGate_CltSelectAvatar( class PacketCltSelectAvatar* pPkt);
};

