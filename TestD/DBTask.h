#pragma once

#include "FactoryManagerBase.h"
#include "Singleton.h"
#include "Factory.h"

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
		//vListNode.Set(this);
	}

	~DBTask(void) { pInterface = NULL; }

	virtual void Execute(DBTable& table){}
	void SetDBInterface( DBInterface* ptr) { pInterface = ptr; }
	//TListNode<DBTask>& GetListNode() { return &_list_node; }
	virtual bool DoAction(DBInterface* ptr) ;
	void Send2World( PacketBase& pkt);

	static void ScheduleSimpleTask(const std::string& strSql);
protected:
	int32				nSockID;
	int32				nAvatarID;
	uint32				nFlag;
	int32				nTableNum;
	int64				nAvatarDID;
	std::string			sqlTemplate;
	DBInterface*		pInterface;
	//std::list<DBTask*>	vListNode;
};

