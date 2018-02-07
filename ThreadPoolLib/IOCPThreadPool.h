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
		HANDLE completionPort;
		std::vector<HANDLE> hWorkerThread;
		unsigned int thread_Max_Size;
		CRITICAL_SECTION cs;
	public:
		CIOCPThreadPool()
		{
			completionPort = NULL;
			InitializeCriticalSection(&cs);
		}
		~CIOCPThreadPool()
		{
			Stop();
			DeleteCriticalSection(&cs);
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
