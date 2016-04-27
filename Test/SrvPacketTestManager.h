#pragma once
class SrvPacketTestManager
{
public:
	SrvPacketTestManager(void);
	~SrvPacketTestManager(void);

	virtual void TestSend2Clt(class PacketBase* pkt);
	virtual void TestSend2Peer( class PacketBase* pkt);

	void StartPeerRandPacket() { m_bPeerSend = !m_bPeerSend; }

protected:
	void TickSendTestPacket(int32 nFrameTime);
	void DoSend();

	bool	m_bPeerSend;
	bool	m_bCltSend;
	int32	m_InvTime;
	int32	m_CurTime;
};

