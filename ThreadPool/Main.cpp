// ThreadPool.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "../ThreadPoolLib/IOCPThreadPool.h"

bool stop1 = true;
bool stop2 = true;
bool stop3 = true;
bool stop4 = true;
bool arr[10000] = { false };
void Sum(void *p)
{
	auto value = static_cast<int*>(p);
	printf("Stop 1 %d\n", *value);
	stop1 = false;
}
void Sum1(void *p)
{
	auto value = static_cast<int*>(p);
	printf("Stop 2 %d\n", *value);
	stop2 = false;
}
void Sum2(void *p)
{
	auto value = static_cast<int*>(p);
	printf("Stop 3 %d\n", *value);
	stop3 = false;
	Sleep(1);
}
void Sum24(void *p)
{
	auto value = static_cast<double*>(p);
	printf("Stop 4 %lf\n", *value);
	stop4 = false;
}

void Check(void *p)
{
	auto value = static_cast<int*>(p);
	int num = *value;
	printf("Check %d\n", num);

	arr[num] = true;
	delete p;
	Sleep(1);
}


struct Test
{
	int test;
};
int main()
{
	Threading::IOCPThreadPool threadpool;
	threadpool.Init(3);

	//singleton
	Threading::IOCPThreadPool::Instance()->Init();

	int a = 100;
	Test t;
	t.test = 200;

	Test t1;
	t1.test = 300;
	threadpool.InsertQueueItem(std::bind(Sum, std::placeholders::_1), &a);
	threadpool.InsertQueueItem(std::bind(Sum1, std::placeholders::_1), &t);
	threadpool.InsertQueueItem(std::bind(Sum2, std::placeholders::_1), &t1);
	double test = 10000;
	threadpool.InsertQueueItem(std::bind(Sum24, std::placeholders::_1), &test);

	for (int i = 0; i < 10000; i++)
	{
		auto val = new int(i);
		threadpool.InsertQueueItem(std::bind(Sum2, std::placeholders::_1), val);
		threadpool.InsertQueueItem(std::bind(Check, std::placeholders::_1), val);
	}
	getchar();
    return 0;
}

