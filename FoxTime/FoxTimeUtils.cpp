#include "FoxTimeUtils.h"

void FoxTimeUtils::getTime(time_t& rawtime)
{
	::time(&rawtime);
}

time_t FoxTimeUtils::getTime()
{
	return ::time(nullptr);
}

tm* FoxTimeUtils::getLocalTime(const time_t& timer)
{
	return ::localtime(&timer);
}

tm* FoxTimeUtils::getLocalTime()
{
	time_t rawtime = getTime();
	return getLocalTime(rawtime);
}

tm* FoxTimeUtils::getGmTime(const time_t& timep)
{
	return ::gmtime(&timep);
}

tm FoxTimeUtils::makeTm(int year, int month, int day, int hour, int min, int sec)
{
	tm tmTime;
	tmTime.tm_year = year - 1900;
	tmTime.tm_mon = month - 1;
	tmTime.tm_mday = day;
	tmTime.tm_hour = hour;
	tmTime.tm_min = min;
	tmTime.tm_sec = sec;

	return tmTime;
}

time_t FoxTimeUtils::makeTime(const tm& tmTime)
{
	return ::mktime((tm*)(&tmTime));
}

time_t FoxTimeUtils::makeTime(const int& year, const int& month, const int& day, const int& hour, const int& min, const int& sec)
{
	tm tm = makeTm(year, month, day, hour, min, sec);
	return makeTime(tm);
}

double FoxTimeUtils::diffTime(const time_t& time1, const time_t& time2)
{
	return ::difftime(time1, time2);
}

const char* FoxTimeUtils::asctime(const tm* timeptr)
{
	return ::asctime(timeptr);
}

const char* FoxTimeUtils::asctime(const tm& timer)
{
	return asctime(&timer);
}

const char* FoxTimeUtils::asctime(const time_t& timer)
{
	tm* tmptr = getLocalTime(timer);
	return asctime(tmptr);
}

const char* FoxTimeUtils::asctime()
{
	time_t getTime = FoxTimeUtils::getTime();
	return asctime(getTime);
}

const char* FoxTimeUtils::ctime(const time_t* timep)
{
	return ::ctime((time_t*)timep);
}

const char* FoxTimeUtils::ctime(const time_t& timep)
{
	return ctime(&timep);
}
