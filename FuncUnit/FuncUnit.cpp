// FuncUnit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <time.h>
#include "TimePeriod.h"

std::vector<BaseTestUnit*> gUnits;

class Base {
public:
	virtual void f() { cout << "Base::f" << endl; }
	virtual void g() { cout << "Base::g" << endl; }
	void h() { cout << "Base::h" << endl; }

	template<>
};

typedef void(*Fun)(void);  //函数指针

int _tmain(int argc, _TCHAR* argv[])
{
	Base b;
	//  这里指针操作比较混乱,在此稍微解析下:

	//  *****printf("虚表地址:%p\n", *(int *)&b); 解析*****:
	//  1.&b代表对象b的起始地址
	//  2.(int *)&b 强转成int *类型,为了后面取b对象的前四个字节,前四个字节是虚表指针
	//  3.*(int *)&b 取前四个字节,即vptr虚表地址
	//

	//  *****printf("第一个虚函数地址:%p\n", *(int *)*(int *)&b);*****:
	//  根据上面的解析我们知道*(int *)&b是vptr,即虚表指针.并且虚表是存放虚函数指针的
	//  所以虚表中每个元素(虚函数指针)在32位编译器下是4个字节,因此(int *)*(int *)&b
	//  这样强转后为了后面的取四个字节.所以*(int *)*(int *)&b就是虚表的第一个元素.
	//  即f()的地址.
	//  那么接下来的取第二个虚函数地址也就依次类推.  始终记着vptr指向的是一块内存,
	//  这块内存存放着虚函数地址,这块内存就是我们所说的虚表.
	//
// 	printf("虚表地址:%p\n", &b);
// 	printf("虚表地址:%p\n", *(int *)&b);
// 	printf("第一个虚函数地址:%p\n", *(int *)*(int *)&b);
// 	printf("第二个虚函数地址:%p\n", *((int *)*(int *)(&b) + 1));
// 
// 	printf("%p", &Base::f);
// 	printf("%p", &Base::g);
// 
// 	Fun pfun = (Fun)*((int *)*(int *)(&b));  //vitural f();
// 	printf("f():%p\n", pfun);
// 	pfun();
// 
// 	pfun = (Fun)(*((int *)*(int *)(&b) + 1));  //vitural g();
// 	printf("g():%p\n", pfun);
// 	pfun();
// 
// 
// 	gUnits.push_back(new TimePeriod());
// 
// 	freopen("1.txt", "w", stdout);
// 
// 	for (auto itr = gUnits.begin(); itr != gUnits.end(); ++itr)
// 	{
// 		BaseTestUnit* pUnit = *itr;
// 		if(!pUnit)
// 			continue;
// 
// 		SetColor( FOREGROUND_GREEN | FOREGROUND_INTENSITY );
// 		std::cout<<typeid(*pUnit).name()<<" is UnderTest"<<endl;
// 		SetColor(0x0f);
// 
// 		if(pUnit->Calcuate())
// 		{
// 			SetColor( FOREGROUND_BLUE | FOREGROUND_INTENSITY );
// 			std::cout<<typeid(*pUnit).name()<<" is Passed"<<endl;
// 
// 		}
// 		else
// 		{
// 			SetColor( FOREGROUND_RED | FOREGROUND_INTENSITY );
// 			std::cout<<typeid(*pUnit).name()<<" is WrongError"<<endl;
// 		}
// 
// 		SetColor(0x0f);
// 	}

	return 0;
}
