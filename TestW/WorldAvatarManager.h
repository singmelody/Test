#pragma once

#include "Singleton.h"
#include <map>
#include "BaseType.h"
#include "AvatarManager.h"
#include "WorldFuncManager.h"

class CreateWorldAvatarArg;
class WorldAvatar;
class WorldSceneManager;

class WorldAvatarManager : public AvatarManager, public WorldFuncManager, public Singleton<WorldAvatarManager>
{
public:
	WorldAvatarManager(void);
	~WorldAvatarManager(void);

	WorldAvatar* CreateWorldAvatar(CreateWorldAvatarArg& arg);	
	void RemoveWorldAvatar( WorldAvatar* pAvatar);

	int32 CheckAccountOnline( const char* pAccout, int32& nAvatarID);

	WorldSceneManager& SceneMgr;
protected:
	int32 m_ConnectCount;
	std::map< std::string, int32>	m_titleMap;		// title - avatarid
	std::map< std::string, int32>	m_accountMap;	// accountname - avatarid
	std::map< int64, int32>			m_avatarDIDMap;	// avatardid - avatarid
};

