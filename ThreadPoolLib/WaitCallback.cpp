#include "WaitCallback.h"

namespace Threading
{
	WaitCallback::WaitCallback(Func callback, void* obj)
	{
		this->_func = callback;
		this->_obj = obj;
	}
}
