#include "StdAfx.h"
#include "PacketProcessor.h"
#include "MyLog.h"
#include "PacketBase.h"
#include "PacketImpl.h"

PacketProcessor::PacketProcessor()
{
	DefaultPacketFunction = NULL;

	m_RecvPacketCount = 0;
	m_RecvPacketSize = 0;
}

PacketProcessor::~PacketProcessor()
{
	for (FuncMap::iterator itr = m_functionMap.begin(); itr != m_functionMap.end(); ++itr)
		SAFE_DELETE(itr->second);

	m_functionMap.clear();

	SAFE_DELETE(DefaultPacketFunction);
}

bool PacketProcessor::OnPacketReceived(PacketBase* pPkt)
{
	return PushPacket( pPkt );
}

bool PacketProcessor::PushPacket( PacketBase* pPkt )
{
	if(!pPkt)
		return false;

	m_receivingPktList.Push_Back(pPkt);
	return true;
}

void PacketProcessor::RegPacketHandle(int32 nPacketID, FuncEntry* pFunc)
{
	if(!pFunc)
		return;

	FuncMap::iterator itr = m_functionMap.find(nPacketID);
	if( itr != m_functionMap.end() )
	{
		MyLog::error("RegPacket Handle Fail, Hanlder Existing");
		return;
	}

	m_functionMap[nPacketID] = pFunc;
}

void PacketProcessor::RegDefaultHandler(FuncEntry* pFunc)
{
	if(DefaultPacketFunction)
	{
		MyLog::error("RegPacket Default Handler Failed");
		return;
	}

	if(!pFunc)
		return;

	DefaultPacketFunction = pFunc;
}

void PacketProcessor::RegPacketHandler(int32 nPacket, FuncEntry* pFunc)
{
	if(!pFunc)
		return;

	FuncMap::iterator itr = m_functionMap.find(nPacket);
	if( itr != m_functionMap.end() )
	{
		MyLog::error("RegPacket Handler Failed");
		return;
	}

	m_functionMap[nPacket] = pFunc;
}


bool PacketProcessor::ProcessPacket(PacketBase* pPkt)
{
	if(!pPkt)
		return false;

	if( pPkt->GetPacketID() == PacketPack::GetClassStatic()->ClassID() )
	{
		PacketPack* pPack = (PacketPack*)pPkt;

		int32 nCount = pPack->GetPacketCnt();
		for (int32 i = 0; i < nCount; ++i)
		{
			ProcessPacket( pPack->GetPacket(i) );
		}

		return true;
	}

	m_RecvPacketCount++;

	FuncEntry* pFunc = FindFuncEntry( pPkt );
	if(!pFunc)
		return false;


	//FRAME_PROFILE_PACKET( pPkt );
	(*pFunc)(pPkt);

	return true;
}

void PacketProcessor::RemovePacketHandler(int32 nPacketID, char* sFuncName)
{
	FuncMap::iterator itr = m_functionMap.find(nPacketID);
	if(itr != m_functionMap.end())
		m_functionMap.erase(itr);
}

PacketProcessor::FuncEntry* PacketProcessor::FindFuncEntry(PacketBase* pPkt)
{
	FuncEntry* pFunc = NULL;

	if(!pPkt)
	{
		int32 nID = GetFuncEntryID(pPkt);

		FuncMap::iterator itr = m_functionMap.find(nID);
		if( itr != m_functionMap.end() )
			pFunc = itr->second;
	}

	if(!pFunc)
		pFunc = DefaultPacketFunction;

	return pFunc;
}

int32 PacketProcessor::GetFuncEntryID(PacketBase* pPkt)
{
	if(!pPkt)
		return NULL;

	return pPkt->GetPacketID();
}
