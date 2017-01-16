#pragma once

#include "MySmartPtr.h"
#include "PacketSender.h"

class MyClt : public IReferable, public PacketSender
{
public:
	MyClt(void);
	virtual ~MyClt(void);

	void Logout();
	void SendPacket(PacketBase* pPkt);

	void InitAvatarData(class PacketInitAvatarData* pPkt);
	void UpdateAvatarData(class PacketUpdateAvatarData* pPkt);

	GameScene* m_pScene;

	MyCltManager* m_pManager;
};

