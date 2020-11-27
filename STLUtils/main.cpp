
#include "STLAsyncTask.h"
#include "STLDemoRunnable.h"
#include "STLTimedTask.h"
#include "STLStringUtils.h"

int main()
{
	list<string> list;
	STLStringUtils::split("h,h,5,6",",", list);
	vector<string> vec;
	STLStringUtils::split("h,h,5,6", ",", vec);

	STLTimedTask time(new STLDemoRunnable());
	time.create(1000);
	this_thread::sleep_for(chrono::milliseconds(1000 * 60));
	time.close();

	this_thread::sleep_for(chrono::milliseconds(1000 * 10));


	// 1.创建5线程的线程池
	STLAsyncTask asyncTask;
	asyncTask.create(100);

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