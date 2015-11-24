#include "StdAfx.h"
#include "RSAipher.h"
#include "MyLog.h"
#include <stdio.h>
#include <assert.h>

#include "openssl/pem.h"
#include "openssl/rsa.h"
#include "openssl/evp.h"
#include "openssl/err.h"

RSAipher::RSAipher(RSA* pRsa, bool bPublic) 
	: m_pKey(pRsa), m_bPublic(bPublic), m_ptr0(NULL), m_ptr1(NULL)
{
	if(!m_pKey)
		return;

	m_nRSALen = RSA_size(m_pKey);
	if(m_nRSALen <= 0)
		MyLog::error("This is a bad key!");

	m_ptr0 = new unsigned char[m_nRSALen];
	m_ptr1 = new unsigned char[m_nRSALen];
}


RSAipher::~RSAipher(void)
{
	if(m_pKey)
		RSA_free(m_pKey);

	if(m_ptr0)
		SAFE_DELETE(m_ptr0);

	if(m_ptr1)
		SAFE_DELETE(m_ptr1);
}

RSAipher* RSAipher::Create( FILE* pFile, bool bPublic)
{
	if(!pFile)
		return NULL;

	RSA* pKey = bPublic ? PEM_read_RSA_PUBKEY( pFile, NULL, NULL, NULL) : PEM_read_RSAPrivateKey( pFile, NULL, NULL, NULL);

	if(!pKey)
	{
		assert(false);
		return NULL;
	}

	return new RSAipher( pKey, bPublic);
}

void RSAipher::Destroy( RSAipher* pCipher)
{
	if(pCipher)
		SAFE_DELETE(pCipher);
}

bool RSAipher::Encrypt(const std::string& in, std::string& out)
{
	if(!IsValid())
		return false;

	int32 nLeft = int32(in.length());
	const unsigned char* pIn = (const unsigned char *)in.c_str();

	do 
	{
		memset( m_ptr0, 0, m_nRSALen);
		memset( m_ptr1, 0, m_nRSALen);

		const int32 inLen = nLeft > m_nRSALen - RSA_PKCS1_PADDING_SIZE ? m_nRSALen - RSA_PKCS1_PADDING_SIZE : nLeft;
		memcpy( m_ptr0, pIn, inLen);
		
		int32 nRetLen = IsPubKey() ? 
			RSA_public_encrypt( inLen, m_ptr0, m_ptr1, m_pKey, RSA_PKCS1_PADDING) : 
			RSA_private_encrypt( inLen, m_ptr0, m_ptr1, m_pKey, RSA_PKCS1_PADDING);

		if( nRetLen < 0 )
			break;

		nLeft -= inLen;
		pIn += inLen;

		out += inLen;
	} while ( nLeft > 0);

	return nLeft <= 0;
}

bool RSAipher::Decrypt(const std::string& in, std::string& out)
{
	if(!IsValid())
		return false;

	out.clear();

	int32 nLeft = int32(in.length());
	const unsigned char* pIn = (const unsigned char*)in.c_str();

	if(nLeft % m_nRSALen != 0)
		return false;

	do 
	{
		memset( m_ptr0, 0, m_nRSALen);
		memset( m_ptr1, 0, m_nRSALen);

		int32 nRetLen = IsPubKey() ? RSA_public_decrypt( m_nRSALen, m_ptr0, m_ptr1, m_pKey, RSA_PKCS1_PADDING) 
			:  RSA_private_decrypt( m_nRSALen, m_ptr0, m_ptr1, m_pKey, RSA_PKCS1_PADDING);


	} while (nLeft > 0);

	return nLeft <= 0;
}
