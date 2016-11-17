#pragma once

#include "DBInterface.h"
#include "WorldSceneInfo.h"
#include "SceneManager.h"

class WorldAvatar;

class WorldSceneInfo_Trunk : public WorldSceneInfo
{
public:
	WorldSceneInfo_Trunk();
	virtual ~WorldSceneInfo_Trunk(){}

	virtual bool IsCopy() const { return false; }

	virtual void OnSceneCreateSucceed( WorldScene* pScene);
	virtual void OnSceneCreateFailed(WorldScene* pScene);

	int32 HandleCreateParallel( int32 nNodeSrvID );

	int32 GetSceneNormalVolumn();
	int32 GetRequestTotalVolumn();

	SceneRequestList RequestList;

	int32	nCreatingSceneCount;
};

class WorldSceneInfo_MainTrunk : public WorldSceneInfo_Trunk
{
public:
	WorldSceneInfo_MainTrunk(void);
	virtual ~WorldSceneInfo_MainTrunk(void);

	virtual bool LoadSceneInfo(DBRow& row);
	virtual bool TryEnterTargetScene( WorldAvatar* pAvatar, int16 nInstanceID, int32& failreason);

	virtual void OnSceneCreate( Scene* pScene);
	virtual void OnSceneDestroy( Scene* pScene);

	virtual void OnEnterScene(WorldAvatar* pAvatar, WorldScene* pScene);
	virtual void OnLeaveScene(WorldAvatar* pAvatar, WorldScene* pScene);
	void TryStartParallel();
protected:
	void UpdateParallelBits();
	void BroadcastParallel2Nodes();
	bool TryEnterAnyParallel(WorldAvatar* pAvatar, int32& nFailReaon);
	bool HasSceneOnNodeServer(int32 nNodeSrvID);

	std::map<int64, int16>	_teamInstance;
	uint16					_parallelCount;
};


class WorldSceneInfo_Copy : public WorldSceneInfo
{
public:
	WorldSceneInfo_Copy();
	virtual ~WorldSceneInfo_Copy(){}

	virtual bool IsCopy() const { return true; }

	virtual bool TryEnterTargetScene(WorldAvatar* pAvatar, int16 nInstanceID, int32& nFailReason);
	virtual bool TryRebuildSceneCopy(WorldAvatar* pAvatar);

	virtual bool HandleEnterSceneExist(int32 nSceneID, WorldAvatar* pAvatar);

	virtual WorldScene* HandleCreateCopy( WorldAvatar* pAvatar, int64 nSceneProcessBits, int64 nSceneCustomData = 0);

	virtual CreateSceneRst HandleCreateCopyRequest( WorldAvatar* pAvatar, int64 nSceneProcessBits);

	virtual void OnSceneCreateSucceed(WorldScene* pScene);
	virtual void OnSceneCreateFailed(WorldScene* pScene, int32 nErrorID);
};

class WorldSceneInfo_TeamCopy : public WorldSceneInfo_Copy
{
public:
	WorldSceneInfo_TeamCopy();
	virtual ~WorldSceneInfo_TeamCopy(){}

	virtual void OnEnterScene( WorldAvatar* pAvatar, WorldScene* pScene);

	WorldScene* GetTeamExistScene( class WorldTeam* pTeam);
	virtual bool HandleEnterSceneExist(int32 nSceneID, WorldAvatar* pAvatar);
	virtual CreateSceneRst HandleCreateCopyRequest( WorldAvatar* pAvatar, int64 nSceneProcessBits);
protected:
	class WorldTeamManager& TeamMgr;
};

class WorldSceneInfo_TokenCopy : public WorldSceneInfo_Copy
{};

class WorldSceneInfo_MainTrunkCopy : public WorldSceneInfo_TeamCopy
{};

class WorldSceneInfo_ScenarioCopy : public WorldSceneInfo_TeamCopy
{};

class WorldSceneInfo_RootCopy : public WorldSceneInfo_TeamCopy
{};