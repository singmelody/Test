#pragma once
class Time
{
public:
	Time(void);
	Time(uint64 millisec);
	~Time(void);

	uint64 MilliSecond() const
	{
		return m_milliSec;
	}

	uint32 Second() const
	{
		return (uint32)(m_milliSec/1000);
	}

	static Time CurrentTime();

	const Time& operator+(const Time& time)
	{
		m_milliSec += time.m_milliSec;
		return *this;
	}
private:
	uint64 m_milliSec;
};

