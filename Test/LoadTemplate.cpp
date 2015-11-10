#include "StdAfx.h"
#include "LoadTemplate.h"
#include "DBInterface.h"
#include "LoadTemplateManager.h"
#include "MyLog.h"

LoadTemplate::LoadTemplate(void)
{
	SetLoadState(eLS_Unload);
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
				MyLog::message("start load %s from db", m_strName.c_str());
				bSucceed = LoadDataFromDB( pConn->m_pInterface );
				pConn->m_mutex.Unlock();

				if ( bSucceed )
					MyLog::message("LoadTempate:%s from DB Success", m_strName.c_str());
				else
					MyLog::message("LoadTempate:%s from DB Failed", m_strName.c_str());
			}
			catch (...)
			{
				pConn->m_mutex.Unlock();
				MyLog::message("LoadTempate:%s from DB Exception", m_strName.c_str());
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
