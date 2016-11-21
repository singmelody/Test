#pragma once

#include <string>
#include <map>
#include <hash_map>
#include "Singleton.h"
#include "WorldFuncExManager.h"

class WorldAvatar;

class AvatarOnLineManager : public WorldFuncExManager, public Singleton<AvatarOnLineManager>
{
public:
	AvatarOnLineManager(void);
	~AvatarOnLineManager(void);

	void OnConfigLoaded();

	void Tick(int32 nFrameTime);

	void ProcOnlineUserInfo(int32 nFrameTime);

	void SyncOnlineUserInfo2Login();

	void AddAccount(WorldAvatar* pAvatar);
	void DelAccount(WorldAvatar* pAvatar);
	bool IsAccountExist(const char* szAccount);

	void AddAvatar(WorldAvatar* pAvatar);
	void DelAvatar(WorldAvatar* pAvatar);

	WorldAvatar* GetAvatar(const char* pTitle);
	WorldAvatar* GetAvatar(const std::string& strTitle);

	int32 GetOnlineAvatarCount();

	WorldAvatar* GetAvatarByAccountName(const char* accountName);
	WorldAvatar* GetAvatarByDID(int64 nAvatarDID);
	WorldAvatar* GetAvatarByName(const char* name);

	int32 GetMaxOnlineUser() const { return m_nMaxOnlineUser; }
	void SetMaxOnlineUser(int32 v) { m_nMaxOnlineUser = v; SyncOnlineUserInfo2Login(); }
	inline bool EnableAccMaxOnlineUser() const { return config_AccMaxOnlineUser_Enable; }
	inline bool EnableAccMaxOnlineUser(bool v)  { config_AccMaxOnlineUser_Enable = v; }

protected:
	std::map<std::string ,int32>	m_avatarMap;
	std::map<std::string, int32>	m_accountMap;
	std::map<int64,	int32>			m_avatarDIDTable;

	typedef std::hash_map<std::string, int32> AvatarNameMap;
	AvatarNameMap					m_avatarNameTable;

	int32	m_nMaxOnlineUser;
	int32	m_nTimeAccMaxOnlineUser;

	int32	config_MaxOnlineUser;
	bool	config_AccMaxOnlineUser_Enable;	
	int32	config_AccMaxOnlineUser_InitValue;
	int32	config_AccMaxOnlineUser_DeltaTime;
	int32	config_AccMaxOnlineUser_Step;
};

