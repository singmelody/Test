#pragma once
#include "basetestunit.h"

class TimePeriod :
	public BaseTestUnit
{
public:
	struct Info
	{
		int32 nDay;
		int32 nTod;
	};

	TimePeriod(void);
	virtual ~TimePeriod(void);

	virtual bool Calcuate();
	virtual bool Verify();
	virtual bool Verify2();


	vector<int32> m_start;
	vector<int32> m_end;

	vector<int32> m_totalStart;
	vector<int32> m_totalEnd;
};

