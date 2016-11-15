#pragma once

#include "WorldSceneInfo.h"
#include "WorldScene.h"
#include "SceneManager.h"

class WorldAvatar;

//--------------------------------------------------------------
// scene create arg
class CopyOwnerArg
{
public:
	CopyOwnerArg(){}
	virtual ~CopyOwnerArg() {}

	virtual bool CheckOwner( WorldAvatar* pAvatar) = 0;
};

class CopyOwnerArg_Personal : public CopyOwnerArg
{
	DECLARE_FACTORY_ARG0( CopyOwnerArg_Personal, -1, new PoolAllocator )
public:
	CopyOwnerArg_Personal() : m_nAvatarDID(0) {}
	virtual bool CheckOwner( WorldAvatar* pAvatar);
	int64 m_nAvatarDID;
};

class CopyOwnerArg_Team : public CopyOwnerArg
{
	DECLARE_FACTORY_ARG0( CopyOwnerArg_Team, -1, new PoolAllocator )
public:
	CopyOwnerArg_Team() : m_nTeamID(0) {}
	virtual bool CheckOwner( WorldAvatar* pAvatar);
	int64 m_nTeamID;
};

class CopyOwnerArg_Guild : public CopyOwnerArg
{
	DECLARE_FACTORY_ARG0( CopyOwnerArg_Guild, -1, new PoolAllocator )
public:
	CopyOwnerArg_Guild(): m_nGuildID(0) {}
	virtual bool CheckOwner( WorldAvatar* pAvatar);
	int64 m_nGuildID;
};


//----------------------------------------
// world scene
class WorldScene_Copy : public WorldScene
{
	DECLARE_FACTORY_ARG0( WorldScene_Copy, -1, new PoolAllocator);
public:
	WorldScene_Copy();
	virtual ~WorldScene_Copy();

	virtual bool IsCopy() const { return true; }

	virtual bool CheckEnterScene(WorldAvatar* pAvatar) const;
	virtual void UpdateCopyOwner(WorldAvatar* pAvatar);
	virtual bool CheckCopyOwner(WorldAvatar* pAvatar) const;

	void SetOwnerArg(CopyOwnerArg* pArg) { m_pCopyOwnerArg = pArg;}

protected:
	CopyOwnerArg*		m_pCopyOwnerArg;
};

class WorldScene_TeamCopy : public WorldScene_Copy
{
	DECLARE_FACTORY_ARG0( WorldScene_TeamCopy, -1, new PoolAllocator);
public:
	WorldScene_TeamCopy(void);
	virtual ~WorldScene_TeamCopy(void){}

	virtual void UpdateCopyOwner(WorldAvatar* pAvatar);
	virtual bool CheckCopyOwner(WorldAvatar* pAvatar) const;
protected:
	int64 m_nTeamID;
};

class WorldScene_TokenCopy : public WorldScene_Copy
{
	DECLARE_FACTORY_ARG0( WorldScene_TokenCopy, -1, new PoolAllocator);
public:
	WorldScene_TokenCopy();

	virtual void UpdateCopyOwner(WorldAvatar* pAvatar){}
	virtual bool CheckCopyOwner(WorldAvatar* pAvatar) const;
};

class WorldScene_PersonalCopy : public WorldScene_Copy
{
	DECLARE_FACTORY_ARG0( WorldScene_PersonalCopy, -1, new PoolAllocator);

};

class WorldScene_GuildCopy : public WorldScene_Copy
{
	DECLARE_FACTORY_ARG0( WorldScene_GuildCopy, -1, new PoolAllocator);

};