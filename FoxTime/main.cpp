#include "FoxSysTimeUtils.h"
#include "FoxTimeUtils.h"
#include <thread>
using namespace std;

#include <stdio.h>
#include <time.h>

int main()
{
	struct    tm tmTime;
	time_t    timetTime;

	//����һ��ʱ��
	int year = 2013;
	int month = 11;
	int day = 15;
	int hour = 10;
	int min = 30;
	int sec = 30;

	//����tm�ṹ��
	tmTime.tm_year = year - 1900;
	tmTime.tm_mon = month - 1;
	tmTime.tm_mday = day;
	tmTime.tm_hour = hour;
	tmTime.tm_min = min;
	tmTime.tm_sec = sec;

	//tm�ṹת����time_t�ṹ
	timetTime = mktime(&tmTime);
	printf("timetTime=%d\n", timetTime);
	//��ctime����У���£�����ת���Ƿ���ȷ
	printf("After transfer, time is: %s\n", ctime((time_t*)&timetTime));


	tm tm = FoxTimeUtils::makeTm(year, month, day, hour, min, sec);
	printf("%s\n", FoxTimeUtils::asctime(tm));

	time_t s1 = FoxTimeUtils::makeTime(tm);
		 
	printf("timetTime=%d\n", s1);

	const char* ss = ctime((time_t*)&s1);
	ss = FoxTimeUtils::ctime(s1);
	printf("%s\n", FoxTimeUtils::ctime(FoxTimeUtils::makeTime(tm)));
	timeval tv = FoxSysTimeUtils::getTimeOfDay();

	return(0);
}