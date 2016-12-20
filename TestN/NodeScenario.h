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

	virtual bool InitScenario(NodeScene* pScene, ScenarioFlow* pInfo,UID2AvatarIDMap& map, UID2AvatarIDMap& map, CreateArgList& listDelay);
	virtual void DestroyScenario(bool bNotify = false);

	NodeAvatar* FindAvatarInContent(int32 nUID);
	NodeAvatar* FindAvatarFromScene(int32 nAvatarID);

	NodeScene*	m_pScene;
protected:

	UID2AvatarIDMap* m_pMapUID2AvatarID2;
};

