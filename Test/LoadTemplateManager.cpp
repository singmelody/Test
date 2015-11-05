#include "StdAfx.h"
#include "LoadTemplateManager.h"
#include "MyFile.h"

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
// 	if (!DBLOAD)
// 	{
// 	}
}

void LoadTemplateManager::Load(DBInterface* pDBI, int32 nModuleID, int32 nThreadCount)
{
	if(!DBLoader::OpenDB(MaxConnect))
		printf("LoadTemplateManager::Load Failed\n");

	DBConnection* pConn = DBLoader::GetDBConnection();
	if(!pConn)
	{
		
// 		//int32 nStartTickTotal = getMSTime();
// 
// 		DBInterface* pDBI = pConn->m_pInterface;
// 		if(!pDBI)
// 		{
// 			printf("Load LoadModules data failed!\n");
// 			return;
// 		}
	}
}

DBConnection* DBLoader::GetDBConnection()
{
	if(!m_pDBMgr)
		return NULL;

	return m_pDBMgr->GetDBInterface();
}
