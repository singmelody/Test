#include "StdAfx.h"
#include "SceneContentItem.h"


SceneContentItem::SceneContentItem(void)
{
}


SceneContentItem::~SceneContentItem(void)
{
}

bool SceneContentItem::LoadContentItem(DBRow& row)
{
	return true;
}

bool SceneContentItem::InitContentItem(NodeScene* pScene, UID2AvatarIDMap& map, CreateArgList& listDelay)
{
	return true;
}
