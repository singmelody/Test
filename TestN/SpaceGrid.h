#pragma once

#include "Factory.h"
#include "ID2ItemMap.h"
#include <list>
#include "CasterTrunkContainer.h"

class MyAvatar;
class MyPlayer;
class MyNPC;


class SceneGridAvatarMap : public ID2ItemMap< int32, MyAvatar>
{
	DECLARE_FACTORY_ARG0( SceneGridAvatarMap, -1, new PoolAllocator);
public:

};

class SpaceGridList : public std::list<SpaceGrid*>
{

};

class SpaceGridMap : public ID2ItemMap< int32, SpaceGrid>
{
public:
	void OnAvatarEnter( MyAvatar* pAvatar)
	{
		OnAvatarAction( pAvatar, true);
	}

	void OnAvatarExit( MyAvatar* pAvatar)
	{
		OnAvatarAction( pAvatar, false);
	}

protected:
	void OnAvatarAction( MyAvatar* pAvatar, bool bEnter);
};

class SpaceGrid
{
	DECLARE_FACTORY_ARG0( SpaceGrid, -1, new PoolAllocator);
public:
	SpaceGrid(void);
	~SpaceGrid(void);

	void OnAvatarHideFromCltAOI( MyAvatar& pAvatar);

	void EnterGrid( MyAvatar* pAvatar);
	void EnterGrid_Player( MyPlayer* pPlayer);
	void EnterGrid_NPC( MyNPC* pNpc );

	void ExitGrid( MyAvatar* pAvatar, SpaceGrid* pNewGrid);
	void ExitGrid_Player( MyPlayer* pPlayer);
	void ExitGrid_NPC( MyNPC* pNpc );

	SpaceGridMap	m_ArroundGrids;	// ������Ե���Χ�ĸ����б�

	CasterTrunkGate m_AroundGridsPlayersOrganizeByLinkSrv;	// ��Χ����Ұ�GateServer�ķ���
	CasterTrunkGate m_LocalGridPlayersOriganizeByLinkSrv;	// ������Ұ�GateServer�ķ���

	enum ActionType
	{
		eAction_Enter,
		eAction_Exit,
	};

	void SetGridDirty();
};

