#pragma once

#include <string	>

class LogicOuput
{
public:
	LogicOuput(void)
		: m_nType(-1)
		, m_nIdx(-1)
		, m_nOffset(0)
		, m_nSize(0)
	{}

	LogicOuput(int32 nOffset)
		: m_nType(-1)
		, m_nIdx(-1)
		, m_nOffset(nOffset)
		, m_nSize(0)
	{}

	~LogicOuput(void);

	const char* Name() { return m_Name.c_str(); }
	void Name(const char* sName) { m_Name = sName; }

	const char* Des() { return m_Des.c_str(); }
	void Des(const char* sDes) { m_Des = sDes; }

protected:
	int32 m_nType;
	int32 m_nIdx;
	int32 m_nOffset;
	int32 m_nSize;

	std::string m_Name;
	std::string m_Des;
};

