#pragma once

#include <windows.h>
#include <map>
#include <vector>
#include <list>

#include "Factory.h"
#include "PoolAllocatorEx.h"

enum eDBType
{
	eDB_UnKonw = 0,
	eDB_INT,
	eDB_INT64,
	eDB_FLOAT,
	eDB_DOUBLE,
	eDB_BLOB, 
	eDB_NULL, 
	eDB_TEXT, 
	eDB_SMALLINT,
	eDB_TINYINT,
	eDB_BINARY
};

class GDBRow;

class GDBColumn
{
public:
	GDBColumn();
	~GDBColumn();

	void Type(int32 nType) { m_nType = nType; }
	int32 nType() { return m_nType; }

	virtual void FreeColumn() {}
	bool bIsNullValue;

	void SetIndex(int32 nIdx) { m_nIdx = nIdx; }
	int32 Index() { return m_nIdx; }

	void SetRow(GDBRow* row) { m_row = row; }
protected:
	int32 m_nType;
	int32 m_nIdx;
	GDBRow* m_row;
};


class GDBRow
{
	DECLARE_FACTORY_ARG0(GDBRow, -1, new PoolAllocatorEx)
public:
	GDBRow();
	~GDBRow();

	GDBColumn* GetColumn(int32 nIdx);
};


typedef std::list<GDBRow*> GDBRowList;
class GDBTable
{
	typedef std::map< std::string, int> GDBColumnMap;
	typedef std::vector<int> GDBType;
	typedef std::map< int, std::string> GDBColumnName;

public:
	GDBTable();
	~GDBTable();

	void AddColumn( const char* pColName, int32 nColumnID, int32 nValueType);
	const char* GetColumnName(int32 nIdx);
	int32 GetColumnIdx(const char* pCol);
	int32 GetColumnCount();
	int32 GetRowCount();
	int32 GetColumnType(int32 nIdx);

	GDBRowList m_rowList;
private:
	GDBType			m_ValueType;
	GDBColumnName	m_ColName;
	GDBColumnMap	m_ColMap;
};

class DBInterface
{
public:
	DBInterface();
	~DBInterface();

	virtual bool ConnectDB(const char* pConnectStr, const char* pUsername, const char* pPassword) = 0;
	virtual bool CloseDB() = 0;

	virtual bool ExecuteSqlInternal(const char* pSql, GDBTable* pTable) = 0;
	virtual bool GetResult(GDBTable* pTable);
};

