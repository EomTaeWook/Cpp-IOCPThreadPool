#include "IOCPThreadPool.h"
#include "Finally.h"
#include <memory>

namespace Threading
{
	bool CIOCPThreadPool::Init(unsigned int threadMaxSize)
	{
		try
		{
			if (!Stop())
			{
				return false;
			}
			_completionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
			if (_completionPort == INVALID_HANDLE_VALUE)
				return false;

			SYSTEM_INFO info;
			GetSystemInfo(&info);

			if (threadMaxSize > 0)
			{
				if (info.dwNumberOfProcessors * 2 < threadMaxSize)
					_thread_Max_Size = info.dwNumberOfProcessors * 2;
				else
					_thread_Max_Size = threadMaxSize;
			}
			else
			{
				_thread_Max_Size = info.dwNumberOfProcessors * 2;
			}
			for (unsigned int i = 0; i < _thread_Max_Size; i++)
			{
				_hWorkerThread.push_back((HANDLE)_beginthreadex(0, 0, WorkerThread, this, 0, NULL));
			}
		}
		catch (...)
		{
			return false;
		}
		return true;
	}
	bool CIOCPThreadPool::Stop()
	{
		if (_completionPort)
		{
			for (int i = 0; i < _hWorkerThread.size(); i++)
				PostQueuedCompletionStatus(_completionPort, 0, CLOSE_THREAD, NULL);

			for (int i = 0; i < _hWorkerThread.size(); i++)
			{
				WaitForSingleObject(_hWorkerThread[i], INFINITE);
				CloseHandle(_hWorkerThread[i]);
			}

			_hWorkerThread.clear();
			CloseHandle(_completionPort);
			_completionPort = NULL;
		}
		return true;
	}
	bool CIOCPThreadPool::InsertQueueItem(WaitCallback waitCallback, void* args)
	{
		if (_completionPort == NULL) return false;
		std::unique_ptr<Finally> finally(new Finally(std::bind(&LeaveCriticalSection, &_cs)));
		try
		{
			EnterCriticalSection(&_cs);

			CWaitCallback* p_waitCallback = new CWaitCallback(waitCallback, args);
			if (waitCallback == NULL) return false;
			return PostQueuedCompletionStatus(_completionPort, 0, (ULONG_PTR)p_waitCallback, NULL);
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
			if (!GetQueuedCompletionStatus(_completionPort, &numberOfBytes, &callback, &pOverlapped, INFINITE))
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
unsigned int __stdcall CIOCPThreadPool::WorkerThread(void* obj)
{
	CIOCPThreadPool* p_th = static_cast<CIOCPThreadPool*>(obj);
	return p_th->Run();
}