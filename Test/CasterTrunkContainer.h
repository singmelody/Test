#pragma once

#include <map>
#include "Factory.h"

class AvatarSrv;
class CasterTrunkContainer;

class PlayerBranchContainer
{
	DECLARE_FACTORY_ARG0( PlayerBranchContainer, -1, new PoolAllocator);
public:
	PlayerBranchContainer();
	~PlayerBranchContainer();

	typedef std::map< int32, AvatarSrv*> PlayerMap;

	PlayerMap	m_mapPlayers;

	bool		AddPlayer( int32 castID, AvatarSrv* pPlayer);
	bool		RemovePlayer( int32 castID, AvatarSrv*);

	CasterTrunkContainer* m_pParentTrunk;
	int32 m_nBranchID;
};

typedef std::map< int32, PlayerBranchContainer*> PlayerBranchMap;

class CasterTrunkContainer
{
public:
	CasterTrunkContainer(bool bByNode);
	~CasterTrunkContainer(void);

	void AddPlayer( AvatarSrv* pPlayer );
	void AddPlayer( AvatarSrv* pPlayer, int32 nBranchID);

	void RemovePlayer( AvatarSrv* pPlayer );
	void RemovePlayer( AvatarSrv* pPlayer, int32 nBranchID);

	virtual bool IsEmpty() { return m_mapGateBranches.size() == 0; }

	PlayerBranchContainer* GetBranch( int32 nBranchID, bool bCreateNew);
	void RemoveBranch(PlayerBranchContainer* pBranch);
protected:
	const bool m_bByNode;
	PlayerBranchMap m_mapGateBranches;
};

class CasterTrunkGate : public CasterTrunkContainer
{
public:
	CasterTrunkGate() : CasterTrunkContainer(false){}
	~CasterTrunkGate(){}
};

class CasterTrunkNode : public CasterTrunkContainer
{
public:
	CasterTrunkNode() : CasterTrunkContainer(true){}
	~CasterTrunkNode(){}
};

