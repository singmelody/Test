#pragma once

#include <string>

class DBInterface;
class MyFile;
class LoadTemplateManager;
class LoadInfo;

enum LoadState
{
	eLS_Unload	= 0,
	eLS_Loading	= 1,
	eLS_Loaded	= 2
};

class LoadTemplate
{
public:
	LoadTemplate();
	virtual ~LoadTemplate();

	virtual bool LoadData();
	virtual bool LoadDataFromFile();	// odbc interface
	virtual bool LoadDataFromDB(DBInterface* ptr);
	virtual void AfterLoadData(){}

	virtual void BindReferenceTemplates(){}

	virtual void ClearData(){}

	bool ProcessLoad();

	LoadState	m_loadState;
	LoadInfo*	m_pLoadInfo;

	bool		m_bLoadFromDB;
	std::string	m_strName;

	int32		m_LoadCastTicks;
	int32		m_nLoadThreadIdx;

	inline		bool IsLoaded() { return m_loadState == eLS_Loaded; }

	virtual	std::string GetDataFolder(){ return m_folderPath; }
	inline void SetDataFolder( const char* pPath) { m_folderPath = pPath; }

	LoadTemplateManager* m_pLoadMgr;
	LoadTemplate* GetTemplate( const char* pStr);

	MyFile* CreateLoadFile();
	void ReleaseLoadFile(MyFile*& pFile);
protected:
	std::string	m_folderPath;
};

