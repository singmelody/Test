#include "StdAfx.h"
#include "LoadTemplate.h"
#include "DBInterface.h"
#include "LoadTemplateManager.h"

LoadTemplate::LoadTemplate(void)
{
	m_loadState = eLS_Unload;
	m_bLoadFromDB = true;

	m_pLoadInfo = NULL;
	m_pLoadMgr = NULL;
}


LoadTemplate::~LoadTemplate(void)
{
}

bool LoadTemplate::LoadData()
{
	bool bSucceed = false;

	if( m_bLoadFromDB )
	{
		DBConnection* pConn = DBLoader::GetDBConnection();
		if(pConn)
		{
			try
			{
				printf("start load %s from db\n", m_strName.c_str());
				bSucceed = LoadDataFromDB( pConn->m_pInterface );
				pConn->m_mutex.Unlock();

				if ( bSucceed )
					printf("LoadTempate:%s from DB Success\n", m_strName.c_str());
				else
					printf("LoadTempate:%s from DB Failed\n", m_strName.c_str());
			}
			catch (...)
			{
				pConn->m_mutex.Unlock();
				printf("LoadTempate:%s from DB Exception\n", m_strName.c_str());
			}
		}
	}
	else
	{
		// todo:load sqlite
	}

	return bSucceed;
}

bool LoadTemplate::LoadDataFromFile()
{
	return false;
}

bool LoadTemplate::LoadDataFromDB(DBInterface* ptr)
{
	return false;
}

bool LoadTemplate::ProcessLoad()
{
	ClearData();

	if (!LoadData())
		return false;

	AfterLoadData();	// 完成数据加载后整理数据
	return true;
}

LoadTemplate* LoadTemplate::GetTemplate(const char* pStr)
{
	return NULL;
}

MyFile* LoadTemplate::CreateLoadFile()
{
	if(!m_pLoadMgr)
		return NULL;

	return m_pLoadMgr->CreateLoadFile();
}

void LoadTemplate::ReleaseLoadFile(MyFile*& pFile)
{
	m_pLoadMgr->ReleaseLoadFile(pFile);
}
