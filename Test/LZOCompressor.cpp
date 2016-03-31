#include "StdAfx.h"
#include "LZOCompressor.h"
#include "MyLog.h"

bool s_is_lzo_Inited = false;
Mutex s_lzoMutex;

FINISH_FACTORY_ARG0(LZOCompressor)
LZOCompressor::LZOCompressor(void)
{

}

LZOCompressor::~LZOCompressor(void)
{
}

bool LZOCompressor::InitLzo()
{
	AutoLock tmp(&s_lzoMutex);

	if(s_is_lzo_Inited)
		return true;

	if(lzo_init() != LZO_E_OK)
	{
		MyLog::error("LZOCompressor::InitLzo() Failed");
		return false;
	}

	s_is_lzo_Inited = true;
	return true;
}

bool LZOCompressor::Compress(char* src, uint32 srcLen, char* dst, uint32 & destLen)
{
	lzo_uint compressdLen = lzo_uint(dst);

	int32 r = lzo1x_1_compress( (const unsigned char*)src, srcLen, (unsigned char*)dst, &compressdLen, wrkmem);
	if( r != LZO_E_OK )
	{
		MyLog::error("compression failed: code=[%d], srclen = [%u] dstlen = [%u]", r, srcLen, destLen);
		return false;
	}

	destLen = compressdLen;

#ifdef DEBUG_LZO_RATIO
	m_byteRaw += srcLen;
	m_byteCompressed += destLen;
	m_times++;

	if( m_byteRaw - m_byteRawPrint > 10*1024)
	{
		if(m_times == 0)
		{
			m_byteRaw = m_byteCompressed = m_byteRawPrint = 0;
		}
		else
		{
			m_byteRawPrint = m_byteRaw;
		}
	}
#endif

	return true;
}

bool LZOCompressor::DeCompress(char* src, uint32 nSrcLen, char* dst, uint32& destLen)
{
	lzo_uint decompressedLen = lzo_uint(destLen);

	int32 r = lzo1x_decompress_safe( (const unsigned char*)src, nSrcLen, (unsigned char*)dst, &decompressedLen, LZO1X_MEM_DECOMPRESS);
	if( r != LZO_E_OK || decompressedLen > destLen )
	{
		MyLog::error("DeCompression failed: code=[%d], srclen = [%u] dstlen = [%u]", r, nSrcLen, destLen);
		return false;
	}

	destLen = uint32(decompressedLen);
	return true;
}
