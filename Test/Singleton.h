#pragma once

template <class T>
class Singleton
{
public:
	static inline T& Instance()
	{
		static T _instance;
		return _instance;
	}

private:
	Singleton() {}
	~Singleton(){}
	Singleton(const Singleton<T>&);
	Singleton<T>& operator=(const Singleton<T>&);
};