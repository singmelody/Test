#include "StdAfx.h"
#include "MyRandom.h"
#include "Time.h"
#include "MyMath.h"

MyRandom::MyRandom(void) 
	: m_nSeed(Time::CurrentTime().Second())
	, m_nCount(0)
	, m_nOffset(0)
{
}


MyRandom::~MyRandom(void)
{
}

void MyRandom::SetSeed()
{

}

uint32 MyRandom::Rand()
{
	int32 nRand = rand();
	return nRand;
}

uint32 MyRandom::Rand32()
{
	uint32 nRand = rand() ^ ( (uint32) rand() << 15 ) ^ ((uint32)rand()<<30);
	return nRand;
}

f32 MyRandom::RandFloat(f32 fMin, f32 fMax)
{
	if( FLOAT_EQUAL( fMin, fMax))
		return fMin;

	if( fMin > fMax )
		std::swap( fMin, fMax);

	return fMin + (fMax - fMin) * RandFloat();
}

f32 MyRandom::RandFloat()
{
	return Rand() / f32(RAND_MAX);
}

f32 MyRandom::RandFloat(f32 fRange)
{
	return fRange * RandFloat();
}

uint32 MyRandom::RandInt(uint32 nMin, uint32 nMax)
{
	if( nMin == nMax)
		return nMin;

	if( nMin > nMax )
		std::swap( nMin, nMax);

	if( nMax - nMin + 1 <= 0 )
		return nMin;

	if( nMax <= RAND_MAX )
		return nMin + Rand() % (nMax - nMin + 1);
	else
		return nMin + Rand32() % ( nMax - nMin + 1);
}
