#pragma once

#include <vector>
#include "DBInterface.h"
#include "LoadThread.h"
#include "Thread.h"

#define MAXBATCHCOUNT	5

class LoadInfo;
class LoadTemplate;
class LoadTemplateManager;
class LoadThread;

typedef std::list<LoadTemplate*> LoadTemplateList;

class LoadInfo
{
public:
	LoadInfo( LoadTemplateManager* pMgr, DBRow &row);
	~LoadInfo();

	void AddDependence(LoadTemplate* ptr);

	bool CheckLoad();
	bool CanBeLoad( int32& nWaitTemplateCnt);

	std::string				m_strTemplate;

	int32 m_batchID;	// load group order
	int32 m_orderID;	// load order id

	LoadTemplate*			m_pTemplate;
private:

	LoadTemplateManager*	m_pMgr;
	LoadTemplateList*		m_pDependList;
};

class LoadBatch
{
public:
	LoadBatch() {}
	~LoadBatch();

	LoadTemplate* GetTemplate2Load( int32 &nWaitTemplateCnt);

	std::vector<LoadInfo*>	m_list;
	Mutex					m_mutex;
};

class LoadThread : public ThreadBase
{
public:
	LoadThread( LoadTemplateManager* pMgr, LoadBatch* pBatch, int32 nThreadIdx);
	~LoadThread(void);

	void ProcessLoad();
protected:
	void Run();

	LoadTemplateManager*	m_pMgr;
	LoadBatch*				m_pLoadBatch;
	const int32				m_nThreadIdx;
};

class LoadThreadBatch
{
public:
	LoadThreadBatch();
	~LoadThreadBatch();

	void StartBatchLoad(LoadTemplateManager* pMgr, LoadBatch* pBatch, int32 nThreadCount);
	bool IsAllThreadExit();
	void WaitAllThreadExit();

protected:
	std::list<LoadThread*>	m_list;

	bool m_bSyncMode;
};