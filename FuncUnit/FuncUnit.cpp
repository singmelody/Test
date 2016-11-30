// FuncUnit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <time.h>
#include "TimePeriod.h"

struct Base
{
public:
	Base()
	{
		a = 100;
	}
	int a;
};

struct Derivied : public Base
{
public:
	Derivied()
	{
		k = 1000;
	}

	int k;
};

class A
{
public:
	Base* pPtr;

	void setptr(Base* p){ pPtr = p;}

	Base* test1(){ return pPtr;}
	virtual Base* test2(){ return pPtr; }
};

class B : public A
{
public:
	Derivied* test1(){ return (Derivied*)pPtr;}
	virtual Derivied* test2(){
		pPtr->a = -1; 
		return (Derivied*)pPtr; }
};

typedef void(*Fun)(void);  //º¯ÊýÖ¸Õë

int _tmain(int argc, _TCHAR* argv[])
{
	A* pPtr = new B();

	Base* k = new Derivied();

	pPtr->setptr(k);

	Derivied* ppp = pPtr->test1();
	std::cout<<ppp->a<<" "<<ppp->k<<endl;
	
	
	Derivied* pp = pPtr->test2();


	std::cout<<pp->a<<" "<<pp->k<<endl;

	return 0;
}
