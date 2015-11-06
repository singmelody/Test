#pragma once

#include <list>
#include "DBInterface.h"

#define MAXBATCHCOUNT	5

class LoadInfo;
class LoadTemplate;
class LoadTemplateManager;

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
	LoadTemplateList*		m_pDependTemplateList;
};

class LoadBatch
{
public:
	LoadBatch() : m_count(0) {}
	~LoadBatch();

	LoadTemplate* GetTemplate2Load( int32 &nWaitTemplateCnt);

	std::list<LoadInfo*>	m_list;
	int32					m_count;

	Mutex					m_mutex;
};

class LoadThread
{
public:
	LoadThread(void);
	~LoadThread(void);
};

