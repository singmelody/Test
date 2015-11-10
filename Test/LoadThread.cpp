#include "StdAfx.h"
#include "LoadThread.h"
#include "LoadTemplateManager.h"
#include "Timer.h"
#include "MyLog.h"

LoadThread::LoadThread(LoadTemplateManager* pMgr, LoadBatch* pBatch, int32 nThreadIdx) 
	: m_pMgr(pMgr), m_pLoadBatch(pBatch), m_nThreadIdx(nThreadIdx)
{
}


LoadThread::~LoadThread(void)
{
}

void LoadThread::ProcessLoad()
{
	while(true)
	{
		int32 nWaitTemplateCnt = 0;

		// when the template dependence templates is not ready, block template self, cpu clip to other
		// when ready, then load the template self
		LoadTemplate* pTemplate = m_pLoadBatch->GetTemplate2Load( nWaitTemplateCnt );

		if(!pTemplate)
		{
			if(nWaitTemplateCnt > 1)
			{
				GSleep( 10 );
				continue;
			}
			break;
		}

		int32 nStartTick = getMSTime();

		if(pTemplate->ProcessLoad())
		{
			pTemplate->m_nLoadThreadIdx = m_nThreadIdx;
			pTemplate->SetLoadState(eLS_Loaded);
		}

		pTemplate->m_LoadCastTicks = getMSTime() - nStartTick;
	}
}

void LoadThread::Run()
{
	ProcessLoad();
}

LoadInfo::LoadInfo(LoadTemplateManager* pMgr, DBRow &row) : m_pMgr(pMgr)
{
	static int32 nCol_TemplateName = row.GetColumnIdx("TemplateName");
	static int32 nCol_BatachID = row.GetColumnIdx("BatchID");
	static int32 nCol_OrderID = row.GetColumnIdx("OrderID");

	row.Fill( m_strTemplate, nCol_TemplateName, "");
	row.Fill( m_batchID, nCol_BatachID, 0);
	row.Fill( m_orderID, nCol_OrderID, 0);

	m_pTemplate = m_pMgr->GetTemplate( m_strTemplate.c_str());
	if ( m_pTemplate != NULL )
		m_pTemplate->m_pLoadInfo = this;

	m_pDependList = NULL;
}

LoadInfo::~LoadInfo()
{
 	SAFE_DELETE(m_pDependList);
 	
	if(!m_pTemplate)
	{
		m_pTemplate->m_pLoadInfo = NULL;
		m_pTemplate = NULL;
	}
}

void LoadInfo::AddDependence(LoadTemplate* pTemp)
{
	if(!m_pDependList)
		m_pDependList = new LoadTemplateList();

	m_pDependList->push_back(pTemp);
}

bool LoadInfo::CheckLoad()
{
	if(!m_pTemplate)
	{
		MyLog::message("Can't load template [%s]!", m_strTemplate.c_str());
		return false;
	}

	if (m_batchID < 0 || m_batchID >= MAXBATCHCOUNT)
	{
		MyLog::message("BatchID is invalid	[%s] is [%d]", m_strTemplate.c_str(), m_batchID);
		return false;
	}

	if( m_pTemplate->IsLoaded())
	{
		MyLog::message("Don't loadtemplate:	[%s] batchID:[%d] again\n", m_strTemplate.c_str(), m_batchID);
		return false;
	}

	return true;
}

bool LoadInfo::CanBeLoad(int32& nWaitTemplateCnt)
{
	if (!m_pTemplate)
		return false;

	if( m_pTemplate->GetLoadState() != eLS_Unload )
		return false;

	// check dependence is loaded
	if (!m_pDependList)
		return true;

	for( auto itr = m_pDependList->begin(); itr != m_pDependList->end(); ++itr)
	{
		LoadTemplate* pTemplate = *itr;
		if (!pTemplate->IsLoaded())
		{
			nWaitTemplateCnt++;
			return false;
		}
	}

	return true;
}

LoadBatch::~LoadBatch()
{
	for (auto itr = m_list.begin(); itr != m_list.end(); ++itr)
		SAFE_DELETE(*itr);

	m_list.clear();
}

LoadTemplate* LoadBatch::GetTemplate2Load(int32 &nWaitTemplateCnt)
{
	for (auto itr = m_list.begin(); itr != m_list.end(); ++itr)
	{
		LoadInfo* pInfo = *itr;
		AUTOLOCK(m_mutex);
		if ( pInfo->CanBeLoad(nWaitTemplateCnt) )
		{
			LoadTemplate* pTemplate = pInfo->m_pTemplate;
			if(!pTemplate)
				continue;

			pTemplate->SetLoadState(eLS_Loading);
			pTemplate->m_strName = pInfo->m_strTemplate;
			return pTemplate;
		}
	}

	return NULL;
}

LoadThreadBatch::LoadThreadBatch()
{
	
}

LoadThreadBatch::~LoadThreadBatch()
{
	for (auto itr = m_list.begin(); itr != m_list.end(); ++itr)
	{
		if(!(*itr))
			continue;

		(*itr)->Stop();
		SAFE_DELETE(*itr);
	}

	m_list.clear();
}

void LoadThreadBatch::StartBatchLoad(LoadTemplateManager* pMgr, LoadBatch* pBatch, int32 nThreadCount)
{
	if (nThreadCount == 0)
	{
		LoadThread thread( pMgr, pBatch, 0);
		thread.ProcessLoad();
	}
	else
	{
		// create thread obj
		for (int32 i = 0; i < nThreadCount; ++i)
		{
			LoadThread* pThread = new LoadThread( pMgr, pBatch, i+1);
			if(!pThread)
				continue;

			m_list.push_back(pThread);
			pThread->Start();
		}
	}
}

bool LoadThreadBatch::IsAllThreadExit()
{
	for (auto itr = m_list.begin(); itr != m_list.end(); ++itr)
	{
		LoadThread* pThread = *itr;
		if(pThread->GetStatus() != eThread_Exiting)
			return false;
	}

	return true;
}

void LoadThreadBatch::WaitAllThreadExit()
{
	for (auto itr = m_list.begin(); itr != m_list.end(); ++itr)
	{
		LoadThread* pThread = *itr;
		pThread->Wait();
	}
}
