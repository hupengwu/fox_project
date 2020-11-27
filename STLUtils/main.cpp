
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


	// 1.����5�̵߳��̳߳�
	STLAsyncTask asyncTask;
	asyncTask.create(100);

	this_thread::sleep_for(chrono::milliseconds(1000 * 1));

	// 3.ִ��30������
	for (size_t i = 0; i < 3000; i++)
	{
		asyncTask.execute(new STLDemoRunnable());
	}
	
	this_thread::sleep_for(chrono::milliseconds(1000*60));

	// �ر��̳߳�
	asyncTask.close();

	this_thread::sleep_for(chrono::milliseconds(1000 * 1000));

}