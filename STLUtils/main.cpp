
#include "STLAsyncTask.h"
#include "STLDemoRunnable.h"
#include "STLTimedTask.h"

int main()
{
	STLTimedTask time(new STLDemoRunnable());
	time.start(1000);
	this_thread::sleep_for(chrono::milliseconds(1000 * 60));
	time.close();

	this_thread::sleep_for(chrono::milliseconds(1000 * 10));


	// 1.创建5线程的线程池
	STLAsyncTask asyncTask;
	asyncTask.start(100);

	this_thread::sleep_for(chrono::milliseconds(1000 * 1));

	// 3.执行30个任务
	for (size_t i = 0; i < 3000; i++)
	{
		asyncTask.execute(new STLDemoRunnable());
	}
	
	this_thread::sleep_for(chrono::milliseconds(1000*60));

	// 关闭线程池
	asyncTask.close();

	this_thread::sleep_for(chrono::milliseconds(1000 * 1000));

}