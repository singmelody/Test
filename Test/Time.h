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

	static Time CurrentTime();
private:
	uint64 m_milliSec;
};

