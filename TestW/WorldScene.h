#pragma once

#include "Scene.h"
#include <deque>

class WorldAvatar;

class WorldScene : public Scene
{
	DECLARE_FACTORY_ARG0( WorldScene, -1, new PoolAllocator)
public:
	WorldScene(void);
	virtual ~WorldScene(void);

	int64	m_nAvatarDID;
	int32   m_nContentID;

	virtual void EnterScene( WorldAvatar* pAvatar);
	virtual void ExitScene( WorldAvatar* pAvatar);

	void OnClose();
	virtual void BlockScene();

	void Set2CurNode( PacketBase* &pkt);

	virtual bool CheckEnterScene(WorldAvatar* pAvatar) const { return true; }	
	virtual void UpdateCopyOwner(WorldAvatar* pAvatar) {}

	virtual void OnCreateSucceed() {}

	std::deque<int32> m_WaitingQueue;
	void NotifyWaitingAvatars( int32 nErrorID);
	void AddWaitingAvatar( WorldAvatar* pAvatar);

	int32 GetPendingTime() { return m_nPendingTime; }
	void SetPendingTime(int32 nTime) 
	{
		m_nPendingTime = nTime;
	}

	int32 m_nPendingTime;
};

WorldScene* GetWorldScene(int32 nSceneID);