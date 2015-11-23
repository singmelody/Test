#pragma once
#include <string>

typedef struct rsa_st RSA;

class RSAipher
{
public:
	static RSAipher* Create( FILE* pFile, bool bPublic);
	static void Destroy( RSAipher* pCipher );

	inline bool IsValid(){ return m_pKey && m_ptr0 && m_ptr1; }
	inline bool IsPubKey() { return m_bPublic; }

	bool Encrypt( const std::string& in, std::string& out);
	bool Decrypt( const std::string& in, std::string& out);
protected:
	RSAipher(RSA* pKey, bool bPublic);
	~RSAipher();

	RSAipher(const RSAipher& );
	RSAipher& operator=(const RSAipher& );

	const bool		m_bPublic;
	RSA* const		m_pKey;
	int32			m_nRSALen;
	unsigned char*	m_ptr0;
	unsigned char*	m_ptr1;
};

