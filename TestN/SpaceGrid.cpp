#include "StdAfx.h"
#include "SpaceGrid.h"
#include "MyAvatar.h"

SpaceGrid::SpaceGrid(void)
{
}


SpaceGrid::~SpaceGrid(void)
{
}

void SpaceGrid::SetGridDirty()
{

}

void SpaceGridMap::OnAvatarAction(MyAvatar* pAvatar, bool bEnter)
{
	int32 nAvatrID = pAvatar->GetAvatarID();

	for (auto itr = begin(); itr != end(); ++itr)
	{
		SpaceGrid* pGrid = itr->second;
		assert(pGrid);

		if ( pAvatar->IsPlayer() )
		{
			if( bEnter )
			{
				pGrid->m_ArroundGridsPlayerOrignServerLink
			}
		}
	}

}
