#pragma once

#include "AvatarSrv.h"
#include "CommonDataOwner.h"
#include "CommonDataManager.h"

class Scene;
class Team;

class AvatarSrvEx : public AvatarSrv, public CommonDataOwner
{
public:
	AvatarSrvEx(void);
	virtual ~AvatarSrvEx(void);

	void SetCommonDataManager( CommonDataManager* pMgr);
	virtual CommonDataManager* GetCommonDataManager();

	virtual Scene* GetScene();
	virtual void SetScene( Scene* pScene);

	int64 GetGuildID();
	void SetGuildID(int64 nGuildID);

	int16 GetSceneSID();
	int16 GetSceneInstanceID();

	virtual int64 GetTeamID();
	virtual void SetTeamID(int64 nID);
	virtual Team* GetTeam() { return NULL; }
	
	const char* GetName();
	const char* GetAccountName();

	void ReleaseCommonDataConts();
	void ReleaseCommonDataConts(CommonDataType type);
protected:
	int32				m_nSceneID;

	CommonDataManager*	m_pCommonDataMgr;
};

