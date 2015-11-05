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
class GDBTable;

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
	GDBRow* Row() { return m_row; }
protected:
	int32	m_nType;
	int32	m_nIdx;
	GDBRow* m_row;
};

struct GDBRowData
{
	GDBRowData(int32 nInitBuffSize);
	~GDBRowData();

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
		*val = *((T*)((char*)_buff + pos));
	}

};
class GDBRow
{
	DECLARE_FACTORY_ARG0(GDBRow, -1, new PoolAllocatorEx)
public:
	GDBRow();
	~GDBRow();

	GDBColumn* GetColumn(int32 nIdx);

	void AddColumn( const char* data, size_t len, bool isStr);
	
	template <class T>
	void AddColumn(T val)
	{
		size_t oldCursor = m_data.Write(val);
		T columnFlag = sizeof(T);
		columnFlag <<= 32;
		int64 pos = (int64)oldCursor;
		pos &= 0x00000000ffffffff;
		m_column.push_back( columnFlag | pos );
	}

	template <class T>
	void Fill(T& obj, int32 nColumnID, T defaultValue)
	{

	}

	GDBTable*		m_pTable;
protected:
	typedef std::vector<int64> ColumnInfoVec;
	ColumnInfoVec	m_columns;
	GDBRowData		m_data;
	GDBColumn		m_column;
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

	GDBRowList		m_rowList;
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

	virtual bool ExecuteSqlInternal(const char* pSql, GDBTable* pTable) = 0;
	virtual bool GetResult(GDBTable* pTable);

	virtual bool IsConnect() { return true; }
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

	DBConnection* GetDBInterface();


protected:
	DBConnection*		m_connections;
	int32				m_con;
};