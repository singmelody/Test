#pragma once

#include <string>
#include "PeerModuleBase.h"

class ParamPool;
class ServerInfo;

#define WORLDDOG_SET_VALUE( paramName, Val) DOG_SET_VALUE( m_pWorldDogPool, paramName, Val)
#define WORLDDOG_SET_STRING( paramName, Val) DOG_SET_STRING( m_pWorldDogPool, paramName, Val)

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
	std::string m_strDBAIP;
	int32 m_nDBAPort;
	int32 m_olAvatarCount;
};

