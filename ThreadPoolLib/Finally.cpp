#include "Finally.h"
//작성자 엄태욱
//C# Java 언어의 try catch finally 처럼 실행하기 위해
//함수를 벗어날때 소멸자를 사용하는 스마트 포인터로 Finally 구문 실행
Finally::Finally(std::function<void(void)> pFunc)
{
	this->pFunc = pFunc;
}
Finally::~Finally()
{
	if (pFunc != nullptr) pFunc();
}