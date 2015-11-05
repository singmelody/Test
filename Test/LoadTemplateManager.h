#pragma once

#include <list>
#include <map>
#include "LoadTemplate.h"
#include "LoadThread.h"
#include "DBInterface.h"

class DBInterface;
struct DBConnection;

typedef std::list<LoadTemplate*> LoadTemplateList;
typedef std::map< std::string, LoadTemplate*> LoadTemplateMap;

// 1.�����μ���
// 2.ÿ�������������������ļ����̲߳��м���

class LoadTemplateManager
{
public:
	enum
	{
		MaxThread	= 4,
		MaxConnect	= MaxThread + 1
	};

	LoadTemplateManager();
	virtual ~LoadTemplateManager();

	void AddTemplate( const char* strTemplate, LoadTemplate* pTemplate);
	void AddTemplate( const char* strTemplate, LoadTemplate& Template)
	{
		AddTemplate( strTemplate, &Template);
	}

	void AddTemplate( const char* strTemplate, LoadTemplate* pTemplate, const char* strDataFolder);
	void AddTemplate( const char* strTemplate, LoadTemplate& Template, const char* strDataFolder)
	{
		AddTemplate( strTemplate, &Template, strDataFolder);
	}

	void Load( const char* pModule);
	void BindAllReferenceTemplates();

	LoadTemplate* GetTemplate( const char* strTemplate);

	void (*m_pLoadCallback)();	// ���ع��������̻߳ص�����ָ��

	virtual MyFile* CreateLoadFile();
	virtual void ReleaseLoadFile(MyFile* &pFile);

private:
	void Load( DBInterface* pDBI, int32 nModuleID, int32 nThreadCount);
	void LoadDependence( DBInterface* pDBI, int32 nModuleID);
	void OnLoadCompleted();

private:
	void InternalLoadTemplates( LoadInfoList& list, int32 nThreadCount);

	LoadTemplateMap		m_LoadTemplateMap;
	int32				m_nLoadTimeCastTotal;
};

class DBLoader
{
public:
	static bool OpenDB( int32 nConnectCnt );
	static void CloseDB();

	static bool OpenDBCenter( int32 nConnectCnt );
	static bool OpenDBLocal( const char* dbName, const char* dbFileKey);	// ����sqliteʹ��

	static bool SetDBLocalPassword( const char* dbName );

	static DBConnection* GetDBConnection();
	static void SetDBFile( const char* strDBFile);

	static int m_maxThread;

	static const char* GetDBFile() { return m_strDBFile.c_str(); }

private:

	static DBConnectionManager& m_pDBMgr;
	static std::string m_strDBFile;
};