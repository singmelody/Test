#pragma once

#include "Singleton.h"

#define RAN32_MAX 0xffffffffLU
#define MY_RANDOM (MyRandom::Instance())

class MyRandom : public Singleton<MyRandom>
{
public:
	MyRandom(void);
	~MyRandom(void);

	void SetSeed();

	uint32 Rand();
	uint32 Rand32();
	f32 RandFloat();
	f32 RandFloat(f32 fRange);

	f32	RandFloat( f32 fMin, f32 fMax);
	uint32 RandInt( uint32 nMin, uint32 nMax);
protected:
	int32 m_nSeed;
	int32 m_nCount;
	int32 m_nOffset;
};

