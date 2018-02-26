#pragma once
#include<functional>
namespace Threading
{
	typedef std::function<void(void*)> Func;
	class WaitCallback
	{
		friend class IOCPThreadPool;
	private:
		std::function<void(void*)> _func;
		void* _obj;
	private:
		void Run();
	public:
		WaitCallback(Func callback, void* obj = NULL);
		~WaitCallback() {}
	};

	inline void WaitCallback::Run()
	{
		this->_func(this->_obj);
	}
}
