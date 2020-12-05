#pragma once

#include <sys/time.h>

/*
* 对sys/time下的函数简单封装
*/
class FoxSysTimeUtils
{
public:
	/*
	* 获得当前精确时间（1970年1月1日到现在的时间）：精确到毫秒
	*/
	static bool getTimeOfDay(timeval& tv, timezone& tz);
	static bool getTimeOfDay(timeval& tv);
	static timeval getTimeOfDay();

public:
	/*
	* 时差:以微秒为精度
	*/
	static long long diff(timeval& end, timeval& start);
	static void diff(timeval& end, timeval& start, timeval& diff);
	static void diff(timeval& end, timeval& start, long long& diff);
	static void diff(timeval& end, timeval& start, long& sec, long& usec);
};

