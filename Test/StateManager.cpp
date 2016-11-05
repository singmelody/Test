#include "StdAfx.h"
#include "StateManager.h"
#include "MyRandom.h"


Avatar_State_Update_Table::Avatar_State_Update_Table()
{
	memset( m_Table, 0, sizeof(int32)*eGAS_COUNT);


}


bool StateManager::GetData(int32 nIdx, uint32& nData)
{
	if( nIdx < 0 || nIdx >= BIT_LIST_SIZE )
		return false;

	nData = m_BitList[nIdx];
	return true;
}

void StateManager::RandomState()
{
	for(int32 i = 0; i < BIT_LIST_SIZE; ++i)
	{
		m_BitList[i] = (uint32)MyRandom::Instance().RandInt( 0, 4294967294);
	}
}
