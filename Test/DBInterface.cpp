#include "stdafx.h"
#include "DBInterface.h"


void GDBTable::AddColumn(const char* pColName, int32 nColumnID, int32 nValueType)
{

}

const char* GDBTable::GetColumnName(int32 nIdx)
{
	return NULL;
}

DBInterface::DBInterface()
{
}


DBInterface::~DBInterface()
{
}

bool DBInterface::GetResult(GDBTable* pTable)
{
	return true;
}

FINISH_FACTORY_ARG0(GDBRow)
GDBRow::GDBRow()
{

}

GDBRow::~GDBRow()
{

}

GDBColumn* GDBRow::GetColumn(int32 nIdx)
{
	return NULL;
}
