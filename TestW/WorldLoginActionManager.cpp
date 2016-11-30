#include "StdAfx.h"
#include "WorldLoginActionManager.h"
#include "PacketImpl.h"
#include "WorldAvatar.h"

WorldLoginActionManager::WorldLoginActionManager(void)
{
}


WorldLoginActionManager::~WorldLoginActionManager(void)
{
}

void WorldLoginActionManager::Save2DB()
{
	std::map< int64, std::vector<LoginActionData> >::iterator itr = m_LoginActionList.begin();

	for (; itr != m_LoginActionList.end(); ++itr)
	{
		std::vector<LoginActionData>::iterator itr_info = itr->second.begin();
		for (; itr_info != itr->second.end(); ++itr_info)
		{
			PacketLoginActionInfo pkt;
			pkt.nFlag = 2;
			pkt.nTypeID = itr_info->m_nType;
			pkt.nData1 = itr_info->m_nData1;
			pkt.nData2 = itr_info->m_nData2;
			pkt.nData3 = itr_info->m_nData3;
			pkt.nCreateTime = itr_info->m_nCreateTime;
			pkt.nAvatarDID = itr->first;

			Send2DBA(&pkt);
		}
	}
}

void WorldLoginActionManager::SendAction2Node(WorldAvatar* pAvatar)
{
	std::map< int64, std::vector<LoginActionData> >::iterator itr = m_LoginActionList.begin();

	if ( itr == m_LoginActionList.end())
		return;

	PacketLoginActionInfo pkt;
	std::vector<LoginActionData>::iterator itr_info = itr->second.begin();
	for (; itr_info != itr->second.end(); ++itr_info)
	{
		PacketLoginActionInfo pkt;
		pkt.nFlag = 2;
		pkt.nTypeID = itr_info->m_nType;
		pkt.nData1 = itr_info->m_nData1;
		pkt.nData2 = itr_info->m_nData2;
		pkt.nData3 = itr_info->m_nData3;
		pkt.nCreateTime = itr_info->m_nCreateTime;
		pkt.nAvatarDID = itr->first;

		pAvatar->Send2Node(&pkt, pAvatar->GetNodeSrvID());
	}
}
