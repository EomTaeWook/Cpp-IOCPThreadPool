#pragma once
#include<functional>
//�ۼ��� ���¿�
//C# Java ����� try catch finally ó�� �����ϱ� ����
//�Լ��� ����� �Ҹ��ڸ� ����ϴ� ����Ʈ �����ͷ� Finall ���� ����
class Finally
{
private:
	std::function<void(void)> pFunc;
public:
	Finally(std::function<void(void)> pFunc);
	~Finally();
};