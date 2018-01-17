#include "Finally.h"

Finally::Finally(std::function<void(void)> pFunc)
{
	this->pFunc = pFunc;
}
Finally::~Finally()
{
	if (pFunc != nullptr) pFunc();
}