#include "StdAfx.h"
#include "NodeBase.h"


NodeBase::NodeBase(void)
	: PeerModuleBase(eSrv_Node)
	,m_pNodeDogPool(NULL)
{
}


NodeBase::~NodeBase(void)
{
}

void NodeBase::OnAddServerInfo(ServerInfo* pInfo)
{
	PeerModuleBase::OnAddServerInfo(pInfo);

	OnServerInfoChanged(pInfo);
}

void NodeBase::OnRemoveServerInfo(ServerInfo* pInfo)
{
	PeerModuleBase::OnRemoveServerInfo(pInfo);
	OnServerInfoChanged(pInfo);
}

void NodeBase::OnServerInfoChanged(ServerInfo* pInfo)
{
	switch(pInfo->nSrvType)
	{
	case eSrv_World:
		NODEDOG_SET_VALUE( WorldID, pInfo->nSrvID);
		break;
	case eSrv_Gate:
		NODEDOG_SET_VALUE( GateCnt, Servers.m_GateGrp.ServerCnt());
		break;
// 	case eSrv_Collision:
// 		break;
	}
}
