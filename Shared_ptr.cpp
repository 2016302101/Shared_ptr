/**
 * 实现一个简单的shared_ptr
 * gl 2022.3.22
 */

#include<iostream>
#include<mutex>
#include<thread>
using namespace std;

template<class T>
class Shared_Ptr{
public:
	Shared_Ptr(T* ptr = nullptr)
		:_pPtr(ptr)
		, _pRefCount(new int(1))
		, _pMutex(new mutex)
	{} // 默认的构造函数
	~Shared_Ptr()
	{
		Release();
	}
	Shared_Ptr(const Shared_Ptr<T>& sp)
		:_pPtr(sp._pPtr)
		, _pRefCount(sp._pRefCount)
		, _pMutex(sp._pMutex)
	{
		AddRefCount();
	} // 拷贝构造函数
	Shared_Ptr<T>& operator=(const Shared_Ptr<T>& sp)
	{
		//if (this != &sp)
		if (_pPtr != sp._pPtr)
		{
			// 释放管理的旧资源
			Release();
			// 共享管理新对象的资源，并增加引用计数
			_pPtr = sp._pPtr;
			_pRefCount = sp._pRefCount;
			_pMutex = sp._pMutex;
			AddRefCount();
		}
		return *this;
	} // 拷贝赋值函数
	T& operator*(){
		return *_pPtr;
	}
	T* operator->(){
		return _pPtr;
	}
    // 重载指针的解引用和->
	int UseCount() { return *_pRefCount; }
	T* Get() { return _pPtr; }
	void AddRefCount()
	{
		_pMutex->lock();
		++(*_pRefCount);
		_pMutex->unlock();
	}
private:
	void Release()
	{
		bool deleteflag = false; // 需要判断什么时候释放mutex变量
		_pMutex->lock();
		if (--(*_pRefCount) == 0)
		{
			delete _pRefCount;
			delete _pPtr;
			deleteflag = true;
		}
		_pMutex->unlock();
		if (deleteflag == true)
			delete _pMutex;
	}
private:
	int *_pRefCount; // use_count
	T* _pPtr; // 存储对象的指针，sp本身就是一个指针类
	mutex* _pMutex; // 不同线程的智能指针必须互斥的访问use_count
};