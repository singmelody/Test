#pragma once
class WorldSceneInfo : public SceneInfoEx
{
public:
	WorldSceneInfo(void);
	~WorldSceneInfo(void);

	virtual bool LoadSceneInfo( DBRow& row);

	virtual void OnEnterScene( WorldAvatar* pAvatar, WorldScene* pScene) {}
	virtual void OnLeaveScene( WorldAvatar* pAvatar, WorldScene* pScene) {}
protected:
	bool TryEnterScene( WorldAvatar* pAvatar, WorldScene* pScene, int32& nFailReason);

	class WorldSceneManager& SceneMgr;
	class WorldAysnManager& ContextMgr;
	class WorldEnterManager& EnterMgr;
};


class WorldSceneInfo_Trunk : public WorldSceneInfo
{
public:
	WorldSceneInfo();

	virtual bool IsCopy() const { return false; }

	virtual void OnSceneCreateSucceed( WorldScene* pScene);
	virtual void OnSceneCreateFailed(WorldScene* pScene);

	int32 HandleCreateParallel( int32 nNodeSrvID );

	int32 GetSceneNormalVolumn();
	int32 GetRequestTotalVolumn();

	SceneRequestList RequestList;

	int32	nCreatingSceneCount;
};
