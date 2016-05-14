#pragma once

#include "AvatarSrvEx.h"

class WorldTeam;
class WorldGuid;

class WorldAvatarGroup  : public AvatarSrvEx
{
public:
	WorldAvatarGroup(void);
	virtual ~WorldAvatarGroup(void);

	virtual void SetNodeSrvID( int32 nID);

	virtual int64 GetTeamID();
	virtual void SetTeamID(int64 nID);

	virtual void OnAfterPullDataFromNode();
	virtual void OnAfterPullDataFromDBA();

	WorldTeam* GetWorldTeam();
	void SetWorldTeam( WorldTeam* pTeam);

	WorldGuid* GetWorldGuid();
	void GetPosition( f32& x, f32 &y, f32& z) {}

	virtual bool CheckState( int32 nState);
protected:
	int64	m_nTeamID;
};

