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
	virtual ~WorldSceneInfo(void);

	virtual bool LoadSceneInfo( DBRow& row);

	virtual void OnEnterScene( WorldAvatar* pAvatar, WorldScene* pScene) {}
	virtual void OnLeaveScene( WorldAvatar* pAvatar, WorldScene* pScene) {}

	virtual bool TryEnterTargetScene(WorldAvatar* pAvatar, int16 nInstanceID, int32& nFailReason);

	virtual int32 AllocSceneID();
protected:
	bool TryEnterScene( WorldAvatar* pAvatar, WorldScene* pScene, int32& nFailReason);

	class WorldSceneManager& SceneMgr;
	class WorldAsynContextManager& ContextMgr;
	class WorldEnterManager& EnterMgr;
};