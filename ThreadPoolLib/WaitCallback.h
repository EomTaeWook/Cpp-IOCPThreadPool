#pragma once
#include<functional>
namespace Threading
{
	typedef std::function<void(void*)> WaitCallback;
	class CWaitCallback
	{
		friend class CIOCPThreadPool;
	private:
		std::function<void(void*)> _func;
		void* _obj;
	private:
		void Run();
	public:
		CWaitCallback(WaitCallback callback, void* obj = NULL);
		~CWaitCallback();
	};
	inline void CWaitCallback::Run()
	{
		this->_func(this->_obj);
	}
}
