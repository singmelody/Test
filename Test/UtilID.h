#pragma once

class UtilID
{
public:
	UtilID();
	UtilID(const char* pStr);
	UtilID(int nID);
	~UtilID();

	static unsigned int CreateFromString( const char* pStr);

	operator int() const { return m_id; }
	bool operator < (const UtilID& rhs) const;
	bool operator > (const UtilID& rhs) const;
	bool operator == (const UtilID& rhs) const;
	bool operator != (const UtilID& rhs) const;

	UtilID& operator= (UtilID& rhs);
	UtilID& operator= (const char* pStr);
	UtilID& operator= ( char* pStr);
	UtilID& operator= ( int nID);
private:
	unsigned int m_id;
};

