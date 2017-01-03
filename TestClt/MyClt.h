#pragma once
class MyClt : public IReferable, public PacketSender
{
public:
	MyClt(void);
	virtual ~MyClt(void);

	void Logout();
	void SendPacket(PacketBase* pPkt);

	MyCltManager* m_pManager;
};

