#pragma once

#include "FactoryManagerBase.h"
#include "Singleton.h"
#include "Factory.h"
#include "MyListNode.h"

class PacketBase;
class DBInterface;
class DBTable;

class TaskFactory : public FactoryManagerBase, public Singleton<TaskFactory>
{
public:
	TaskFactory();
	virtual ~TaskFactory();
};

class DBTask
{
	DECLARE_FACTORY_ARG0_Ex( DBTask, -1, new PoolAllocatorEx(), TaskFactory::Instance())
public:
	DBTask(void) : nSockID(-1), nAvatarID(0), nFlag(0), nTableNum(0), nAvatarDID(0)
	{

	}

	~DBTask(void) { pInterface = NULL; }

	virtual void Execute(DBTable& table){}
	void SetDBInterface( DBInterface* ptr) { pInterface = ptr; }
	MyListNode<DBTask*>* GetListNode() { return &vListNode; }
	virtual bool DoAction(DBInterface* ptr) ;
	void Send2World( PacketBase& pkt);

	static void ScheduleSimpleTask(const std::string& strSql);

	int64 GetAvatarDID() const { return nAvatarDID;}

	void SetSqlTemplate(const char* s) { sqlTemplate = s;}
	const char* GetSqlTemplate() const { return sqlTemplate.c_str(); }

	int32				nAvatarID;
	int64				nAvatarDID;
protected:
	int32				nSockID;
	uint32				nFlag;
	int32				nTableNum;

	std::string			sqlTemplate;
	DBInterface*		pInterface;
	MyListNode<DBTask*>	vListNode;
};

