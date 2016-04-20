#pragma once
#include "WorldState.h"

class ParamPool;

class WorldState_DataLoading : public WorldState, public Singleton<WorldState_DataLoading>
{
public:
	WorldState_DataLoading(void);
	virtual ~WorldState_DataLoading(void);

	virtual int32 GetState() { return eWS_DataLoading; }

	virtual void OnEnterState(WorldAvatar* pAvatar);
	virtual void OnLeaveState(WorldAvatar* pAvatar);
	virtual void DestroyAvatar( WorldAvatar* pAvatar);
	virtual void Tick( int32 nFrameTime );

protected:
	void RequestAvatarDataFromDBA( WorldAvatar* pAvatar);

	virtual void RegPeerPktHandle( PacketProcessor* pProcessor);
	void PktDBA_SelectUserData( class PacketUserSelectData* pPkt);
	void TryLoadTeam(WorldAvatar* pAvatar);
	void TryLoadGuid( WorldAvatar* pAvatar);

	void PktDBA_CommonDataInit(class PacketCommonDataInit* pPkt);
	void PktDBA_CommonDataReqFinish(class PacketCommonDataReqFinish* pPkt);
	void PktDBA_CreateAvatarRes(class PacketGateCreateAvatarRst* pPkt);

	void PktDBA_ManufactureDataInit( class PacketManufactureDataInit* pPkt);
	void PktDBA_OfflineDataInit( class PacketOfflineDataInit* pPkt);
	void PktDBA_OfflineDataFin( class PacketOfflineDataFin* pPkt);

	bool CheckAvatarSceneInfo( WorldAvatar* pAvatar);
	void CreateGateAvatar( WorldAvatar& avatar, ParamPool& pool);
};

