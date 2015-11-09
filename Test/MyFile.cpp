#include "StdAfx.h"
#include "MyFile.h"


MyFile::MyFile(void)
{
}


MyFile::~MyFile(void)
{
}

FILE* SimpleFile::Open(const char* pFile, const char* flag /*= "rb"*/)
{
	m_fileName = pFile;
	FILE * fp = NULL;
	fopen_s( &fp, m_fileName.c_str(), flag);
	return fp;
}

void SimpleFile::Close(FILE* file)
{
	if (!file)
		return;

	fclose(file);
}

bool SimpleFile::EncryptFile(const char* szKey)
{
	return false;
}

bool SimpleFile::DecryptFile(const char* szKey)
{
	return false;
}
