
#include "STLAsyncTask.h"
#include "STLDemoRunnable.h"
#include "STLTimedTask.h"
#include "STLStringUtils.h"
#include "STLKVMapper.h"
#include "STLKVDemoIterator.h"

int main()
{
	STLByteArray bytes;
	
	string ttt = "f1 F2 F3 F4";
	STLStringUtils::remove_if(ttt,' ');

	STLStringUtils::str2bytes("f1 F2 F3 F4", bytes);
	BYTE i = bytes.getAt(0);
	i = bytes.getAt(1);
	i = bytes.getAt(2);
	i = bytes.getAt(3);
	i = 241;
	string tt;
	STLStringUtils::bytes2str(bytes,tt);

	STLKVMapper<string, string> k2vmaper;
	std::map<string, string > k2v;
	k2v["1"] = "one";
	k2v["2"] = "two";
	k2v["3"] = "three";

	
	k2vmaper.swap(k2v);

	set<string> keys;
	keys.insert("1");
	keys.insert("2");

	bool r = k2vmaper.hasKey("1");
//	k2vmaper.getKeys(keys);
	STLKVDemoIterator it;
	k2vmaper.foreach(it);
	k2vmaper.foreach(keys,it);

	list<string> list;
	STLStringUtils::split("h,h,5,6",",", list);
	vector<string> vec;
	STLStringUtils::split("h,h,5,6", ",", vec);

	STLTimedTask getTime(new STLDemoRunnable());
	getTime.create(1000);
	this_thread::sleep_for(chrono::milliseconds(1000 * 60));
	getTime.close();

	this_thread::sleep_for(chrono::milliseconds(1000 * 10));


	// 1.创建5线程的线程池
	STLAsyncTask threads;
	threads.create(100);

	this_thread::sleep_for(chrono::milliseconds(1000 * 1));

	// 3.执行30个任务
	for (size_t i = 0; i < 3000; i++)
	{
		threads.execute(new STLDemoRunnable());
	}
	
	this_thread::sleep_for(chrono::milliseconds(1000*60));

	// 关闭线程池
	threads.close();

	this_thread::sleep_for(chrono::milliseconds(1000 * 1000));

}