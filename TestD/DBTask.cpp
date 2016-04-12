#include "StdAfx.h"
#include "DBTask.h"
#include "DBInterface.h"
#include "DBAServer.h"

FINISH_FACTORY_ARG0(DBTask)
bool DBTask::DoAction(DBInterface* ptr)
{
	DBTable table;
	if(!ptr->ExecuteSql( sqlTemplate.c_str(), &table) )
		return false;

	SetDBInterface(ptr);
	Execute(table);
	return true;
}

void DBTask::Send2World(PacketBase& pkt)
{
	DBAServer::Instance().Send2World(&pkt);
}

