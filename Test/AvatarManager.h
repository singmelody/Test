#pragma once

#include <hash_map>

class AvatarSrv;

class AvatarManager : public stdext::hash_map< int32, AvatarSrv*>
{
public:
	AvatarManager(void);
	virtual ~AvatarManager(void);


	bool AddPlayerAvatar(AvatarSrv* pAvatar);
	bool AddPlayerAvatar( int32 nAvatarID, AvatarSrv* pAvatar);
	bool RemovePlayerAvatar(int32 nAvatarID);

	virtual AvatarSrv* GetAvatar(int32 nAvatarID);
	void SetSrvGroupID(int32 nGroupID);

protected:
	int32	m_nSrvGroupID;
	int32	m_nIDCounter;
	int32	GenNewID();
};

