#pragma once

#include "Singleton.h"
#include <map>

class CreateWorldAvatarArg;
class WorldAvatar;

class WorldAvatarManager : public Singleton<WorldAvatarManager>
{
public:
	WorldAvatarManager(void);
	~WorldAvatarManager(void);

	WorldAvatar* CreateWorldAvatar(CreateWorldAvatarArg& arg);	
protected:
	int32 m_ConnectCount;
	std::map< std::string, int32>	m_titleMap;		// title - avatarid
	std::map< std::string, int32>	m_accountMap;	// accountname - avatarid
	std::map< int64, int32>			m_avatarDIDMap;	// avatardid - avatarid
};

