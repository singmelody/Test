#pragma once

#include "Singleton.h"
#include "AvatarDefine.h"

class Avatar_State_Update_Table : public Singleton<Avatar_State_Update_Table>
{
public:
	Avatar_State_Update_Table();
	~Avatar_State_Update_Table(){}

	int32 m_Table[eGAS_COUNT];
};

class StateManager
{
public:
	StateManager(void)
	{
		for (int32 i = 0; i < BIT_LIST_SIZE; ++i)
		{
			m_BitList[i] = 0;
			m_DirtyList[i] = 0;
		}
	}
	~StateManager(void){}

	void ClearAllBit()
	{
		for(int32 i = 0; i < BIT_LIST_SIZE;++i)
		{
			if (m_BitList[i] == 0)
				continue;

			m_DirtyList[i] = 0xffffffff;
			m_BitList[i] = 0;
		}
	}

	bool CheckBit(int32 nBit)
	{
		if( nBit < 0 || nBit >= MAX_BIT_SIZE )
			return false;

		int32 dIndex = nBit / BIT_SIZE;
		int32 bitIdx = nBit % BIT_SIZE;

		return (m_BitList[dIndex] & ( 0x1 << bitIdx )) ? true : false;
	}

	bool SetBit(int32 nBit)
	{
		if( nBit < 0 || nBit >= MAX_BIT_SIZE )
			return false;

		int32 dIndex = nBit / BIT_SIZE;
		int32 bitIdx = nBit % BIT_SIZE;

		m_BitList[dIndex] |= ( 0x1 << bitIdx );

		if(Avatar_State_Update_Table::Instance().m_Table[nBit] != 0)
			m_DirtyList[dIndex] |= ( 0x1 << bitIdx );

		return true;
	}

	bool ClearBit(int32 nBit)
	{
		if( nBit < 0 || nBit >= MAX_BIT_SIZE )
			return false;

		int32 dIndex = nBit / BIT_SIZE;
		int32 bitIdx = nBit % BIT_SIZE;

		m_BitList[dIndex] &= ~( 0x1 << bitIdx );

		if(Avatar_State_Update_Table::Instance().m_Table[nBit] != 0)
			m_DirtyList[dIndex] |= ( 0x1 << bitIdx );

		return true;
	}

	bool GetData( int32 nIdx, uint32& nData);

	void RandomState();
protected:
	uint32	m_BitList[BIT_LIST_SIZE];
	uint32  m_DirtyList[BIT_LIST_SIZE];
};

