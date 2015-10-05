#include "stdafx.h"
#include "ODBCInterface.h"


ODBCInterface::ODBCInterface()
{
	m_hEnv = NULL;
	m_hDbc = NULL;
	m_result = SQL_SUCCESS;

	m_rowCount = NULL;
	m_hStmt = NULL;
	m_colCount = NULL;

	m_bConnected = false;
}


ODBCInterface::~ODBCInterface()
{
	if (m_hStmt)
		SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);

	if (m_hDbc)
		SQLDisconnect(m_hDbc);

	if (m_hDbc)
		SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);

	if (m_hEnv)
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
}

bool ODBCInterface::ConnectDB(const char* pConnectStr, const char* pUsername, const char* pPassword)
{
	CloseDB();

	m_connectStr = pConnectStr;
	m_username = pUsername;
	m_password = pPassword;

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
	SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	SQLAllocHandle( SQL_HANDLE_DBC, m_hEnv, &m_hDbc);

	return true;
}

bool ODBCInterface::ExecuteSqlInternal(const char* pSql, GDBTable* pTable)
{
	if (!pTable || !pTable)
		return false;

	try
	{
		Clear();

		m_result = SQLExecDirect( m_hStmt, (SQLCHAR*)pSql, SQL_NTS);
		
		if (m_result != SQL_SUCCESS && m_result != SQL_SUCCESS_WITH_INFO && m_result != SQL_NO_DATA)
		{
			printf("this sql is error:%s\n", pSql);
			return false;
		}

		m_rowCount = m_colCount = 0;
		if (SQL_SUCCESS != SQLRowCount(m_hStmt, &m_rowCount))
		{
			printf("sql row count failed");
			return false;
		}

		if (SQL_SUCCESS != SQLNumResultCols(m_hStmt, &m_colCount))
		{
			printf("sql col count failed");
			return false;
		}

		if (m_colCount > MAXCOL)
			return false;

		if (m_colCount <= 0 && m_rowCount <= 0)
		{
			Clear();
			return true;
		}

		SQLCHAR rColName[MAX_COLUMN_NAMELEN];
		SQLSMALLINT Type;

		int32 nDBType = eDB_UnKonw;

		bool arrUnsignedFlag[MAXCOL + 1];
		for (int32 i = 0; i < m_colCount; ++i)
		{
			m_result = SQLDescribeCol(m_hStmt, i + 1, rColName, MAX_COLUMN_NAMELEN, NULL, &Type, NULL, NULL, NULL);

			if ( ( m_result != SQL_SUCCESS) && ( m_result != SQL_SUCCESS_WITH_INFO) && ( m_result != SQL_NO_DATA ))
				continue;

			arrUnsignedFlag[i] = false;

			// 对可能出现unsinged的数据列进行标记,方便dbtable统一处理
			switch (Type)
			{
			case SQL_INTEGER:
			case SQL_BIGINT:
				{
					SQLLEN len = 0;
					SQLRETURN ret = SQLColAttribute( m_hStmt, i+1, SQL_DESC_UNSIGNED, 0, 0, 0, &len);
					if (ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS)
						arrUnsignedFlag[i] = ( len == SQL_TRUE);
				}
				break;
			default:
				break;
			}

			switch (Type)
			{
			case SQL_INTEGER:
			case SQL_NUMERIC:
				nDBType = eDB_INT;
				break;
			case SQL_BIGINT:
				nDBType = eDB_INT64;
				break;
			case SQL_BINARY:
			case SQL_VARBINARY:
			case SQL_LONGVARBINARY:
				nDBType = eDB_BINARY;
				break;
			case SQL_CHAR:
			case SQL_VARCHAR:
			case SQL_LONGVARCHAR:
				nDBType = eDB_TEXT;
				break;
			case SQL_SMALLINT:
				nDBType = eDB_SMALLINT;
				break;
			case SQL_TINYINT:
				nDBType = eDB_TINYINT;
				break;
			case SQL_REAL:
			case SQL_FLOAT:
				nDBType = eDB_FLOAT;
				break;
			case SQL_DOUBLE:
				nDBType = eDB_DOUBLE;
			default:
				nDBType = eDB_UnKonw;
				break;
			}

			pTable->AddColumn( (char*)rColName, i, nDBType);
		}

		return GetResult( pTable);
	}
	catch (...)
	{
		printf("something bad happen\n");
	}

	return true;
}

void ODBCInterface::Clear()
{
	SQLCloseCursor(m_hStmt);
	SQLFreeStmt(m_hStmt, SQL_UNBIND);
}

bool ODBCInterface::CloseDB()
{
	return true;
}

bool ODBCInterface::GetResult(GDBTable* pTable)
{
	return true;
}
