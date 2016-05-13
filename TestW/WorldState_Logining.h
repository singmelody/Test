#pragma once

class PacketProcessor;

class WorldState_Logining : public WorldState, public Singleton<WorldState_Logined>
{
public:
	WorldState_Logining(void);
	~WorldState_Logining(void);

	void RegPeerPktHandle( PacketProcessor* pProc);

protected:
	void PktDBA_UserData(class PacketUserData* pPkt);
	void PktDBA_UserDataReqFinish(class PacketUserDataRegFinish* pPkt);

};

