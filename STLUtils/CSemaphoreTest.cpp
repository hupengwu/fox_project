#include "CSemaphoreTest.h"

#include "STLSemaphore.h"
#include <iostream>


const int BufferSize = 12;
const int DataSize = 50;

int buff[BufferSize];
STLSemaphore freeSpace(BufferSize);//写入线程
STLSemaphore usedSpace(-10);//消费线程

std::mutex m;

void producer()
{
	for (int i = 0; i < DataSize; i++)
	{
		freeSpace.wait();

		std::thread::id thread_id = std::this_thread::get_id();
		buff[i % BufferSize] = i;
		printf("Thread %d,producer %d\n", thread_id, i);

		usedSpace.signal();
		this_thread::sleep_for(chrono::milliseconds(1000));
	}
}

void consumer()
{
	for (int i = 0; i < DataSize; i++)
	{
		usedSpace.wait();

		std::thread::id thread_id = std::this_thread::get_id();
		printf("Thread %d, consumer %d\n", thread_id, buff[i % BufferSize]);

		freeSpace.signal();
	}
}

int CSemaphoreTest::test()
{
	std::thread threads1(producer);
	std::thread threads2(consumer);

	threads1.join();
	threads2.join();
	std::cout << "finish!" << std::endl;

	std::cin.get();
	return 0;
}

