#include "FoxSysTimeUtils.h"

bool FoxSysTimeUtils::getTimeOfDay(timeval& tv, timezone& tz)
{
	return ::gettimeofday(&tv, &tz) == 0;
}

bool FoxSysTimeUtils::getTimeOfDay(timeval& tv)
{
	return ::gettimeofday(&tv, nullptr) == 0;
}

timeval FoxSysTimeUtils::getTimeOfDay()
{
	timeval tv;
	gettimeofday(&tv, nullptr);
	return tv;
}

long long FoxSysTimeUtils::diff(timeval& end, timeval& start)
{
	long long diff = (end.tv_sec - start.tv_sec) * (long long)1000000 + (long long)(end.tv_usec - start.tv_usec);
	return diff;
}

void FoxSysTimeUtils::diff(timeval& end, timeval& start, timeval& diff)
{
	long long diffv = FoxSysTimeUtils::diff(end, start);
	diff.tv_sec = (long)(diffv / 1000000);
	diff.tv_usec = (long)(diffv % 1000000);
}

void FoxSysTimeUtils::diff(timeval& end, timeval& start, long long& diff)
{
	diff = FoxSysTimeUtils::diff(end, start);
}

void FoxSysTimeUtils::diff(timeval& end, timeval& start, long& sec, long& usec)
{
	long long diffv = FoxSysTimeUtils::diff(end, start);
	sec = (long)(diffv / 1000000);
	usec = (long)(diffv % 1000000);
}
