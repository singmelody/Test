#include "StdAfx.h"
#include "MyMD5.h"

#define MD_BUFFER_SIZE 0x40
#define A 0x67452301
#define B 0xefcdab89
#define C 0x98badcfe
#define D 0x10325476

#define F( X ,Y ,Z ) (( X & Y ) | ( (~X) & Z ))
#define G( X ,Y ,Z ) (( X & Z ) | ( Y & (~Z) ))
#define H( X ,Y ,Z ) (X ^ Y ^ Z)
#define I( X ,Y ,Z ) (Y ^ ( X | (~Z) ))

#define FF(a ,b ,c ,d ,Mj ,s ,ti ) {a = b + ( (a + F(b,c,d) + Mj + ti) << s);}
#define GG(a ,b ,c ,d ,Mj ,s ,ti ) {a = b + ( (a + G(b,c,d) + Mj + ti) << s);}
#define HH(a ,b ,c ,d ,Mj ,s ,ti)  {a = b + ( (a + H(b,c,d) + Mj + ti) << s);}
#define II(a ,b ,c ,d ,Mj ,s ,ti)  {a = b + ( (a + I(b,c,d) + Mj + ti) << s);}

void MyMD::Init()
{
	m_stContext.count[0] = m_stContext.count[1] = 0;
	m_stContext.digest[0] = A;
	m_stContext.digest[1] = B;
	m_stContext.digest[2] = C;
	m_stContext.digest[3] = D;
}

void MyMD::Update(const void* pvSrc, uint32 cbSize)
{
	const byte* pbySrc = (const byte*)pvSrc;
	uint32 mdi, *pdw;

	mdi = (m_stContext.count[0] >> 3) & 0x3f;

	if((m_stContext.count[0] + (cbSize << 3)) < m_stContext.count[0])
		m_stContext.count[1]++;

	m_stContext.count[0] += cbSize << 3;
	m_stContext.count[1] += cbSize >> 29;

	while (cbSize--)
	{
		m_stContext.buffer[mdi++] = *pbySrc;

		if(mdi >= MD_BUFFER_SIZE)
		{
			pdw = (uint32*)m_stContext.buffer;
			Transform(pdw);
			mdi = 0;
		}
	}
}

void MyMD::Final()
{
	uint32 mdi, padLen, buffer[MD_BUFFER_SIZE/4],*pdw;

	buffer[MD_BUFFER_SIZE/4-2] = m_stContext.count[0];
	buffer[MD_BUFFER_SIZE/4-1] = m_stContext.count[1];

	mdi = (m_stContext.count[0] >>3) & 0x3f;

	padLen = (mdi < MD_BUFFER_SIZE - 8) ? 
		( MD_BUFFER_SIZE - 8 - mdi) : (MD_BUFFER_SIZE*2 - 8 - mdi);

	Update(m_abyPadding, padLen);

	*(uint32*)&m_stContext.buffer[MD_BUFFER_SIZE - 8] = buffer[MD_BUFFER_SIZE/4-2];
	*(uint32*)&m_stContext.buffer[MD_BUFFER_SIZE - 4] = buffer[MD_BUFFER_SIZE/4-1];
	pdw = (uint32*)m_stContext.buffer;

	Transform(pdw);
}

void MyMD4::Transform(const uint32 *in)
{

}


void MyMD5::Transform(const uint32 *in)
{
	uint32
	a = m_stContext.digest[0],
	b = m_stContext.digest[1],
	c = m_stContext.digest[2],
	d = m_stContext.digest[3];


	// roll 1
	FF(a ,b ,c ,d ,in[0],7 ,0xd76aa478 )
	FF(d ,a ,b ,c ,in[1] ,12 ,0xe8c7b756 )
	FF(c ,d ,a ,b ,in[2] ,17 ,0x242070db )
	FF(b ,c ,d ,a ,in[3] ,22 ,0xc1bdceee )
	FF(a ,b ,c ,d ,in[4 ],7 ,0xf57c0faf )
	FF(d ,a ,b ,c ,in[5 ],12 ,0x4787c62a )
	FF(c ,d ,a ,b ,in[6] ,17 ,0xa8304613 )
	FF(b ,c ,d ,a ,in[7] ,22 ,0xfd469501)
	FF(a ,b ,c ,d ,in[8] ,7 ,0x698098d8 )
	FF(d ,a ,b ,c ,in[9],12 ,0x8b44f7af )
	FF(c ,d ,a ,b ,in[10] ,17 ,0xffff5bb1 )
	FF(b ,c ,d ,a ,in[11] ,22 ,0x895cd7be )
	FF(a ,b ,c ,d ,in[12] ,7 ,0x6b901122 )
	FF(d ,a ,b ,c ,in[13] ,12 ,0xfd987193 )
	FF(c ,d ,a ,b ,in[14] ,17 ,0xa679438e )
	FF(b ,c ,d ,a ,in[15] ,22 ,0x49b40821 )
	// roll 2
	GG(a ,b ,c ,d ,in[1] ,5 ,0xf61e2562 )
	GG(d ,a ,b ,c ,in[6] ,9 ,0xc040b340 )
	GG(c ,d ,a ,b ,in[11] ,14 ,0x265e5a51 )
	GG(b ,c ,d ,a ,in[0] ,20 ,0xe9b6c7aa )
	GG(a ,b ,c ,d ,in[5] ,5 ,0xd62f105d )
	GG(d ,a ,b ,c ,in[10] ,9 ,0x02441453 )
	GG(c ,d ,a ,b ,in[15] ,14 ,0xd8a1e681 )
	GG(b ,c ,d ,a ,in[4] ,20 ,0xe7d3fbc8 )
	GG(a ,b ,c ,d ,in[9] ,5 ,0x21e1cde6 )
	GG(d ,a ,b ,c ,in[14] ,9 ,0xc33707d6 )
	GG(c ,d ,a ,b ,in[3] ,14 ,0xf4d50d87 )
	GG(b ,c ,d ,a ,in[8],20 ,0x455a14ed )
	GG(a ,b ,c ,d ,in[13] ,5 ,0xa9e3e905 )
	GG(d ,a ,b ,c ,in[2] ,9 ,0xfcefa3f8 )
	GG(c ,d ,a ,b ,in[7] ,14 ,0x676f02d9 )
	GG(b ,c ,d ,a ,in[12] ,20 ,0x8d2a4c8a )
	// roll 3
	HH(a ,b ,c ,d ,in[5] ,4 ,0xfffa3942 )
	HH(d ,a ,b ,c ,in[8] ,11 ,0x8771f681 )
	HH(c ,d ,a ,b ,in[11] ,16 ,0x6d9d6122 )
	HH(b ,c ,d ,a ,in[14] ,23 ,0xfde5380c )
	HH(a ,b ,c ,d ,in[1] ,4 ,0xa4beea44 )
	HH(d ,a ,b ,c ,in[4] ,11 ,0x4bdecfa9 )
	HH(c ,d ,a ,b ,in[7] ,16 ,0xf6bb4b60 )
	HH(b ,c ,d ,a ,in[10] ,23 ,0xbebfbc70 )
	HH(a ,b ,c ,d ,in[13] ,4 ,0x289b7ec6 )
	HH(d ,a ,b ,c ,in[0] ,11 ,0xeaa127fa )
	HH(c ,d ,a ,b ,in[3] ,16 ,0xd4ef3085 )
	HH(b ,c ,d ,a ,in[6] ,23 ,0x04881d05 )
	HH(a ,b ,c ,d ,in[9] ,4 ,0xd9d4d039 )
	HH(d ,a ,b ,c ,in[12] ,11 ,0xe6db99e5 )
	HH(c ,d ,a ,b ,in[15] ,16 ,0x1fa27cf8 )
	HH(b ,c ,d ,a ,in[2] ,23 ,0xc4ac5665 )
	// roll 4
	II(a ,b ,c ,d ,in[0],6 ,0xf4292244 )
	II(d ,a ,b ,c ,in[7] ,10 ,0x432aff97 )
	II(c ,d ,a ,b ,in[14] ,15 ,0xab9423a7 )
	II(b ,c ,d ,a ,in[5],21 ,0xfc93a039 )
	II(a ,b ,c ,d ,in[12] ,6 ,0x655b59c3 )
	II(d ,a ,b ,c ,in[3],10 ,0x8f0ccc92 )
	II(c ,d ,a ,b ,in[10] ,15 ,0xffeff47d )
	II(b ,c ,d ,a ,in[1] ,21 ,0x85845dd1 )
	II(a ,b ,c ,d ,in[8] ,6 ,0x6fa87e4f )
	II(d ,a ,b ,c ,in[15] ,10 ,0xfe2ce6e0 )
	II(c ,d ,a ,b ,in[6] ,15 ,0xa3014314 )
	II(b ,c ,d ,a ,in[13] ,21 ,0x4e0811a1 )
	II(a ,b ,c ,d ,in[4] ,6 ,0xf7537e82 )
	II(d ,a ,b ,c ,in[11] ,10 ,0xbd3af235 )
	II(c ,d ,a ,b ,in[2] ,15 ,0x2ad7d2bb )
	II(b ,c ,d ,a ,in[9] ,21 ,0xeb86d391 )

	m_stContext.digest[0] += a;
	m_stContext.digest[1] += b;
	m_stContext.digest[2] += c;
	m_stContext.digest[3] += d;
}
