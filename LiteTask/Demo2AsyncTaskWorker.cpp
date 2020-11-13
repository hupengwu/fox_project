#include "Demo2AsyncTaskWorker.h"

#include <list>
void Demo2AsyncTaskWorker::executThreadFunc()
{
	std::thread::id thread_id = std::this_thread::get_id();
	printf("Thread %d\n", thread_id);

	list<int> lis;
	//lis.pop_front

	this_thread::sleep_for(chrono::milliseconds(1000*10));
}
