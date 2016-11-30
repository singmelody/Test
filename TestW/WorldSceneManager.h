#pragma once

#include "Singleton.h"
#include "SceneManager.h"
#include "WorldFuncManager.h"
#include "BaseType.h"
#include <map>
#include <list>
#include "GameObject.h"
#include "ID2ItemMap.h"

class Scene;
class SceneInfo;
class WorldScene;
class WorldAvatarManager;
class WorldSceneInfo;

class WorldSceneManager : public SceneManager, public WorldFuncManager, public Singleton<WorldSceneManager>
{
public:
	WorldSceneManager(void);
	~WorldSceneManager(void);

	WorldAvatarManager& AvatarMgr;

	virtual void RegPeerPktHandle(class PacketProcessor* pProc);

	virtual void OnSceneCreated( Scene* pScene);
	virtual void OnSceneDestroy( Scene* pScene);

	virtual SceneInfo* CreateSceneInfo( int32 nSceneType, int32 nPlayerMax);
	void OnCreatingSceneTimeout( WorldScene* pScene);
	void NotifyCreateSceneResult( int32 nNodeID, int32 nAvatarID, int32 nResult, WorldScene* pScene = NULL);
	void SyncParallelInfo2Node( int32 nNodeSrvID);

	void TickCreatingScenes(int32 nFrameTime);
	void TickClosingScenes( int32 nFrameTime);

	bool GetSceneCreateParam( int16 nSceneSID, SceneCreateArg& obj);
	WorldScene* CreateWorldScene( SceneCreateArg& arg);
	void OnNodeCrashed( int32 nNodeID = SERVERID_NULL);

	WorldScene* GetWorldScene(int32 nSceneID) { return (WorldScene*)GetScene(nSceneID); }
	WorldSceneInfo* GetWorldSceneInfo( uint16 nSceneSID);

	virtual bool Tick( int32 nFrameTime);

	void OnNodeCrash(int32 nNodeID = SERVERID_NULL);

	typedef ID2ItemMap< int32, WorldScene> CreatingSceneMap;
	CreatingSceneMap m_mapCreatingScenes;
protected:
	void PktNode_CreateNodeSceneRst(class PacketCreateNodeSceneRst* pPkt);
	void PktNode_SceneProcessBits(class PacketSceneProcessBits* pPkt);
	void PktNode_DestroySceneRequest(class PacketDestroySceneRequest* pPkt);
	void PktNode_WarScene(class PacketWarScene* pPkt);

	TickList m_ScenesTickList;

};

