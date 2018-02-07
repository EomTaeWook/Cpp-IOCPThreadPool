#pragma once
#include<functional>
namespace Threading
{
	typedef std::function<void(void*)> WaitCallback;
	class CWaitCallback
	{
		friend class CIOCPThreadPool;
	private:
		std::function<void(void*)> func;
		void* p_obj;
	private:
		void Run();
	public:
		CWaitCallback(WaitCallback callback, void* p_obj = NULL);
		~CWaitCallback();
	};
	inline void CWaitCallback::Run()
	{
		this->func(this->p_obj);
	}
}
