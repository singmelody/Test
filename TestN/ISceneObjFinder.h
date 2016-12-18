#pragma once
class ISceneObjFinder
{
public:
	ISceneObjFinder(void);
	virtual ~ISceneObjFinder(void);

	virtual NodeAvatar* GetSceneAvatar(int32 nAvatarID) = 0;
	virtual NodeAvatar* GetScenePlayer(int32 nAvatarID) = 0;
	virtual NodeAvatar* GetSceneNpc(int32 nAvatarID) = 0;

	virtual MyBullet* GetSceneBullet(int32 nID) = 0;
};

