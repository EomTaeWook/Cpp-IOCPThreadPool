#pragma once
#include<Windows.h>
#include<process.h>
#include<vector>
#include "WaitCallback.h"

//작성자 엄태욱 2018-01-10
//IOCP 쓰레드 풀
namespace Threading
{
	template <typename T>
	class Singleton
	{
	public:
		Singleton() {}
		~Singleton() {}
		static std::shared_ptr<T> Instance();
	private:
		static std::shared_ptr<T> _instance;
	};

	template <typename T>
	static std::shared_ptr<T> Singleton<T>::Instance()
	{
		if (_instance.get() == 0)
		{
			_instance = std::make_shared<T>();
		}
		return _instance;
	}

	template <typename T>
	std::shared_ptr<T> Threading::Singleton<T>::_instance = NULL;

	#define CLOSE_THREAD -1
	class CIOCPThreadPool : public Singleton<CIOCPThreadPool>
	{
	private:
		HANDLE _completionPort;
		std::vector<HANDLE> _hWorkerThread;
		unsigned int _thread_Max_Size;
		CRITICAL_SECTION _cs;
	public:
		CIOCPThreadPool()
		{
			_completionPort = NULL;
			InitializeCriticalSection(&_cs);
		}
		~CIOCPThreadPool()
		{
			Stop();
			DeleteCriticalSection(&_cs);
		}
		bool Init(unsigned int threadSize = 0);

		bool InsertQueueItem(WaitCallback waitCallback, void* args);
	private:
		bool Stop();
		bool DeleteItem(CWaitCallback* WaitCallback);
		int Run();
		static unsigned int __stdcall WorkerThread(void*);
	};
}
