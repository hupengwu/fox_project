
#include "STLThreadPool.h"
#include "STLDemoRunnable.h"

int main()
{
	// 1.����5�̵߳��̳߳�
	STLThreadPool* threadPool = STLThreadPool::newThreadPool(100);

	this_thread::sleep_for(chrono::milliseconds(1000 * 1));

	// 3.ִ��30������
	for (size_t i = 0; i < 3000; i++)
	{
		threadPool->execute(new STLDemoRunnable());
	}
	
	this_thread::sleep_for(chrono::milliseconds(1000*100));

	// �ر��̳߳�
	STLThreadPool::shutdownThreadPool(threadPool);

	this_thread::sleep_for(chrono::milliseconds(1000 * 1000));

}