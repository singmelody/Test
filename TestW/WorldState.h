#pragma once

class WorldAvatar;
class PacketProcessor;

class WorldState : public WorldFuncExManager
{
public:
	WorldState(void);
	virtual ~WorldState(void);

	virtual int32 GetState() { return eWS_Null; }
	virtual void OnEnterStage( WorldAvatar* pAvatar);
	virtual void OnLeaveStage( WorldAvatar* pAvatar);

	virtual void RegPeerPktHandle( PacketProcessor* pProc){}

	virtual void Tick(int32 nFrameTime) {}

	virtual bool CheckInStage( WorldAvatar* pAvatar, const char* pSzErrorLocation);

	virtual void OnGateClosed(int32 nSrvID);
	virtual void OnNodeCrashed( int32 nSrvID, bool bUseSHM) {}
	virtual void OnSceneClosed( Scene* pScene) {}
	virtual void OnPullAvatarDataFinish( WorldAvatar* pAvatar) {}

	virtual void OnCreateNodeAvatarSucceed( WorldAvatar* pAvatar) {}
	virtual void OnCreateNodeAvatarFailed( WorldAvatar* pAvatar, int32 nErrorID){}

	virtual void DestroyAvatar( WorldAvatar* pAvatar);
	TickList m_listAvatars;

protected:
	WorldAvatar* GetWorldAvatarAndCheckStage( int32 nAvatarID, const char* pSzaErrorLocation);
};

