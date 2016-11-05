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

typedef void(*Fun)(void);  //����ָ��

int _tmain(int argc, _TCHAR* argv[])
{
	Base b;
	//  ����ָ������Ƚϻ���,�ڴ���΢������:

	//  *****printf("����ַ:%p\n", *(int *)&b); ����*****:
	//  1.&b�������b����ʼ��ַ
	//  2.(int *)&b ǿת��int *����,Ϊ�˺���ȡb�����ǰ�ĸ��ֽ�,ǰ�ĸ��ֽ������ָ��
	//  3.*(int *)&b ȡǰ�ĸ��ֽ�,��vptr����ַ
	//

	//  *****printf("��һ���麯����ַ:%p\n", *(int *)*(int *)&b);*****:
	//  ��������Ľ�������֪��*(int *)&b��vptr,�����ָ��.��������Ǵ���麯��ָ���
	//  ���������ÿ��Ԫ��(�麯��ָ��)��32λ����������4���ֽ�,���(int *)*(int *)&b
	//  ����ǿת��Ϊ�˺����ȡ�ĸ��ֽ�.����*(int *)*(int *)&b�������ĵ�һ��Ԫ��.
	//  ��f()�ĵ�ַ.
	//  ��ô��������ȡ�ڶ����麯����ַҲ����������.  ʼ�ռ���vptrָ�����һ���ڴ�,
	//  ����ڴ������麯����ַ,����ڴ����������˵�����.
	//
// 	printf("����ַ:%p\n", &b);
// 	printf("����ַ:%p\n", *(int *)&b);
// 	printf("��һ���麯����ַ:%p\n", *(int *)*(int *)&b);
// 	printf("�ڶ����麯����ַ:%p\n", *((int *)*(int *)(&b) + 1));
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
