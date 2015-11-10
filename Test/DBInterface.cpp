#include "stdafx.h"
#include "DBInterface.h"
#include <assert.h>
#include "MyLog.h"

DBColumn::DBColumn()
{

}


DBColumn::~DBColumn()
{

}

DBTable::DBTable()
{
}

DBTable::~DBTable()
{
	for (auto itr = m_rowList.begin(); itr != m_rowList.end(); ++itr)
		FACTORY_DELOBJ(*itr);
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
		MyLog::message("should not load empty table");
		return false;
	}

	std::string strSql = std::string("select * from ") + pTableName;
	if( TRUE == ExecuteSql( strSql.c_str(), table))
		return false;

	MyLog::message("Failed to load table\n", pTableName);
	return false;
}

DBInterface::DBInterface()
{
}


DBInterface::~DBInterface()
{

}

FINISH_FACTORY_ARG0(DBRow)

DBRow::DBRow() : m_data(512)
{

}

DBRow::~DBRow()
{

}

DBColumn* DBRow::GetColumn(int32 nIdx)
{
	m_column.Type(m_pTable->GetColumnType(nIdx));
	m_column.SetIndex(nIdx);
	m_column.SetRow(this);

	return &m_column;
}

int32 DBRow::GetColumnIdx(const char* pName)
{
	if(!m_pTable)
		return -1;

	int32 nIdx = m_pTable->GetColumnIdx(pName);
	return nIdx;
}

void DBRow::AddColumn(const char* data, size_t len, bool isStr)
{
	size_t oldCursor = m_data.Write( data, len);
	if (isStr)
	{
		static char strEnd = '\0';
		m_data.Write( &strEnd, 1);
		len += 1;
	}

	int64 nColumnFlag = len;
	nColumnFlag <<= 32;
	int64 pos = (int64)oldCursor;
	pos &= 0x00000000ffffffff;
	m_columns.push_back( nColumnFlag | pos );
}

void DBRow::AddColumn()
{
	m_columns.push_back((int64)0);
}

DBRowData::DBRowData(int32 nInitBuffSize)
: _capacity(nInitBuffSize)
{
	_buff = new char[_capacity];
	assert( _buff != NULL );
}

DBRowData::~DBRowData()
{
	SAFE_DELETE_ARRAY(_buff);
}

size_t DBRowData::Write(const char* data, size_t len)
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
	if( n > MAX_USE_CONNECTION )
		m_con = MAX_USE_CONNECTION;

	m_con = n;
}

DBConnectionManager::~DBConnectionManager()
{
	for (int32 i = 0; i < m_con; ++i)
	{
		if(!m_connections[i].m_pInterface)
			continue;

		m_connections[i].m_pInterface->CloseDB();
		SAFE_DELETE(m_connections->m_pInterface);
	}
}

void DBConnectionManager::Close()
{
	for (int i = 0; i < m_con; ++i)
	{
		DBConnection& con = m_connections[i];
		if(!con.m_pInterface)
			continue;

		con.m_pInterface->CloseDB();
		SAFE_DELETE(con.m_pInterface);
	}
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
