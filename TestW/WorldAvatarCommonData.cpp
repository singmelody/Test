#include "StdAfx.h"
#include "WorldAvatarCommonData.h"
#include "PacketImpl.h"
#include "WorldAvatar.h"
#include "CommonDataObj.h"
#include "ParamTypeDef.h"

FINISH_FACTORY_ARG0(WorldAvatarCommonData);
WorldAvatarCommonData::WorldAvatarCommonData(void)
{
	m_pOwner = NULL;
}


WorldAvatarCommonData::~WorldAvatarCommonData(void)
{
}

void WorldAvatarCommonData::SetOwner(CommonDataOwner* pOwner)
{
	CommonDataManager::SetOwner(pOwner);
	m_pOwner = (WorldAvatar*)pOwner;
}

void WorldAvatarCommonData::InitData2Node(int32 nSrvID)
{
	// send item packet

	// combine packet handle, avoid send too many small packet
	PacketPack pack;
	pack.SetNodeSyncArg( m_pOwner, m_pOwner->GetNodeSrvID());

	int32 nAvatarID = m_pOwner->GetAvatarID();

	for (int32 i = 0; i < eCommonData_Max; ++i)
	{
		CommonDataCont* pCont = m_pCont[i];
		if(!pCont)
			continue;

		for (CommonDataObject* pObj = pCont->GetNext(NULL); pObj != NULL; pObj = pCont->GetNext(pObj))
		{
			ParamPool* pItemPool = pObj->GetParamPool();
			assert(pItemPool);

			if( i == eCommonData_Item && IsStorageItem(*pItemPool))
				continue;

			PacketCommonDataInit pkt;
			pkt.SetAvatarID( nAvatarID );
			pObj->InitPacket( pkt, m_pOwner);
			pkt.SyncParamPool( &pack, pItemPool, eParam_Flag_Server, eParam_Sync_All);
		}
	}

	pack.SendPacket();
}
