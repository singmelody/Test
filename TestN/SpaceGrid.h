#pragma once

#include "Factory.h"
#include "ID2ItemMap.h"

class MyAvatar;
class MyPlayer;
class MyNPC;

class SpaceGridMap : public ID2ItemMap< int32, SpaceGrid>
{

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

	SpaceGridMap	m_ArroundGrids;
};

