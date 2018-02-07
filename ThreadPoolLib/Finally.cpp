#include "Finally.h"
Finally::Finally(std::function<void(void)> func)
{
	this->_func = func;
}
Finally::~Finally()
{
	if (_func != nullptr) _func();
}