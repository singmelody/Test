#include "StdAfx.h"
#include "MyNetThread.h"
#include "SelectReactor.h"
#include "MyLog.h"
#include "NetEventHandler.h"

MyNetThread::MyNetThread(uint32 nMaxNumOfHandle)
{
	m_pReactor = new SelectReactor();

	m_nMaxNumOfHandle = nMaxNumOfHandle;
	m_nNumOfHandle = 0;
}

MyNetThread::~MyNetThread(void)
{
	for (auto itr = m_waitList.begin(); itr != m_waitList.end(); ++itr)
		SAFE_DELETE(itr->first);

	m_waitList.clear();
	SAFE_DELETE(m_pReactor);
}

void MyNetThread::RegisterHandler(NetEventHandler* pHandler, uint32 nEvent)
{
	if(!pHandler)
		return;

	m_mutex.Lock();
	m_waitList.push_back(std::make_pair( pHandler, nEvent));
	m_mutex.Unlock();
}

bool MyNetThread::OnInit()
{
	if(!m_pReactor->Open( m_nMaxNumOfHandle, 1000))
	{
		MyLog::message("NetThread::OnInit open reactor failed!");
		return false;
	}

	m_nNumOfHandle = 0;
	return true;
}

void MyNetThread::OnQuit()
{
	m_mutex.Lock();

	for (HANDLER_VEC::iterator itr = m_waitList.begin(); itr != m_waitList.end(); ++itr)
	{
		itr->first->HandleClose();
	}

	m_waitList.clear();
	m_mutex.Unlock();
	m_pReactor->Close();
}

bool MyNetThread::Process(const Time& time)
{
	HANDLER_VEC newHandlers;

	m_mutex.Lock();
	newHandlers.assign( m_waitList.begin(), m_waitList.end());
	m_waitList.clear();
	m_mutex.Unlock();


	for (HANDLER_VEC::iterator itr = newHandlers.begin(); itr != newHandlers.end(); ++itr)
	{
		if(!m_pReactor->RegisterHandler( itr->first, itr->second))
		{
			MyLog::message("MyNetThread::Process RegisterHandler failed!");
			itr->first->HandleClose();
		}
	}

	newHandlers.clear();
	m_nNumOfHandle = m_pReactor->GetNumOfHandler();
	m_pReactor->HandleEvents( time, 15);

	return true;
}
