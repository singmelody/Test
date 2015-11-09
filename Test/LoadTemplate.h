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

	inline	bool IsLoaded() { return m_loadState == eLS_Loaded; }
	LoadState GetLoadState() { return m_loadState; }
	void SetLoadState(LoadState state) { m_loadState = state; }

	virtual	std::string GetDataFolder(){ return m_folderPath; }
	inline void SetDataFolder( const char* pPath) { m_folderPath = pPath; }

	LoadTemplateManager* m_pLoadMgr;
	LoadTemplate* GetTemplate( const char* pStr);

	MyFile* CreateLoadFile();
	void ReleaseLoadFile(MyFile*& pFile);

	LoadState	m_loadState;
	LoadInfo*	m_pLoadInfo;

	bool		m_bLoadFromDB;
	std::string	m_strName;

	int32		m_LoadCastTicks;
	int32		m_nLoadThreadIdx;
protected:
	std::string	m_folderPath;
};

