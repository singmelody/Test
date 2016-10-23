#pragma once
#include "CasterTrunkContainer.h"

class WorldGroup
{
public:
	WorldGroup(void);
	virtual ~WorldGroup(void);

	// 按GateSrvID分组列表
	CasterTrunkGate m_GateTrunk;
	void UpdateGateTrunk( WorldAvatar* pAvatar, bool bOnline);	// 队伍成员的添加,删除和上下线

	// 按NodeSrvID分组列表
	CasterTrunkNode m_NodeTrunk;
	void UpdateNodeTrunk( WorldAvatar* pAvatar, bool bOnline);	// 队伍成员的添加,删除和上下线

	void UpdateAllTrunk( WorldAvatar* pAvatar, bool bOnline)
	{
		UpdateGateTrunk( pAvatar, bOnline);
		UpdateNodeTrunk( pAvatar, bOnline);
	}
};

