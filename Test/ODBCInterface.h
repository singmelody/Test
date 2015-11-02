#pragma once
#include <string>
#include "DBInterface.h"
#include "sql.h"
#include "sqlext.h"
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

	virtual bool ExecuteSqlInternal( const char* pSql, GDBTable* pTable);
protected:
	void Clear();

	SQLHENV		m_hEnv;
	SQLHDBC		m_hDbc;
	SQLRETURN	m_result;

	SQLLEN		m_rowCount;
	SQLHSTMT	m_hStmt;
	SQLSMALLINT m_colCount;
	bool		m_bConnected;

	std::string m_connectStr;
	std::string m_username;
	std::string m_password;
};

