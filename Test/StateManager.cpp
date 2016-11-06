#include "StdAfx.h"
#include "StateManager.h"
#include ""


void StateManager::RandomState()
{
	for(int32 i = 0; i < BIT_LIST_SIZE; ++i)
	{
		m_BitList[i] = (uint32)Random::Instance().RandInt( 0, 4294967294);
	}
}
