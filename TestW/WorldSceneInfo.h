#pragma once

#include "CircularList.h"
#include "CircularQueue.h"
#include "BaseType.h"
#include "SceneInfo.h"
#include "WorldEnterManager.h"

class WorldAvatar;
class WorldScene;

class SceneRequestList : public CircularQueue< int32, 10240>
{
public:
	SceneRequestList();
};

class WorldSceneInfo : public SceneInfoEx
{
public:
	WorldSceneInfo(void);
	~WorldSceneInfo(void);

	virtual bool LoadSceneInfo( DBRow& row);

	virtual void OnEnterScene( WorldAvatar* pAvatar, WorldScene* pScene) {}
	virtual void OnLeaveScene( WorldAvatar* pAvatar, WorldScene* pScene) {}

	virtual bool TryEnterTargetScene(WorldAvatar* pAvatar, int16 nInstanceID, int32& nFailReason);
protected:
	bool TryEnterScene( WorldAvatar* pAvatar, WorldScene* pScene, int32& nFailReason);

	class WorldSceneManager& SceneMgr;
	class WorldAsynContextManager& ContextMgr;
	class WorldEnterManager& EnterMgr;
};


class WorldSceneInfo_Trunk : public WorldSceneInfo
{
public:
	WorldSceneInfo_Trunk();

	virtual bool IsCopy() const { return false; }

	virtual void OnSceneCreateSucceed( WorldScene* pScene);
	virtual void OnSceneCreateFailed(WorldScene* pScene);

	int32 HandleCreateParallel( int32 nNodeSrvID );

	int32 GetSceneNormalVolumn();
	int32 GetRequestTotalVolumn();

	SceneRequestList RequestList;

	int32	nCreatingSceneCount;
};
