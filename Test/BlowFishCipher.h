#pragma once

#include "Factory.h"
#include "PoolAllocator.h"


struct SKeyWrapper;

class BlowFishCipher
{
	DECLARE_FACTORY_ARG0( BlowFishCipher, -1, new PoolAllocator);
public:
	static bool GenerateKey(const char* keyBuff, int32 nKeyBuffLen);

	BlowFishCipher(void);
	~BlowFishCipher(void);

	void InitKey( const char* pszKey, int32 nKeyLen);

	void Encrypt( const void* in, const void* out, int32 nLen);

	void Decrypt( const void* in, const void* out, int32 nLen);

private:
	BlowFishCipher(const BlowFishCipher&);
	BlowFishCipher& operator=(const BlowFishCipher&);

	SKeyWrapper*	m_pKey;
	bool			m_bInited;
};

