#pragma once

#include "WorldStateGame.h"

class WorldState_Gaming : public WorldStateGameNode
{
public:
	WorldState_Gaming(void);
	virtual ~WorldState_Gaming(void);

	virtual int32 GetState() { return eWS_Gaming; }
	virtual void OnEnterState( WorldAvatar* pAvatar);
	virtual void Tick(int32 nFrameTime);

	virtual void OnGateClosed(int32 nSrvID);
	virtual void OnSceneClosed( Scene* pScene);
	virtual void OnNodeCrashed( int32 nSrvID, bool bUseSHM, WorldAvatar* pAvatar);

	virtual void DestroyAvatar( WorldAvatar* pAvatar);
	int32 ProcessCreateSceneRequest( class PacketCreateSceneRequest* pPkt);

	virtual void RegPeerPktHandle( PacketProcessor* pProc);
protected:
	void ExitGame( WorldAvatar* pAvatar);

	void PktNode_AvatarDataBackup( class PacketAvatarDataBackup* pPkt);	// roll data schedule save notify
	void PktLogout2Login( class PacketLogout2Login* pPkt);
	void PktNode_ChangeSceneRequest(class PacketChangeSceneRequest* pPkt);
	void PktNode_TickOut(class PacketNodeKickOut* pPkt);

	void PktNode_CommonDataUpdate(class PacketCommonDataUpdate* pPkt);
	void PktNode_CommonDataCreate(class PacketCommonDataCreate* pPkt);
	void PktNode_CommonDataDelete(class PacketCommonDataDelete* pPkt);

	void PktNode_CreateSceneRequest(class PacketCreateSceneRequest* pPkt);
};


