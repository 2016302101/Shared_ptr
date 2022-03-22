/**
 * ʵ��һ���򵥵�shared_ptr
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
	{} // Ĭ�ϵĹ��캯��
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
	} // �������캯��
	Shared_Ptr<T>& operator=(const Shared_Ptr<T>& sp)
	{
		//if (this != &sp)
		if (_pPtr != sp._pPtr)
		{
			// �ͷŹ���ľ���Դ
			Release();
			// ��������¶������Դ�����������ü���
			_pPtr = sp._pPtr;
			_pRefCount = sp._pRefCount;
			_pMutex = sp._pMutex;
			AddRefCount();
		}
		return *this;
	} // ������ֵ����
	T& operator*(){
		return *_pPtr;
	}
	T* operator->(){
		return _pPtr;
	}
    // ����ָ��Ľ����ú�->
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
		bool deleteflag = false; // ��Ҫ�ж�ʲôʱ���ͷ�mutex����
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
	T* _pPtr; // �洢�����ָ�룬sp�������һ��ָ����
	mutex* _pMutex; // ��ͬ�̵߳�����ָ����뻥��ķ���use_count
};