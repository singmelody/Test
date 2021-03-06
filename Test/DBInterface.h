#pragma once

#include <windows.h>
#include <map>
#include <vector>
#include <list>
#include <assert.h>

#include "Factory.h"
#include "PoolAllocatorEx.h"
#include "MyVector.h"

#define USE_SQLITE_DB		0	// sqlite may not be thread safe, please use single thread
#define USE_ODBC_DB			1
#define USE_CONNECTION		4	// may be less the num of thread , depends on the test result
#define MAX_USE_CONNECTION	16

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

class DBRow;
class DBTable;

class DBColumn
{
public:
	DBColumn();
	~DBColumn();

	void Type(int32 nType) { m_nType = nType; }
	int32 nType() { return m_nType; }

	virtual void FreeColumn() {}
	bool bIsNullValue;

	void SetIndex(int32 nIdx) { m_nIdx = nIdx; }
	int32 Index() { return m_nIdx; }

	void SetRow(DBRow* row) { m_row = row; }
	DBRow* Row() { return m_row; }
protected:
	int32	m_nType;
	int32	m_nIdx;
	DBRow*	m_row;
};

struct DBRowData
{
	DBRowData(int32 nInitBuffSize);
	~DBRowData();

	char*	_buff;
	size_t	_capacity;
	size_t	_cursor;

	size_t Write(const char* data, size_t len);

	template <class T>
	size_t Write(T nValue)
	{
		char* addr = reinterpret_cast<char*>(&nValue);
		return Write( addr, sizeof(T));
	}

	template <class T>
	void Read( size_t pos, T& val)
	{
		assert (pos < _cursor);
		val = *((T*)((char*)_buff + pos));
	}

	template<>
	void Read( size_t pos, std::string& val)
	{
		assert (pos < _cursor);
		val = (char*)((char*)_buff + pos);
	}
};

class DBRow
{
	DECLARE_FACTORY_ARG0(DBRow, -1, new PoolAllocatorEx)
public:
	DBRow();
	~DBRow();

	DBColumn* GetColumn(int32 nIdx);
	int32 GetColumnIdx(const char* pName);

	void AddColumn( const char* data, size_t len, bool isStr);
	void AddColumn();
	
	template <class T>
	void AddColumn(T val)
	{
		size_t oldCursor = m_data.Write(val);
		int64 columnFlag = sizeof(T);
		columnFlag <<= 32;
		int64 pos = (int64)oldCursor;
		pos &= 0x00000000ffffffff;
		m_columns.push_back( columnFlag | pos );
	}

	template <class T, class T1>
	void Fill(T& obj, int32 nColumnID, T1 defaultValue)
	{
		if( nColumnID < 0 || nColumnID >= (int32)m_columns.size())	
		{
			obj = defaultValue;
			return;
		}

		int64 nColumnFlag = m_columns[nColumnID];

		size_t pos = (size_t)(nColumnFlag & 0x00000000ffffffff);
		size_t len = (size_t)( (nColumnFlag >> 32) & 0x00000000ffffffff);

		if(len > 0)
		{
			m_data.Read( pos, obj);
		}
		else
			obj = defaultValue;
	}

	void Fill(Vector3& obj, int32 nColX, int32 nColY, int32 nColZ);

	void SetDBTable(DBTable* pTable) { m_pTable = pTable; }

	void Release() {}

	DBTable*		m_pTable;
protected:
	typedef std::vector<int64> ColumnInfoVec;
	ColumnInfoVec	m_columns;
	DBRowData		m_data;
	DBColumn		m_column;
};


typedef std::list<DBRow*> DBRowList;
class DBTable
{
	typedef std::map< std::string, int> GDBColumnMap;
	typedef std::vector<int> GDBType;
	typedef std::map< int, std::string> GDBColumnName;

public:
	DBTable();
	~DBTable();

	void AddColumn( const char* pColName, int32 nColumnID, int32 nValueType);
	const char* GetColumnName(int32 nIdx);
	int32 GetColumnIdx(const char* pCol);
	int32 GetColumnCount();
	int32 GetRowCount();
	int32 GetColumnType(int32 nIdx);

	DBRowList		m_rowList;
private:
	GDBType			m_valueType;
	GDBColumnName	m_colName;
	GDBColumnMap	m_colMap;
};

class DBInterface
{
public:
	DBInterface();
	~DBInterface();

	virtual bool ConnectDB(const char* pConnectStr, const char* pUsername, const char* pPassword) = 0;
	virtual bool CloseDB() = 0;

	virtual bool IsConnect() { return true; }

	virtual bool LoadTable( const char* pTableName, DBTable& table);
	virtual bool ExecuteSql( const Char* sSql, DBTable* pTable) = 0;
	virtual bool ExecuteSql( const Char* sSql, DBTable& pTable) { 
		return ExecuteSql( sSql, &pTable); 
	}
	virtual bool ExecuteSqlInternal(const char* pSql, DBTable* pTable) = 0;
};

struct DBConnection
{
	DBConnection() : m_pInterface(NULL)
	{
	}


#if !USE_SQLITE_DB
	Mutex m_mutex;
#else
	sturct SqliteMutex
	{
		void UnLock(){}
		bool AttemptLock() { return true;}
	}
#endif

	bool CheckDBConnect()
	{
		if(!m_pInterface)
			return false;

		if(!m_pInterface->IsConnect())
		{
			for (int32 i = 0; i < 5; ++i)
			{
			}
		}
	}

	DBInterface* m_pInterface;
};

class DBConnectionManager
{
public:
	DBConnectionManager(int32 n);
	virtual ~DBConnectionManager();

	virtual bool Init( const char* dbName, const char* user, const char* pwd, const char* codepageSql) { return false;}
	virtual void Close();

	DBConnection* GetDBInterface();


protected:
	DBConnection		m_connections[MAX_USE_CONNECTION];
	int32				m_con;
};

template <typename T> T GetRowValue(DBRow& row, int32 nColumnID, T defaultValue)
{
	return GetValue( row, nColumnID, defaultValue);
}

template <typename T> T GetValue(DBRow& row, int32 nColumnID, T defaultValue)
{
	T val;
	row.Fill( val, nColumnID, defaultValue);
	return val;
}