#pragma once

#include "WorldStateGame.h"

class WorldState_WaitingNodeData : public WorldStateGame, public Singleton<WorldState_WaitingNodeData>
{
public:
	WorldState_WaitingNodeData(void);
	~WorldState_WaitingNodeData(void);

	virtual int32 GetState() { return eWS_WaitingNodeData; }
	virtual void RegPeerPktHandle(PacketProcessor* pProc);

	virtual void OnEnterState(WorldAvatar* pAvatar);
	virtual void OnLeaveState(WorldAvatar* pAvatar);

	virtual void Tick(int32 nFrameTime);

	virtual void Destroy(WorldAvatar* pAvatar);

protected:
	void PktNodeDataSync_AvatarData(class PacketNodeDataSyncAvatar* pPkt);
	void PktNodeDataSync_CommonData( class PacketNodeDataSyncCommonData* pPkt);
	void PktNodeDataSync_Fin( class PacketNodeDataSyncFin* pPkt);
	void PktNodeDataSync_AllFinish(class PacketNodeDataSyncAllFinish* pPkt);

	const static int32	MAX_WAIT_TIME = 5*60*1000;
	std::set<int64>		m_avatarDIDs;
};

