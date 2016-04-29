#pragma once

class PacketProcessor;

class WorldStage_Logining 
{
public:
	WorldStage_Logining(void);
	~WorldStage_Logining(void);

	void RegPeerPktHandle( PacketProcessor* pProc);

protected:
	void PktDBA_UserData(class PacketUserData* pPkt);
	void PktDBA_UserDataReqFinish(class PacketUserDataRegFinish* pPkt);

};

