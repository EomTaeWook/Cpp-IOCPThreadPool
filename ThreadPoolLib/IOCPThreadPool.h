#pragma once
#include<Windows.h>
#include<process.h>
#include<vector>
#include "WaitCallback.h"
namespace Threading
{
	#define CLOSE_THREAD -1
	//작성자 엄태욱 2018-01-10
	//IOCP 쓰레드 풀
	class CIOCPThreadPool
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
