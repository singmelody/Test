#pragma once
#include <string>

class MyFile
{
public:
	MyFile(void);
	~MyFile(void);
};

class SimpleFile : public MyFile
{
public:
	SimpleFile() { m_bEncrypt = false;}
	virtual ~SimpleFile() {}

	virtual FILE*	Open(const char* pFile, const char* flag = "rb");
	virtual void	Close(FILE* file);

	bool EncryptFile(const char* szKey);
	bool DecryptFile(const char* szKey);
protected:
	bool		m_bEncrypt;
	std::string m_fileName;
};
