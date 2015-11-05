#include "StdAfx.h"
#include "LoadTemplateManager.h"
#include "MyFile.h"
#include "Timer.h"
#include <sstream>

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
	if( itr != m_LoadTemplateMap.end() )
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
		printf("LoadTemplateManager::Load Failed\n");
		return;
	}

	DBConnection* pConn = DBLoader::GetDBConnection();
	if(pConn)
	{
		int32 nStartTickTotal = getMSTime();

		DBInterface* pDBI = pConn->m_pInterface;
		if(!pDBI)
		{
			printf("Load LoadModules data failed!\n");
			return;
		}

		std::stringstream sstr;
		sstr << "select * from LoadModules where modulename = '" << pModule << "'";

		DBTable table;
		if ( !pDBI->ExecuteSql( sstr.str().c_str(), table))
		{
			printf("Load LoadModules Data Failed\n");
			return;
		}

		int32 nCol_ModuleID		= table.GetColumnIdx("ModuleID");
		int32 nCol_ThreadCount	= table.GetColumnIdx("ThreadCount");
		int32 nCol_BindReferenceTemplates = table.GetColumnIdx("BindReferenceTemplates");

		GDBRowList& rows = table.m_rowList;
		for (auto itr = rows.begin(); itr != rows.end(); ++itr)
		{
			GDBRow& row = **itr;

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
}

void LoadTemplateManager::Load(DBInterface* pDBI, int32 nModuleID, int32 nThreadCount)
{

}

void LoadTemplateManager::InternalLoadTemplates(LoadInfoList& list, int32 nThreadCount)
{

}

DBConnection* DBLoader::GetDBConnection()
{
	if(!m_pDBMgr)
		return NULL;

	return m_pDBMgr->GetDBInterface();
}
