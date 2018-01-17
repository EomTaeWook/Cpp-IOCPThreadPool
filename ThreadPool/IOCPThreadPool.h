#pragma once
#include<Windows.h>
#include<process.h>
#include<functional>
#include<vector>
#define CLOSE_THREAD -1

typedef std::function<void(void*)> WaitCallback;
//작성자 엄태욱 2018-01-10
//IOCP 쓰레드 풀
class CWaitCallback
{
	friend class CIOCPThreadPool;
private:
	std::function<void(void*)> func;
	void* p_obj;
private:
	void Run();
public:
	CWaitCallback(WaitCallback callback, void* p_obj = NULL)
	{
		this->func = callback;
		this->p_obj = p_obj;
	}
	~CWaitCallback()
	{
	}
};
inline void CWaitCallback::Run()
{
	this->func(this->p_obj);
}

class CIOCPThreadPool
{
private:
	HANDLE completionPort;
	std::vector<HANDLE> m_hWorkerThread;
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
	bool Stop();
	bool InsertQueueItem(WaitCallback waitCallback, void* args);
private:
	bool DeleteItem(CWaitCallback* WaitCallback);
	int Run();
	static unsigned int __stdcall WorkerThread(void*);
};