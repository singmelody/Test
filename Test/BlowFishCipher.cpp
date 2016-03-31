#include "StdAfx.h"
#include "BlowFishCipher.h"

#include <openssl/rand.h>
#include <openssl/blowfish.h>
#include <assert.h>

struct SKeyWrapper
{
	DECLARE_FACTORY_ARG0( SKeyWrapper, -1, new PoolAllocator);
public:
	BF_KEY*	GetPtr() { return &m_key; }
	BF_KEY m_key;
};

FINISH_FACTORY_ARG0(SKeyWrapper);
FINISH_FACTORY_ARG0(BlowFishCipher);

BlowFishCipher::BlowFishCipher(void) : m_bInited(false)
{
	m_pKey = FACTORY_NEWOBJ(SKeyWrapper);
}


BlowFishCipher::~BlowFishCipher(void)
{
	FACTORY_DELOBJ(m_pKey);
}

bool BlowFishCipher::GenerateKey(const char* keyBuff, int32 nKeyBuffLen)
{
	if(!keyBuff || nKeyBuffLen < 0 )
		return false;

	return RAND_pseudo_bytes((unsigned char*)keyBuff, nKeyBuffLen) >= 0;
}

void BlowFishCipher::InitKey(const char* pszKey, int32 nKeyLen)
{
#ifdef _DEBUG
	assert( pszKey && nKeyLen > 0 && nKeyLen <= 256 && !m_bInited);
	m_bInited = true;
#endif

	if(!pszKey || nKeyLen < 0 || nKeyLen > 256)
		return;

	if(!m_pKey || !m_pKey->GetPtr() )
		return;

	BF_set_key( m_pKey->GetPtr(), nKeyLen, (unsigned char*)pszKey);
}

void BlowFishCipher::Encrypt(const void* in, const void* out, int32 nLen)
{
	assert(m_bInited);

	if(!m_pKey || !m_pKey->GetPtr())
		return;

	int32 n = 0;
	unsigned char iv[8] = {0};
	BF_ofb64_encrypt( (unsigned char*)in, (unsigned char*)out, nLen, m_pKey->GetPtr(), iv, &n);
}

void BlowFishCipher::Decrypt(const void* in, const void* out, int32 nLen)
{
	assert(m_bInited);

	if(!m_pKey || !m_pKey->GetPtr())
		return;

	int32 n = 0;
	unsigned char iv[8] = {0};
	BF_ofb64_encrypt( (unsigned char*)in, (unsigned char*)out, nLen, m_pKey->GetPtr(), iv, &n);
}
