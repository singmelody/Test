#include "StdAfx.h"
#include "PacketParamPool.h"
#include "ParamDef.h"
#include "PacketSender.h"
#include "ParamPool.h"
#include "ParamTypeDef.h"

PacketParamPool::PacketParamPool(void)
{
}


PacketParamPool::~PacketParamPool(void)
{
}

void PacketParamPool::SyncParamPool(SyncPacketArg& arg, ParamPool* pPool, uint32 paramFlag, uint32 syncFlag, uint32 nParamFlagExclude)
{
	if(!pPool)
		return;

	char pBuffer[PACKET_EX_BUFF_SIZE] = {0};
	m_ParamType = pPool->GetParamTypeID();

	bool bAll = (syncFlag & eParam_Sync_All) != 0;
	bool bClearDirty = (syncFlag & eParam_Sync_ClearDirty) != 0;
	syncFlag &= ~eParam_Sync_ClearDirty;

	int32 nStartPos = 0;
	while (true)
	{
		m_ParamFlag = 0;

		if(nStartPos == 0)
		{
			m_ParamFlag |= ePacketFirst;	// first packet
			if(bAll)
				m_ParamFlag |= ePacketFull;	// all packet
		}

		int32 nWriteSize = PACKET_EX_BUFF_SIZE;

		bool bFinish = pPool->Write( pBuffer, nWriteSize, nStartPos, paramFlag, syncFlag, nParamFlagExclude);
		if( 0 == nWriteSize )
			break;

		if(bFinish)
			m_ParamFlag |= ePacketLast;

		WriteBuffer( pBuffer, nWriteSize);
		arg.SendPacket(this);

		if(bFinish)
			break;
	}

	if(bClearDirty)
		pPool->ClearDirty();
}
