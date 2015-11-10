#pragma once
class FunctionBase_Arg0
{
public:
	FunctionBase_Arg0(void){}
	virtual ~FunctionBase_Arg0(void){}
	virtual void operator()(){}
};

template <class T>
class Function_Arg0 : public FunctionBase_Arg0
{
	typedef void (T::*Function)();
public:
	Function_Arg0(T* p, Function f)
	{
		pObj = p;
		func = f;
	}

	virtual ~Function_Arg0(){}

	virtual void operator()()
	{
		if(!pObj || !func)
			return;
		(pObj->*func)();
	}

private:
	T* pObj;
	Function func;
};

template<class T>
class FunctionBase_Arg1
{
public:
	FunctionBase_Arg1(void){}
	virtual ~FunctionBase_Arg1(void){}
	virtual void operator()(T& arg1){}
};

template <class T, class T1>
class Function_Arg1 : public FunctionBase_Arg1<T1>
{
	typedef void (T::*Function)(T1& arg1);
public:
	Function_Arg1(T* p, Function f)
	{
		pObj = p;
		func = f;
	}

	virtual ~Function_Arg1(){}

	virtual void operator()(T1& arg1)
	{
		if(!pObj || !func)
			return;
		(pObj->*func)(arg1);
	}

private:
	T* pObj;
	Function func;
};