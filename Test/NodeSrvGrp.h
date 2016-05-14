#pragma once

#include <vector>
#include "ServerGrp.h"
#include "ServerGrpInfo.h"

class Scene;

class NodeSrvInfo : public ServerInfo
{
public:
	NodeSrvInfo(int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& addr);
	~NodeSrvInfo(void);

	virtual void UpdateDetailsPool();

	void AddupSceneLoadValue( Scene* pScene, bool bAdd);

	int32 nTotalLoadValue;
	int32 nTrunkLoadValue;
};

typedef std::vector<NodeSrvInfo*> NodeSrvList;

class NodeSrvGrp : public ServerGrp
{
public:
	NodeSrvGrp() {}
	~NodeSrvGrp(){}

	virtual ServerInfo* CreateSrvInfo( int32 nSrvType, int32 nSrvID, int32 nSocketID, SockAddr& laddr);
	virtual void SortList();

	int32 GetLowSceneLoadNode( int32 exclude_ID = SERVERID_NULL);

	virtual void SortList();
protected:
	NodeSrvList m_listByTrunkValue;
	NodeSrvList m_listByLoadValue;

	class TrunkCmp
	{
	public:
		bool operator()(NodeSrvInfo* pLeft, NodeSrvInfo* pRight) const
		{
			return pLeft->nTrunkLoadValue < pRight->nTrunkLoadValue;
		}
	};

	class  LoadValueCmp
	{
	public:
		bool operator()(NodeSrvInfo* pLeft, NodeSrvInfo* pRight) const
		{
			return pLeft->nTrunkLoadValue < pRight->nTrunkLoadValue;
		}
	};
};