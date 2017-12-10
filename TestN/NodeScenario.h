#pragma once

#include "Scenario.h"
#include "NodeScene.h"
#include "SceneContentItem.h"

class NodeAvatar;

class NodeScenario : public Scenario
{
public:
	NodeScenario(void);
	virtual ~NodeScenario(void);

	virtual bool InitScenario(NodeScene* pScene, ScenarioFlow* pInfo,UID2AvatarIDMap& map, CreateArgList& listDelay);
	virtual void DestroyScenario(bool bNotify = false);

	NodeAvatar* FindAvatarInContent(int32 nUID);
	NodeAvatar* FindAvatarFromScene(int32 nAvatarID);

	void AddAvatarToScene( int32 nCount, int32 ids[]);
	void AddAvatarToScene( int32 nAvatraID, NodeAvatar* pAvatar);

	void RemoveNPCFromScene( int32 nCount, int32 ids[]);
	void RemoveNPCFromScene( int32 nAvatarID, NodeNPC* pNpc);

	void CombineSceneProcess();
	void CombineSceneInstanceProcess();

	virtual bool CheckStarted();
	virtual bool CheckInterval();

	virtual bool CheckSceneProcess( int32 iBitPos);
	virtual bool CheckSceneInstanceProcess( int32 iBitPos);

	virtual bool CheckSceneProcessStart();
	virtual bool CheckSceneInstanceProcessStart();

	NodeScene*	m_pScene;
protected:

	ScenarioFlow*		m_pScenarioFlow;
	UID2AvatarIDMap*	m_pMapUID2AvatarID2;
};

