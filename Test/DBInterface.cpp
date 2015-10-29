#include "stdafx.h"
#include "DBInterface.h"
#include <assert.h>


DBColumn::DBColumn()
{

}


DBColumn::~DBColumn()
{

}

void DBTable::AddColumn(const char* pColName, int32 nColumnID, int32 nValueType)
{
	m_colMap[pColName] = nColumnID;
	m_colName[nColumnID] = pColName;
	m_valueType.push_back(nValueType);
}

const char* DBTable::GetColumnName(int32 nIdx)
{
	auto itr = m_colName.find(nIdx);
	if( itr == m_colName.end())
		return "";

	return itr->second.c_str();
}

int32 DBTable::GetColumnIdx(const char* pCol)
{
	auto itr = m_colMap.find(pCol);
	if(itr == m_colMap.end())
		return -1;

	return itr->second;
}

int32 DBTable::GetColumnCount()
{
	return (int32)m_colMap.size();
}

int32 DBTable::GetRowCount()
{
	return (int32)m_rowList.size();
}

int32 DBTable::GetColumnType(int32 nIdx)
{
	if ( nIdx < 0 || nIdx >= (int32)m_valueType.size())
		return 0;

	return m_valueType[nIdx];
}

bool DBInterface::LoadTable(const char* pTableName, DBTable& table)
{
	if(!pTableName)
	{
		printf("should not load empty table\n");
		return false;
	}

	std::string strSql = std::string("select * from ") + pTableName;
	if( TRUE == ExecuteSql( strSql.c_str(), table))
		return false;

	printf("Failed to load table\n", pTableName);
	return false;
}

DBInterface::DBInterface()
{
}


DBInterface::~DBInterface()
{

}

bool DBInterface::GetResult(DBTable* pTable)
{
	return true;
}

FINISH_FACTORY_ARG0(GDBRow)

GDBRow::GDBRow() : m_data(512)
{

}

GDBRow::~GDBRow()
{

}

DBColumn* GDBRow::GetColumn(int32 nIdx)
{
	m_column.Type(m_pTable->GetColumnType(nIdx));
	m_column.SetIndex(nIdx);
	m_column.SetRow(this);

	return &m_column;
}

GDBRowData::GDBRowData(int32 nInitBuffSize)
: _capacity(nInitBuffSize)
{
	_buff = new char[_capacity];
	assert( _buff != NULL );
}

GDBRowData::~GDBRowData()
{
	SAFE_DELETE_ARRAY(_buff);
}

size_t GDBRowData::Write(const char* data, size_t len)
{
	if(_cursor + len > _capacity)
	{
		while (_capacity < _cursor + len)
			_capacity <<=2;

		char* b = new char[_capacity];
		memcpy( b, _buff, _cursor);
		SAFE_DELETE_ARRAY(_buff);
		_buff = b;
	}

	memcpy( _buff + _cursor, data, len);
	size_t oldCursor = _cursor;
	_cursor += len;
	return oldCursor;
}

DBConnectionManager::DBConnectionManager(int32 n)
{
	m_con = n;
}

DBConnectionManager::~DBConnectionManager()
{

}

DBConnection* DBConnectionManager::GetDBInterface()
{
	uint32 i = 0;
	for (;;)
	{
		DBConnection* conn = &(m_connections[ (i++) % m_con]);

		if(conn->m_mutex.TryLock())
			return conn;
	}

	return NULL;
}
