#pragma once

#include <time.h>
#include <string>

/*
* ʱ�亯���ļ򵥷�װ
* https://www.cnblogs.com/jingzhishen/p/3583175.html
* ����֪ʶ��
* ʱ���ʽ
* 1.��1970�굽���ھ����˶����� ��time_t���ͣ�
* 2.��һ���ṹ���ֱ�洢������ʱ���� ��tm�ṹ�壩
* 3.tm�ṹ��year����1900��ʼ����ģ�ת���ɿɶ������Ҫ+1900���·�����0����ģ�ת���ɿɶ����·�Ҫ+1
* 
* struct tm  
* {
*     int tm_sec; // �룬������Χ0-59�� ��������61
*	  int tm_min; // ���ӣ�0-59
*	  int tm_hour; //Сʱ�� 0-23
*	  int tm_mday; //�գ���һ�����еĵڼ��죬1-31
*	  int tm_mon;  //�£� ��һ������0-11  // 1 + p->tm_mon;
*	  int tm_year; //�꣬ ��1900�����Ѿ�������//  1900�� p->tm_year;
*	  int tm_wday; //���ڣ�һ���еĵڼ��죬������������0-6
*	  int tm_yday; //�ӽ���1��1�յ�Ŀǰ����������Χ0-365
*	  int tm_isdst;
* };
*/

class FoxTimeUtils
{
public:// ��ȡʱ��
	/*
	* ��ȡ��ǰϵͳtime_tʱ�䣬�䷵��ֵ����1970��������ֵ��
	*/
	static void getTime(time_t& rawtime);
	static time_t getTime();

	/*
	* ��time_tʱ��ת����tmʱ�䣬����ֵ��time_t�ṹ��ָ�룬����ֵ��tm���͵�ָ�롣
	*/
	static tm* getLocalTime(const time_t& timer);// ����ʱ��ת����UTCʱ��
	static tm* getLocalTime();
	static tm* getGmTime(const time_t& timep);// û�о���ʱ��ת����UTCʱ��

public:// ת��ʱ��
	/*
	* ת��tm��ʽ��ʱ��
	*/
	static tm makeTm(int year, int month, int day, int hour, int min, int sec);

	/*
	* ��tmʱ��ת��Ϊtime_tʱ�䣬����ֵ��tm���͵ĵ�ַ������ֵ��time_t���͡�
	*/
	static time_t makeTime(const tm& tmTime);
	static time_t makeTime(const int& year, const int& month, const int& day, const int& hour, const int& min, const int& sec);

public:// ʱ���
	/*
	* ����ʱ����������
	*/
	static double diffTime(const time_t& time1, const time_t& time2);

public:// ��ʽ��ʱ���ַ���
	/*
	* ��timeptrָ���tm�ṹ���д����ʱ��ת��Ϊ�ַ��������ص��ַ�����ʽΪ��Mmm dd hh:mm:ss yyyy�����磺Nov 15 10:30:30 2013
	*/
	static const char* asctime(const tm* timeptr);
	static const char* asctime(const tm& timer);
	static const char* asctime(const time_t& timer);
	static const char* asctime();

	/*
	*��timeptrָ���tm�ṹ���д����ʱ��ת��Ϊ�ַ��������ص��ַ�����ʽΪ��Www Mmm dd hh:mm:ss yyyy�����磺Fri Nov 15 10:30:30 2013
	*/
	static const char* ctime(const time_t* timep);
	static const char* ctime(const time_t& timep);
};

