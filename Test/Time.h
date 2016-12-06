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

	const Time& operator+=(const Time& time)
	{
		m_milliSec += time.m_milliSec;
		return *this;
	}

	bool operator!=(const Time& time) const
	{
		return m_milliSec != time.m_milliSec;
	}

	const Time& operator+(const Time& time)
	{
		m_milliSec += time.m_milliSec;
		return *this;
	}

	bool operator<(const Time& time) const
	{
		return m_milliSec < time.m_milliSec;
	}

	bool operator>(const Time& time) const
	{
		return m_milliSec > time.m_milliSec;
	}

	bool operator<=(const Time& time) const
	{
		return m_milliSec <= time.m_milliSec;
	}

	friend Time operator-(const Time& time1, const Time& time2);
	friend Time operator+(const Time& time1, const Time& time2);

private:
	uint64 m_milliSec;
};

inline Time operator-(const Time& time1, const Time& time2)
{
	return time1.m_milliSec - time2.m_milliSec;
}

inline Time operator+(const Time& time1, const Time& time2)
{
	return time1.m_milliSec + time2.m_milliSec;
}

const static Time ZERO_TIME;

