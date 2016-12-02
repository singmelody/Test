#pragma once

#include "Thread.h"
#include "WorkUnit.h"

class MyProcessThread : public ThreadBase, public WorkUnit
{
public:
	MyProcessThread(void);
	virtual ~MyProcessThread(void);

	void Stop();

private:
	void Run();
};

