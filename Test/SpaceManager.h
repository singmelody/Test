#pragma once

#include "Factory.h"

class CSpaceManager
{
	DECLARE_FACTORY_ARG0( CSpaceManager, -1, new GPoolAllocater)
public:
	CSpaceManager(void);
	~CSpaceManager(void);

	virtual void Init(SceneInfo* pSi);

	void UpdateGrid( NodeAvatar* pAvatar);

	SpaceGrid* GetGridByIndex( int32 nIdxX, int32 nIdxY);
	SpaceGrid* GetGridByPos( Vector3& pos);

	int32 GetCellCount() { return cell_count; }

	SpaceGrid* GetCellByIdx(int32 nIdx)
	{
		if( nIdx < 0 || nIdx >= cell_count )
			return NULL;

		return m_cellList[nIdx];
	}

	void GetStartPos( f32& startX, f32& startY)
	{
		startX = startY = 0;
	}

	void GetSize( f32& sX, f32& sY, f32& sZ)
	{
		sZ = Scene_size_x;
		sY = Scene_size_y;
		sZ = block_size;
	}

	void GetCellStartPos( int32 nIdx, f32& nStartX, f32& nStartY)
	{
		int32 nIdxY = int32(idx/cell_count_x);
		int32 nIdxX = nIdx%cell_count_x;

		startX = nIdxX*block_size;
		startY = nIdxY*block_size;
	}

	void GetCellSize( f32& size, f32& size_z)
	{
		size = block_size;
		size_z = 0;
	}

	void TickSpace( int32 nDeltaTime );
	SpaceGridMap m_mapDirtyGrid;
protected:
	SpaceGrid** m_CellList;
	
	f32 Scene_size_x;
	f32 Scene_size_y;

	f32 block_size;

	int32 cell_count_x;
	int32 cell_count_y;
	int32 cell_count;
};

