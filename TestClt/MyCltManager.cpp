#include "StdAfx.h"
#include "MyCltManager.h"
#include "Thread.h"
#include "PacketImpl.h"

MyCltManager::MyCltManager(void)
{
}


MyCltManager::~MyCltManager(void)
{
}

void MyCltManager::Connect2LoginSrv(const char* sAddr, int32 nPort)
{
	pClt->Logout();

	pClt->m_pManager = this;

	int32 nSocketID = m_pNetManager->Connect( (char*)sAddr, nPort);
	if( nSocketID < 0 )
		return false;

	pClt->m_nSocketID = nSocketID;

	PacketConnectLoginServer pkt;
	memset( pkt.m_macAddress, 0, pkt.MAC_ADDRESS_LEN);

	pkt.m_nCpuTypeStrLen = pkt.m_nGpuTypeStrLen = 0;
	pkt.m_nPhyMemory = pkt.m_nVideioMemory = 0;
	pkt.uiversionlen = 0;
	pkt.m_nPhyProcessor = pkt.m_nLogicalProcessor = pkt.m_nAvailProcessor = 0;
	pkt.m_nWinVersion = 0;
	pkt.m_bIs64bIT = 0;

	strcpy_s( pkt.m_GameVersion, m_strFullVersion.c_str());
	pClt->SendPacket(&pkt);
	pClt->AddTicks("SendLogin");

	m_pCltsLoginPending[nSocketID] = pClt;
}

void MyCltManager::SendCltSelectAvatar(int32 nIdx)
{
	PacketCltSelectAvatar selPkt;

	selPkt.SetAvatarID(m_nAvatarID);
	selPkt.nAvatarIdx = nIdx;
	selPkt.SetPacketType(ePacketType_GateProc);

	SendPacket(&selPkt);

	AddTicks("SendCltSelectAvatar");
}

void MyCltManager::SendCltCreateCharacter(const std::string& strRoleName)
{

}

void MyCltManager::SendCltCreateDefaultCharacter()
{

}

void MyCltManager::SendCltDelCharacter(int32 nIdx)
{

}

void MyCltManager::InitNetManager()
{
	CltPacketProcessor* pProc = CltPacketProcessor::GetSingletonPtr();

	FunctionBase_Arg1<int32>* pCFun = new FunctionBase_Arg1<int32>( this, &);
	FunctionBase_Arg1<int32>* pDFun = new FunctionBase_Arg1<int32>( this, &);


	NetManager* pManager = new NetManagerReactor( true, 819200, 131072, 819200, 131072, NULL, pCFun, pDFun);
	pManager->SetPacketProcessor(pProc);

#if PACKET_USE_INDEX_DATA
	pManager->UseIndexWhenSend(true);
	pManager->UseIndexWhenRecv(false);
#endif

	m_pNetManager = pManager;

	CltActorPktHandleManager::Instance().RegisterPktHandlers(pProc);
	CltLogicPktHandleManager::Instance().RegisterPktHandlers(pProc);

	m_pThread = new MyThread("MyCltManager::ThreadRun");
	m_pThread->Init( new FunctionBase_Arg0<MyCltManager>(this, &MyCltManager::ThreadRun))
}

void MyCltManager::Tick(bool bSleep)
{
	MyPacketProcessor::GetSingleton().DoProc();

	Timer::Instance().Tick();

	int32 nFrameTime = Timer::Instance().FrameTime();
	ProcAutoLogin(nFrameTime);

	for (auto itr = m_Clts.begin(); itr != m_Clts.end(); ++itr)
	{
		itr->second->Tick(nFrameTime);
	}

	if(bSleep)
		GameUtil::Sleep(10);
}

void MyCltManager::ThreadRun()
{
	while (1)
	{
		if(m_pNetManager)
			m_pNetManager->NetRun();

		Sleep(1);
	}
}

void MyCltManager::SendPacket(int32 nSocketID, PacketBase* pPkt)
{
	if(!pPkt)
		return;

	if(m_pNetManager)
		m_pNetManager->SendPacket( *pPkt, nSocketID);
}

int32 MyCltManager::GetTicks(const std::string& key)
{
	MapTicks::iterator itr = m_mapTicks.find(key);
	if( itr == m_mapTicks.end() )
		return 0;
	return itr->second;
}

int32 MyCltManager::SetTicks(const std::string& key)
{
	int32 nTicks = GetTickCount();
	m_mapTicks[key] = nTicks;
	return nTicks;
}
