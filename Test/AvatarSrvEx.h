#pragma once

#include "AvatarSrv.h"
#include "CommonDataOwner.h"

class Scene;

class AvatarSrvEx : public AvatarSrv, public CommonDataOwner
{
public:
	AvatarSrvEx(void);
	virtual ~AvatarSrvEx(void);

	virtual Scene* GetScene();
	virtual void SetScene( Scene* pScene);
	
	const char* GetName();
	const char* GetAccountName();
protected:
	int32	m_sceneID;
};

