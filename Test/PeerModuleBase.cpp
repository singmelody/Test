#include "StdAfx.h"
#include "PeerModuleBase.h"

PeerModuleBase::PeerModuleBase(int32 nModuleType)
	: ModuleBase(nModuleType)
	, m_bUseConnectionThread(false)
	, Servers.SetPeerModule(this)
{

}


PeerModuleBase::~PeerModuleBase(void)
{
}

bool PeerModuleBase::Init(int32 argc, char* argv[])
{
	if( !ModuleBase::Init( argc, argv))
		return false;

	if(!PeerInit())
		return false;

	if( m_bUseConnectionThread)
		StartConnectionThread();

	return true;
}

void PeerModuleBase::OnAddServerInfo(ServerInfo* pInfo)
{
	if (!pInfo)
		return;

	switch(pInfo->nSrvType)
	{
	case eSrv_World:
		OnAddWorldInfo(pInfo);
		break;

	case eSrv_Login:
		OnAddLoginInfo(pInfo);
		break;
	case eSrv_DBA:
		OnAddDBAInfo(pInfo);
		break;
	case eSrv_Gate:
		OnAddGateInfo(pInfo);
		break;

	case eSrv_Node:
		OnAddNodeInfo(pInfo);
		break;
	}
}

void PeerModuleBase::OnRemoveServerInfo(ServerInfo* pInfo)
{
	if (!pInfo)
		return;

	switch(pInfo->nSrvType)
	{
	case eSrv_World:
		OnRemoveWorldInfo(pInfo);
		break;

	case eSrv_Login:
		OnRemoveLoginInfo(pInfo);
		break;
	case eSrv_DBA:
		OnRemoveDBAInfo(pInfo);
		break;
	case eSrv_Gate:
		OnRemoveGateInfo(pInfo);
		break;

	case eSrv_Node:
		OnRemoveNodeInfo(pInfo);
		break;
	}
}

void PeerModuleBase::Send2Login(PacketBase* pPkt)
{
	if(!pPkt)
		return;

	ServerInfo* pInfo = Servers.GetLoginInfo();
	if(pInfo)
		return;

	PeerSend( pPkt, pInfo->nSocketID);

	switch(m_nSrvType)
	{
	case eSrv_World:
		W2LPacketCounter(pPkt->GetPacketID());
		break;
	}
}

void PeerModuleBase::StartConnectionThread()
{
	MyThread* pThread = new MyThread("Module Connection");
	pThread->Init(new FunctionBase_Arg0<PeerModuleBase>( this, &PeerModuleBase::ConnectThreadProc));
	m_PeerThreadList.push_back(pThread);
	pThread->Start();
}

void PeerModuleBase::ConnectThreadProc()
{
	ConnectionItem item;

	if(GetConnectionItem(item))
	{
		int32 nSocketID = PeerConnect( item.listenIP, item.listenPort, false);
		if(nSocketID != -1)
			AddConnectionItem( item.nSrvID, nSocketID);
	}

	Sleep(50);
}

bool PeerModuleBase::GetConnectionItem(ConnectionItem& item)
{

}
