#include "StdAfx.h"
#include "MyThreadGroup.h"


MyThreadGroup::MyThreadGroup(void)
{
	m_Threads.clear();
}


MyThreadGroup::~MyThreadGroup(void)
{
	StopThreads();
}

void MyThreadGroup::StartThreads(uint32 nNumOfThread)
{
	if(!m_Threads.empty())
		return;

	for (uint32 nIdx = 0; nIdx < nNumOfThread; ++nIdx)
	{
		Thread* pThread = CreateThread();
		if(pThread)
		{
			m_Threads.push_back(pThread);
			pThread->Start();
		}
	}
}

void MyThreadGroup::StopThreads()
{
	for (auto itr = m_Threads.begin(); itr != m_Threads.end(); ++itr)
	{
		(*itr)->Stop();
		(*itr)->Wait();
		DestroyThread(*itr);
	}

	m_Threads.clear();
}

uint32 MyThreadGroup::GetThreadNum() const
{
	return (uint32)m_Threads.size();
}

Thread* MyThreadGroup::GetMinLoadThread()
{
	if( 0 == m_Threads.size())
		return NULL;

	size_t nThreadIdx = 0;
	uint32 nThreadLoad = uint32(-1);

	for (size_t nIdx = 0; nIdx < (int32)m_Threads.size(); ++nIdx)
	{
		if(m_Threads[nIdx]->GetLoad() < nThreadLoad )
		{
			nThreadLoad = m_Threads[nIdx]->GetLoad();
			nThreadIdx = nIdx;
		}
	}

	std::swap( m_Threads[nThreadIdx], m_Threads[m_Threads.size() -1]);

	return m_Threads[m_Threads.size() - 1];
}
