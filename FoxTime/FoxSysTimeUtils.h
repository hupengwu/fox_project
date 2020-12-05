#pragma once

#include <sys/time.h>

/*
* ��sys/time�µĺ����򵥷�װ
*/
class FoxSysTimeUtils
{
public:
	/*
	* ��õ�ǰ��ȷʱ�䣨1970��1��1�յ����ڵ�ʱ�䣩����ȷ������
	*/
	static bool getTimeOfDay(timeval& tv, timezone& tz);
	static bool getTimeOfDay(timeval& tv);
	static timeval getTimeOfDay();

public:
	/*
	* ʱ��:��΢��Ϊ����
	*/
	static long long diff(timeval& end, timeval& start);
	static void diff(timeval& end, timeval& start, timeval& diff);
	static void diff(timeval& end, timeval& start, long long& diff);
	static void diff(timeval& end, timeval& start, long& sec, long& usec);
};

