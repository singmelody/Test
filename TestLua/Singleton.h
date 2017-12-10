/** 	
　　Singleton模式的实现
　　File		:singleton.cpp 
　　Author		:singmelody 
　　Date		:2012.11.19
*/
#include <iostream>
using namespace std;

template <class T>
class Singleton
{
public:
    static inline T& Instance()
    {
        static T _instance;
        return _instance;
    }
protected:
    Singleton() {}
    ~Singleton() {}
    Singleton( const Singleton< T >& );
    Singleton< T >& operator= ( const Singleton< T > & );
}; 