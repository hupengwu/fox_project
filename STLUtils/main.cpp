
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


	// 1.����5�̵߳��̳߳�
	STLAsyncTask asyncTask;
	asyncTask.start(100);

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