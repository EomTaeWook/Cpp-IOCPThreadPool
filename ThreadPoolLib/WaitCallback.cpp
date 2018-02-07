#include "WaitCallback.h"

namespace Threading
{
	CWaitCallback::CWaitCallback(WaitCallback callback, void* obj)
	{
		this->_func = callback;
		this->_obj = obj;
	}
	CWaitCallback::~CWaitCallback()
	{
	}
}
