#pragma once

#include <string>
#include "Singleton.h"
#include "WorldBase.h"

class WorldServer : public WorldBase, public Singleton<WorldServer>
{
	friend class WarCltManager;
	friend class WarSrvManager;
	friend class WorldSystemManager;
public:
	WorldServer();
	virtual ~WorldServer();

	virtual bool Init(int32 nArgc, char* argv[]);

	void LoginDBAServer();
	bool Start();

	virtual bool ClusterCheck();



	virtual void ProcessLogic( int32 nFrameTime);

	virtual void OnConfigLoaded();
	virtual void AppendLoadTemplate();

	virtual void OnGateDisconnect(ServerInfo* pInfo);
	virtual void OnNodeDisconnect(ServerInfo* pInfo);

	void OnRecvSrvConnectPkt( class PacketSrvConnect* pPkt);
	virtual void OnAllocateSrvID( int32 nSrvType, int32 nSrvID, int32 nSocketID);

	void TickOutAvatar(WorldAvatar* pAvatar);

	inline bool IsSrvShutingdown() const { return m_shutdownStage != eSDS_None; }

	static bool bUseBilling;
protected:
	enum ShutDownStage
	{
		eSDS_None,
		eSDS_WaitingLoginForbidden,
		eSDS_KickingAllPlayers,
		eSDS_WritingWorldData,
		eSDS_WaitingOtherSrvShutDown,
	};

	ShutDownStage	m_shutdownStage;
	bool			m_bBillingRested;

	int32			m_nNodeCount;
	int32			m_nCollisionCount;
};