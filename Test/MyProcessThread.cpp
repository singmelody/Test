#include "StdAfx.h"
#include "MyProcessThread.h"


MyProcessThread::MyProcessThread(void)
{
}


MyProcessThread::~MyProcessThread(void)
{
}

void MyProcessThread::Stop()
{
	Exit();
}

void MyProcessThread::Run()
{
	Execute();
}
