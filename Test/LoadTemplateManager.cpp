#include "StdAfx.h"
#include "LoadTemplateManager.h"
#include "MyFile.h"
#include "Timer.h"
#include <sstream>
#include "ODBCInterface.h"
#include "CommonConfig.h"
#include "MyLog.h"
#include "GameUtil.h"

DBConnectionManager* DBLoader::m_pDBMgr = NULL;
std::string DBLoader::m_strDBFile;
int32 DBLoader::m_maxThread = 4;

LoadTemplateManager::LoadTemplateManager(void)
{
	m_pLoadCallback = NULL;
}


LoadTemplateManager::~LoadTemplateManager(void)
{
	m_LoadTemplateMap.clear();
}

void LoadTemplateManager::AddTemplate(const char* strTemplate, LoadTemplate* pTemplate)
{
	auto itr = m_LoadTemplateMap.find(strTemplate);
	if( itr != m_LoadTemplateMap.end())
		return;

	pTemplate->m_pLoadMgr = this;
	m_LoadTemplateMap[strTemplate] = pTemplate;
}

void LoadTemplateManager::AddTemplate(const char* strTemplate, LoadTemplate* pTemplate, const char* strDataFolder)
{
	pTemplate->SetDataFolder( strDataFolder );
	AddTemplate( strTemplate, pTemplate);
}

LoadTemplate* LoadTemplateManager::GetTemplate(const char* strTemplate)
{
	auto itr = m_LoadTemplateMap.find(strTemplate);
	if( itr == m_LoadTemplateMap.end() )
		return NULL;

	return itr->second;
}

MyFile* LoadTemplateManager::CreateLoadFile()
{
	return new SimpleFile();
}

void LoadTemplateManager::ReleaseLoadFile(MyFile* &pFile)
{
	SAFE_DELETE(pFile);
}

void LoadTemplateManager::Load(const char* pModule)
{
	if(!DBLoader::OpenDB(MaxConnect))
	{
		MyLog::message("LoadTemplateManager::Load Failed");
		return;
	}

	DBConnection* pConn = DBLoader::GetDBConnection();
	if(pConn)
	{
		int32 nStartTickTotal = getMSTime();

		DBInterface* pDBI = pConn->m_pInterface;
		if(!pDBI)
		{
			MyLog::message("Load LoadModules data failed!");
			return;
		}

		std::stringstream sstr;
		sstr << "select * from LoadModules where ModuleName = '" << pModule << "'";

		DBTable table;
		if ( !pDBI->ExecuteSql( sstr.str().c_str(), table))
		{
			MyLog::message("Load LoadModules Data Failed");
			return;
		}

		static int32 nCol_ModuleID		= table.GetColumnIdx("ModuleID");
		static int32 nCol_ThreadCount	= table.GetColumnIdx("ThreadCount");
		static int32 nCol_BindReferenceTemplates = table.GetColumnIdx("BindReferenceTemplates");

		DBRowList& rows = table.m_rowList;
		for (auto itr = rows.begin(); itr != rows.end(); ++itr)
		{
			DBRow& row = **itr;

			int32 nModuleID = -1;
			row.Fill( nModuleID, nCol_ModuleID,	0);

			if ( nModuleID >= 0)
			{
				int32 nThreadCount = -1;
				row.Fill( nThreadCount,	nCol_ThreadCount,	0);

				if( nThreadCount > DBLoader::m_maxThread )
					nThreadCount = DBLoader::m_maxThread;

				if( nThreadCount > MaxThread )
					nThreadCount = MaxThread;

				Load( pDBI, nModuleID, nThreadCount);

				bool bBind = false;
				row.Fill( bBind, nCol_BindReferenceTemplates, false);

				if(bBind)
					BindAllReferenceTemplates();
			}
			row.Release();
		}
		
		m_nLoadTimeCastTotal = getMSTime() - nStartTickTotal;
		pConn->m_mutex.Unlock();
	}

	DBLoader::CloseDB();
}

void LoadTemplateManager::Load(DBInterface* pDBI, int32 nModuleID, int32 nThreadCount)
{
	std::stringstream sstr;
	sstr << "select * from LoadTemplates where ModuleID = "<< nModuleID <<" order by BatchID, OrderID";

	DBTable table;
	if (!pDBI->ExecuteSql( sstr.str().c_str(), table))
	{
		MyLog::message("LoadTemplate Data Failed");
		return;
	}

	LoadBatch loadBatches[MAXBATCHCOUNT];

	DBRowList& rows = table.m_rowList;

	for ( DBRowList::iterator itr = rows.begin(); itr != rows.end(); ++itr)
	{
		DBRow& row = **itr;

		LoadInfo* pInfo = new LoadInfo( this, row);
		if(!pInfo)
			assert(false);

		row.Release();

		if (!pInfo->CheckLoad())
		{
			SAFE_DELETE(pInfo);
			continue;
		}

		if(pInfo->m_batchID >= MAXBATCHCOUNT )
		{
			SAFE_DELETE(pInfo);
			continue;
		}

 		LoadBatch& batch = loadBatches[pInfo->m_batchID];
 		batch.m_list.push_back(pInfo);
	}

	LoadDependence( pDBI, nModuleID);

	for (int32 i = 0; i < MAXBATCHCOUNT; ++i)
	{
		LoadBatch& ins = loadBatches[i];
		if(ins.m_list.size() == 0)
			continue;

		InternalLoadTemplates( ins, nThreadCount);
	}

	OnLoadCompleted();
}

void LoadTemplateManager::LoadDependence(DBInterface* pDBI, int32 nModuleID)
{
	char textBuff[1024] = {0};
	sprintf_s( textBuff, "select * from LoadDependence where ModuleID = %d order by DependOrder", nModuleID);

	DBTable table;
	if (!pDBI->ExecuteSql( textBuff, table))
	{
		MyLog::message("Load LoadDependence data failed!");
		return;
	}

	static int32 nCol_TemplateName = table.GetColumnIdx("TemplateName");
	static int32 nCol_DependTemplate = table.GetColumnIdx("DependTemplate");

	auto itr = table.m_rowList.begin();
	auto itrEnd = table.m_rowList.end();

	for (; itr != itrEnd; ++itr)
	{
		DBRow& row = **itr;

		std::string strTemplate;
		std::string strDepend;
		row.Fill( strTemplate, nCol_TemplateName, std::string(""));
		row.Fill( strDepend, nCol_DependTemplate, std::string(""));

		row.Release();

		LoadTemplate* pTemplate = GetTemplate(strTemplate.c_str());
		if(!pTemplate)
			continue;

		if (pTemplate->IsLoaded())
			continue;

		LoadTemplate* pDepand = GetTemplate( strDepend.c_str() );
		if(!pDepand)
		{
			MyLog::message("Can't find DependTemplate [%s]->[%s]", strTemplate.c_str(), strDepend.c_str());
			continue;
		}

		pTemplate->m_pLoadInfo->AddDependence( pDepand );

		row.Release();
	}
}

void LoadTemplateManager::BindAllReferenceTemplates()
{
	for (auto itr = m_LoadTemplateMap.begin(); itr != m_LoadTemplateMap.end(); ++itr)
	{
		LoadTemplate* pTemplate = itr->second;
		if(!pTemplate)
			continue;

		pTemplate->BindReferenceTemplates();
	}
}

void LoadTemplateManager::OnLoadCompleted()
{

}

void LoadTemplateManager::InternalLoadTemplates(LoadBatch& list, int32 nThreadCount)
{
	LoadThreadBatch threads;

	if( nThreadCount > (int32)list.m_list.size() )
		nThreadCount = list.m_list.size();

	threads.StartBatchLoad( this, &list, nThreadCount);

	while (!threads.IsAllThreadExit())
	{
		if( m_pLoadCallback != NULL )
			m_pLoadCallback();

		// wait all thread exit
		GameUtil::Sleep(10);
	}

	threads.WaitAllThreadExit();
}

bool DBLoader::OpenDB(int32 nConnectCnt)
{
	switch( CommonConfig::DBType )
	{
	case eDBType_DBC:
		return OpenDBCenter( nConnectCnt );
	}

	return true;
}

void DBLoader::CloseDB()
{
	if(!m_pDBMgr)
		return;

	m_pDBMgr->Close();
	SAFE_DELETE(m_pDBMgr);
}

bool DBLoader::OpenDBCenter(int32 nConnectCnt)
{
	m_pDBMgr = new ODBCConnectionManager( nConnectCnt );

	if(!m_pDBMgr)
		return false;

	if(!m_pDBMgr->Init( CommonConfig::DBCName.c_str(), CommonConfig::DBCUser.c_str(), CommonConfig::DBCPwd.c_str(), "set names utf8"))
		return false;

	return true;
}

DBConnection* DBLoader::GetDBConnection()
{
	if(!m_pDBMgr)
		return NULL;

	return m_pDBMgr->GetDBInterface();
}

void DBLoader::SetDBFile(const char* strDBFile)
{
	m_strDBFile = strDBFile;
}
