#pragma once
#include "CasterTrunkContainer.h"

class WorldGroup
{
public:
	WorldGroup(void);
	virtual ~WorldGroup(void);

	// ��GateSrvID�����б�
	CasterTrunkGate m_GateTrunk;
	void UpdateGateTrunk( WorldAvatar* pAvatar, bool bOnline);	// �����Ա�����,ɾ����������

	// ��NodeSrvID�����б�
	CasterTrunkNode m_NodeTrunk;
	void UpdateNodeTrunk( WorldAvatar* pAvatar, bool bOnline);	// �����Ա�����,ɾ����������

	void UpdateAllTrunk( WorldAvatar* pAvatar, bool bOnline)
	{
		UpdateGateTrunk( pAvatar, bOnline);
		UpdateNodeTrunk( pAvatar, bOnline);
	}
};

