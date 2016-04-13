#pragma once
class ShareMemory
{
public:
	ShareMemory(void);
	~ShareMemory(void);

	bool	Open( const char* szPath, SM_Key nKey, uint32 nSize, bool& bCreated);
	void	Close();
	void*	GetShareMemoryPtr() const;
	void	Write2File(const char* filePath);
	void	ReadFromFile( const char* filePath);

	static void SHMRM( const char* szPath, SM_KEY nKey);

protected:
	SM_KEY		m_SMKey;
	std::string m_SMName;
};

