#pragma once
#include<functional>
//작성자 엄태욱
//언어의 try catch finally 처럼 실행하기 위해
//함수를 벗어날때 소멸자를 사용하는 스마트 포인터로 Finally 구문 실행
class Finally
{
private:
	std::function<void(void)> _func;
public:
	Finally(std::function<void(void)> func =nullptr);
	~Finally();
};
