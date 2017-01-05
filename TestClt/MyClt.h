#pragma once
class MyClt : public IReferable, public PacketSender
{
public:
	MyClt(void);
	~MyClt(void);
};

