#pragma once

#include <time.h>
#include <string>

/*
* 时间函数的简单封装
* https://www.cnblogs.com/jingzhishen/p/3583175.html
* 背景知识：
* 时间格式
* 1.从1970年到现在经过了多少秒 （time_t类型）
* 2.用一个结构来分别存储年月日时分秒 （tm结构体）
* 3.tm结构的year是以1900开始计算的，转化成可读的年份要+1900，月份是以0计算的，转化成可读的月份要+1
* 
* struct tm  
* {
*     int tm_sec; // 秒，正常范围0-59， 但允许至61
*	  int tm_min; // 分钟，0-59
*	  int tm_hour; //小时， 0-23
*	  int tm_mday; //日，即一个月中的第几天，1-31
*	  int tm_mon;  //月， 从一月算起，0-11  // 1 + p->tm_mon;
*	  int tm_year; //年， 从1900至今已经多少年//  1900＋ p->tm_year;
*	  int tm_wday; //星期，一周中的第几天，从星期日算起，0-6
*	  int tm_yday; //从今年1月1日到目前的天数，范围0-365
*	  int tm_isdst;
* };
*/

class FoxTimeUtils
{
public:// 获取时间
	/*
	* 获取当前系统time_t时间，其返回值是自1970来的秒数值。
	*/
	static void getTime(time_t& rawtime);
	static time_t getTime();

	/*
	* 将time_t时间转换成tm时间，输入值是time_t结构的指针，返回值是tm类型的指针。
	*/
	static tm* getLocalTime(const time_t& timer);// 经过时区转换的UTC时间
	static tm* getLocalTime();
	static tm* getGmTime(const time_t& timep);// 没有经过时区转换的UTC时间

public:// 转换时间
	/*
	* 转换tm格式的时间
	*/
	static tm makeTm(int year, int month, int day, int hour, int min, int sec);

	/*
	* 将tm时间转换为time_t时间，输入值是tm类型的地址，返回值是time_t类型。
	*/
	static time_t makeTime(const tm& tmTime);
	static time_t makeTime(const int& year, const int& month, const int& day, const int& hour, const int& min, const int& sec);

public:// 时间差
	/*
	* 两个时间相差的秒数
	*/
	static double diffTime(const time_t& time1, const time_t& time2);

public:// 格式化时间字符串
	/*
	* 把timeptr指向的tm结构体中储存的时间转换为字符串，返回的字符串格式为：Mmm dd hh:mm:ss yyyy，例如：Nov 15 10:30:30 2013
	*/
	static const char* asctime(const tm* timeptr);
	static const char* asctime(const tm& timer);
	static const char* asctime(const time_t& timer);
	static const char* asctime();

	/*
	*把timeptr指向的tm结构体中储存的时间转换为字符串，返回的字符串格式为：Www Mmm dd hh:mm:ss yyyy，例如：Fri Nov 15 10:30:30 2013
	*/
	static const char* ctime(const time_t* timep);
	static const char* ctime(const time_t& timep);
};

