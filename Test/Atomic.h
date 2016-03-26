#pragma once

void InterLockedAdd( volatile long * dst, long nVal);
void InterLockedAdd64( volatile int64 * dst, int64 nVal);
long InterLockedCompareExchange(volatile long *dst, long val, long compare);

class AtomicInt64
{
public:
	AtomicInt64(void):m_val(0), m_addTimes(0){}
	~AtomicInt64(void);

	void Add(int64 nVal);
	void Reset();
	inline int64 Get() const { return m_val; }
	inline int64 GetTimes() const { return m_addTimes; }
private:
	AtomicInt64(const AtomicInt64&);
	AtomicInt64& operator=( const AtomicInt64&);

	volatile int64 m_val;
	volatile int64 m_addTimes;
};

class SpinLock
{
public:
	SpinLock(volatile long* lock);
	~SpinLock();

private:
	SpinLock(const SpinLock& );
	SpinLock& operator=(const SpinLock&);

	volatile long* m_pLock;
};

class RealLock
{
public:
	RealLock(volatile long* lock);
	~RealLock();

private:
	RealLock(const RealLock& );
	RealLock& operator=(const RealLock&);

	volatile long* m_pLock;
};

class WriteLock
{
public:
	WriteLock(volatile long* lock);
	~WriteLock();

private:
	WriteLock(const RealLock& );
	WriteLock& operator=(const WriteLock&);

	volatile long* m_pLock;
};