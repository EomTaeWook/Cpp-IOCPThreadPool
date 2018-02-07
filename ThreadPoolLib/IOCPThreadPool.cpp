#include "IOCPThreadPool.h"
#include "Finally.h"
#include <memory>

bool CIOCPThreadPool::Init(unsigned int threadMaxSize)
{
	if (!Stop())
	{
		return false;
	}
	completionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (completionPort == INVALID_HANDLE_VALUE)
		return false;

	SYSTEM_INFO info;
	GetSystemInfo(&info);

	if (threadMaxSize > 0)
	{
		if (info.dwNumberOfProcessors * 2 < threadMaxSize)
			thread_Max_Size = info.dwNumberOfProcessors * 2;
		else
			thread_Max_Size = threadMaxSize;
	}
	else
	{
		thread_Max_Size = info.dwNumberOfProcessors * 2;
	}
	for (unsigned int i = 0; i < thread_Max_Size; i++)
	{
		m_hWorkerThread.push_back((HANDLE)_beginthreadex(0, 0, WorkerThread, this, 0, NULL));
	}
	return true;
}
bool CIOCPThreadPool::Stop()
{
	if (completionPort)
	{
		for (int i = 0; i < m_hWorkerThread.size(); i++)
			PostQueuedCompletionStatus(completionPort, 0, CLOSE_THREAD, NULL);

		for (int i = 0; i < m_hWorkerThread.size(); i++)
		{
			WaitForSingleObject(m_hWorkerThread[i], INFINITE);
			CloseHandle(m_hWorkerThread[i]);
		}

		m_hWorkerThread.clear();
		CloseHandle(completionPort);
		completionPort = NULL;
	}
	return true;
}
bool CIOCPThreadPool::InsertQueueItem(WaitCallback waitCallback, void* args)
{
	std::unique_ptr<Finally> finallyObj(new Finally(std::bind(&LeaveCriticalSection, &cs)));
	try
	{
		EnterCriticalSection(&cs);

		CWaitCallback* p_waitCallback = new CWaitCallback(waitCallback, args);
		if (completionPort == NULL) return false;
		if (waitCallback == NULL) return false;
		return PostQueuedCompletionStatus(completionPort, 0, (ULONG_PTR)p_waitCallback, NULL);
	}
	catch (...)
	{
		return false;
	}
	return true;
}
int CIOCPThreadPool::Run()
{
	DWORD numberOfBytes = 0;
	ULONG_PTR callback = 0;
	LPOVERLAPPED pOverlapped = 0;

	while (true)
	{
		if (!GetQueuedCompletionStatus(completionPort, &numberOfBytes, &callback, &pOverlapped, INFINITE))
		{
			break;
		}
		if ((int)callback == CLOSE_THREAD) break;

		CWaitCallback* pCallback = reinterpret_cast<CWaitCallback*>(callback);
		if (pCallback != NULL)
		{
			pCallback->Run();
			DeleteItem(pCallback);
		}
		else
			continue;
	}
	return 0;
}
bool CIOCPThreadPool::DeleteItem(CWaitCallback* waitCallback)
{
	if (waitCallback)
	{
		delete waitCallback;
		waitCallback = NULL;
		return true;
	}
	return false;
}
unsigned int __stdcall CIOCPThreadPool::WorkerThread(void* p_obj)
{
	CIOCPThreadPool* p_th = static_cast<CIOCPThreadPool*>(p_obj);
	return p_th->Run();
}
