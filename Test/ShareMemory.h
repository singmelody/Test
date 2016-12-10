#pragma once

#include "BaseType.h"
#include <string>

class ShareMemory
{
public:
	ShareMemory(void);
	~ShareMemory(void);

	bool	Open( const char* szPath, SM_KEY nKey, uint32 nSize, bool& bCreated);
	void	Close();
	void	Detach();

	void	Reset();

	void*	GetShareMemoryPtr() const;
	void	Write2File(const char* filePath);
	void	ReadFromFile( const char* filePath);
	void	CloseShareMemory();

	static void SHMRM( const char* szPath, SM_KEY nKey);

protected:
	int32		m_SMSize;
	SM_KEY		m_SMKey;
	std::string m_SMName;
	void*		m_SMMemPtr;

	SMHandle	m_SMHandle;
};

