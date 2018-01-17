#include "WaitCallback.h"

CWaitCallback::CWaitCallback(WaitCallback callback, void* p_obj)
{
	this->func = callback;
	this->p_obj = p_obj;
}
CWaitCallback::~CWaitCallback()
{
}
