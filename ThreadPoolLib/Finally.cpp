#include "Finally.h"
//�ۼ��� ���¿�
//C# Java ����� try catch finally ó�� �����ϱ� ����
//�Լ��� ����� �Ҹ��ڸ� ����ϴ� ����Ʈ �����ͷ� Finally ���� ����
Finally::Finally(std::function<void(void)> pFunc)
{
	this->_func = pFunc;
}
Finally::~Finally()
{
	if (_func != nullptr) _func();
}