#pragma once

#include <windows.h>
#include <map>
#include <vector>
#include <list>

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

class GDBRow
{
	DECLARE_FACTORY_ARG0_EX( GDBRow, -1);
public:
	GDBRow();
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
};

