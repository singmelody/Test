#pragma once
#include <string>
#include "DBInterface.h"
#include "sql.h"
#include "sqlext.h"

class DB_Static_DS;

struct DB_DS : public NoCopyable
{
public:
	DB_DS() : m_bRecordSqlStatics(false)
	{
		Reset();
	}

	~DB_DS();

	void Reset();
	void AddDB_DS(DB_DS& ds, bool bReset);
	void DoStatic(const char* szSql, int32 timeuse);
	void ToString( std::string& str, const std::string& strHeader);

	void EnableRecord()
	{
		m_bRecordSqlStatics = true;
	}

private:
	Mutex m_lock;
	typedef std::map< uint32, DB_Static_DS*> HashID2StaticMap;
	HashID2StaticMap m_mapHashID2Statics;
	int32 nTotalCount;
	int32 nTotalTimeUse;

	bool m_bEnableSqlStatics;
	bool m_bRecordSqlStatics;	
};

class ODBCInterface : public DBInterface
{
	static const int MAXCOL = 500;
	static const int MAXROW = 50000;
	static const int MAX_COLUMN_BUFFER = 2049;
	static const int MAX_COLUMN_NAMELEN = 30;
	static const int DEFAULT_ROW_BUFFER_SIZE = 512;
public:
	ODBCInterface();
	~ODBCInterface();

	virtual bool ConnectDB(const char* pConnectStr, const char* pUsername, const char* pPassword);
	virtual bool CloseDB();

	bool GetResult(DBTable* pTable, bool* arrColumnSignedFlag = 0);
	bool GetResult(DBTable* pTable, int32 nMaxRow, bool* arrFlag );

	virtual bool ExecuteSql( const Char* sSql, DBTable* pTable);
	virtual bool ExecuteSqlInternal( const char* pSql, DBTable* pTable);

	virtual bool IsConnect() { return m_connected; }

	void DiagState();
	DB_DS& GetDBDS() { return m_db_static; }
protected:
	void Clear();

	SQLHENV		m_hEnv;
	SQLHDBC		m_hDbc;
	SQLRETURN	m_result;

	SQLLEN		m_rowCount;
	SQLHSTMT	m_hStmt;
	SQLSMALLINT m_colCount;
	bool		m_connected;

	std::string m_connectStr;
	std::string m_username;
	std::string m_password;

	char		m_databuffer[MAX_COLUMN_BUFFER];
	SQLINTEGER	m_dataLength;
	SQLINTEGER	m_errorCode;

	DB_DS		m_db_static;
};

class ODBCConnectionManager : public DBConnectionManager
{
public:
	ODBCConnectionManager(int32 n);
	virtual ~ODBCConnectionManager();

	virtual bool Init( const char* dbName, const char* user, const char* pwd, const char* codepageSql);

	virtual void Close();
	virtual DBConnection* GetDBInterface();

};