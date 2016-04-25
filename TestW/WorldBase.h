#pragma once

#include <string>

class ParamPool;
class ServerInfo;

class WorldBase : public PeerModuleBase
{
public:
	WorldBase(void);
	virtual ~WorldBase(void);

	virtual bool Init(int32 nArgc, char* argv[]);
	virtual void ProcessConsole(int32 nFrameTime);

	bool	m_bWarWorld;

	ParamPool*  m_pWorldDogPool;

	virtual bool CreateDogPool();
	virtual void OnConfigLoaded();
	virtual void InitDogParamPool( ServerInfo* pInfo);
	virtual void UpdateDogPool(int32 nFrameTime);
	virtual void BroadcastDogPool();

	virtual void OnAddServerInfo(ServerInfo* pInfo);
	virtual void OnRemoveServerInfo( ServerInfo* pInfo);

	void OnServerInfoChanged( ServerInfo* pInfo);
protected:
	std::string strDBAIP;
	int32 m_nDBAPort;
	int32 m_olAvatarCount;
};

