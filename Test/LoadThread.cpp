#include "StdAfx.h"
#include "LoadThread.h"
#include "LoadTemplateManager.h"

LoadThread::LoadThread(void)
{
}


LoadThread::~LoadThread(void)
{
}

LoadInfo::LoadInfo(LoadTemplateManager* pMgr, DBRow &row)
{
	static int32 nCol_TemplateName = -1;
	static int32 nCol_BatachID = -1;
	static int32 nCol_OrderID = -1;

	row.Fill( m_strTemplate, nCol_TemplateName, std::string(""));
	row.Fill( m_batchID, nCol_BatachID, 0);
	row.Fill( m_orderID, nCol_OrderID, 0);

	m_pTemplate = m_pMgr->GetTemplate( m_strTemplate.c_str());
	if ( m_pTemplate != NULL )
		m_pTemplate->m_pLoadInfo = this;

	m_pDependTemplateList = NULL;
}

LoadInfo::~LoadInfo()
{
	SAFE_DELETE(m_pDependTemplateList);
	SAFE_DELETE(m_pTemplate);
}

void LoadInfo::AddDependence(LoadTemplate* ptr)
{
	if(!m_pDependTemplateList)
		m_pDependTemplateList = new LoadTemplateList();

	m_pDependTemplateList->push_back(ptr);
}

bool LoadInfo::CheckLoad()
{
	if(!m_pTemplate)
	{
		printf("Can't load template [%s]!", m_strTemplate.c_str());
		return false;
	}

	if (m_batchID < 0 || m_batchID >= MAXBATCHCOUNT)
	{
		printf("BatchID is invalid	[%s] is [%d]", m_strTemplate.c_str(), m_batchID);
		return false;
	}

	if( m_pTemplate->IsLoaded())
	{
		printf("Don't loadtemplate:	[%s] batchID:[%d] again\n", m_strTemplate.c_str(), m_batchID);
		return false;
	}

	return true;
}

bool LoadInfo::CanBeLoad(int32& nWaitTemplateCnt)
{
	if (!m_pTemplate)
		return false;

	if( m_pTemplate->m_loadState != eLS_Unload )
		return false;

	// check dependence is loaded
	if (m_pDependTemplateList != NULL)
	{
		for( auto itr = m_pDependTemplateList->begin(); itr != m_pDependTemplateList->end(); ++itr)
		{
			LoadTemplate* pTemplate = *itr;
			if (!pTemplate->IsLoaded())
			{
				nWaitTemplateCnt++;
				return false;
			}
		}
	}
	return true;
}

LoadBatch::~LoadBatch()
{
	for (auto itr = m_list.begin(); itr != m_list.end(); ++itr)
		SAFE_DELETE(*itr);
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
			pTemplate->m_loadState = eLS_Loading;
			pTemplate->m_strName = pInfo->m_strTemplate;
		
			return pTemplate;
		}
	}

	return NULL;
}