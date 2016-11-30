#include "StdAfx.h"
#include "PacketParamPool.h"
#include "ParamDef.h"
#include "PacketSender.h"
#include "ParamPool.h"
#include "ParamTypeDef.h"
#include "ParamPoolOwner.h"
#include "PacketBase.h"
#include "PacketImpl.h"
#include "PacketSender.h"

PACKET_IMPL(PacketParamPoolBase);

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
	m_nParamType = pPool->GetParamTypeID();

	bool bAll = (syncFlag & eParam_Sync_All) != 0;
	bool bClearDirty = (syncFlag & eParam_Sync_ClearDirty) != 0;
	syncFlag &= ~eParam_Sync_ClearDirty;

	int32 nStartPos = 0;
	while (true)
	{
		m_nParamFlag = 0;

		if(nStartPos == 0)
		{
			m_nParamType |= ePacketFirst;	// first packet
			if(bAll)
				m_nParamType |= ePacketFull;	// all packet
		}

		int32 nWriteSize = PACKET_EX_BUFF_SIZE;

		bool bFinish = pPool->Write( pBuffer, nWriteSize, nStartPos, paramFlag, syncFlag, nParamFlagExclude);
		if( 0 == nWriteSize )
			break;

		if(bFinish)
			m_nParamType |= ePacketLast;

		WriteBuffer( pBuffer, nWriteSize);
		arg.SendPacket(this);

		if(bFinish)
			break;
	}

	if(bClearDirty)
		pPool->ClearDirty();
}


void PacketParamPool::SyncParamPool(PacketPack* pPack, ParamPool* pPool, uint32 paramFlag, uint32 syncFlag, uint32 nParamFlagExclude /*= 0*/)
{
	if(!pPool)
		return;

	char buffer[MAX_PACKET_BUFFER_SIZE] = {0};

	m_nParamType = pPool->GetParamTypeID();

	bool bAll = syncFlag & eParam_Sync_All;
	bool bClearDirty = syncFlag & eParam_Sync_ClearDirty;

	syncFlag &= ~eParam_Sync_ClearDirty;	// clear the dirty flag at once

	int32 nStartPos = 0;
	while (true)
	{
		m_nParamFlag = 0;

		if( nStartPos == 0 )
		{
			m_nParamFlag |= ePacketFirst;		// first packet
			if(bAll)
				m_nParamFlag |= ePacketFull;	// only packet
		}

		int32 nWriteSize = MAX_PACKET_BUFFER_SIZE;

		bool bFinish = pPool->Write( buffer, nWriteSize, nStartPos, paramFlag, syncFlag, nParamFlagExclude);
		if( 0 == nWriteSize )
			break;

		if(bFinish)
			m_nParamFlag |= ePacketLast; // last packet 

		WriteBuffer( buffer, nWriteSize);

		pPack->FillPacket(this);

		if(bFinish)
			break;
	}

	if(bClearDirty)
		pPool->ClearDirty();
}


void PacketParamPool::SyncParamPool(PacketSender* pPack, SENDPARAMFUNC1 pFunc, ParamPool* pPool, uint32 paramFlag, uint32 syncFlag, uint32 nParamFlagExclude /*= 0*/)
{
	SyncPacketArg1 arg;

	arg.m_pSender = pPack;
	arg.m_pFunc = pFunc;

	SyncParamPool( arg, pPool, paramFlag, syncFlag, nParamFlagExclude);
}

void PacketParamPool::SyncParamPool(PacketSender* pPack, SENDPARAMFUNC2 pFunc, int32 nParam, ParamPool* pPool, uint32 paramFlag, uint32 syncFlag, uint32 nParamFlagExclude /*= 0*/)
{
	SyncPacketArg2 arg;

	arg.m_pSender = pPack;
	arg.m_pFunc = pFunc;
	arg.m_nParam = nParam;

	SyncParamPool( arg, pPool, paramFlag, syncFlag, nParamFlagExclude);
}

void PacketParamPool::SyncParamPool(PacketSender* pPack, SENDPARAMFUNC3 pFunc, bool bParam, ParamPool* pPool, uint32 paramFlag, uint32 syncFlag, uint32 nParamFlagExclude /*= 0*/)
{
	SyncPacketArg3 arg;

	arg.m_pSender = pPack;
	arg.m_pFunc = pFunc;
	arg.m_bParam = bParam;

	SyncParamPool( arg, pPool, paramFlag, syncFlag, nParamFlagExclude);

}

void PacketParamPool::SyncParam2DBA(class PacketSender* pSender, ParamPool* pPool, uint32 nParamFlag, uint32 nSyncFlag, uint32 nParamFlagExclude /*= 0*/)
{
	SyncParamPool( pSender, &PacketSender::Send2DBA, pPool, nParamFlag, nSyncFlag, nParamFlagExclude);

}

void PacketParamPool::SyncParam2World(class PacketSender* pSender, ParamPool* pPool, uint32 nParamFlag, uint32 nSyncFlag, uint32 nParamFlagExclude /*= 0*/)
{
	Sync2WorldArg arg( pSender );
	SyncParamPool( arg, pPool, nParamFlag, nSyncFlag, nParamFlagExclude);

}

void PacketParamPool::SyncParam2Login(class PacketSender* pSender, ParamPool* pPool, uint32 nParamFlag, uint32 nSyncFlag, uint32 nParamFlagExclude /*= 0*/)
{
	SyncParamPool( pSender, &PacketSender::Send2Login, pPool, nParamFlag, nSyncFlag, nParamFlagExclude);
}

void PacketParamPool::SyncParam2Node(class PacketSender* pSender, int32 nSrvID, ParamPool* pPool, uint32 nParamFlag, uint32 nSyncFlag, uint32 nParamFlagExclude /*= 0*/)
{
	Sync2NodeArg arg( pSender, nSrvID);
	SyncParamPool( arg, pPool, nParamFlag, nSyncFlag, nParamFlagExclude);
}

void PacketParamPool::SyncParam2Gate(class PacketSender* pSender, bool bGateProc, ParamPool* pPool, uint32 nParamFlag, uint32 nSyncFlag, uint32 nParamFlagExclude)
{
	SyncParamPool( pSender, &PacketSender::Send2Gate, bGateProc, pPool, nParamFlag, nSyncFlag, nParamFlagExclude);
}

void PacketParamPool::SyncParam2Dog(class PacketSender* pSender, int32 nSrvID, ParamPool* pPool, uint32 nParamFlag, uint32 nSyncFlag, uint32 nParamFlagExclude /*= 0*/)
{
	SyncParamPool( pSender, &PacketSender::Send2Dog, nSrvID, pPool, nParamFlag, nSyncFlag, nParamFlagExclude);

}

bool PacketParamPool::UpdateParamPool(ParamPoolOwner* pPool, bool bDirty /*= false*/)
{
	if(!pPool)
		return false;

	return UpdateParamPool( pPool->GetParamPool(), bDirty);
}

bool PacketParamPool::UpdateParamPool(ParamPool* pPool, bool bDirty /*= false*/)
{
	if(!pPool)
		return false;

	char pBuffer[PACKET_EX_BUFF_SIZE];
	int32 nSize = ReadBuffer( pBuffer, PACKET_EX_BUFF_SIZE);
	if(nSize == 0)
		return false;

	pPool->Read( pBuffer, bDirty);
	return true;
}

