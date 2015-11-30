#pragma once

#include "DBInterface.h"
#include "MyMath.h"

class NodeScene;

typedef std::map<int32,int32> UID2AvatarIDMap;
typedef std::vector<int32> CreateArgList;

class SceneContentItem
{
public:
	SceneContentItem(void);
	~SceneContentItem(void);

	virtual bool LoadContentItem(DBRow& row);
	virtual bool InitContentItem( NodeScene* pScene, UID2AvatarIDMap& map, CreateArgList& listDelay);


	int32 GetUID() { return (m_contentID & 0xffff << 16) + (m_npcType & 0xfff) << 12 + m_id & 0xffff; }

protected:
	int32 m_contentID;
	int32 m_npcType;
	int32 m_id;

	Point3	m_pos;
	Vector3 m_dir;
};

