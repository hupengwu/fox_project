#include "Log4jLogger.h"

Log4jLogger* Log4jLogger::logger = nullptr;

Log4jLogger::Log4jLogger()
{
}

Log4jLogger::~Log4jLogger()
{
}

ILogger* Log4jLogger::getLogger()
{
	if (logger == nullptr)
	{
		logger = new Log4jLogger();
	}

	return logger;
}

void Log4jLogger::freeLogger()
{
	if (logger != nullptr)
	{
	    delete logger;
		logger = nullptr;
	}
}
void Log4jLogger::debug(string message)
{
	printLine(this_thread::get_id(), "debug", message);
}

void Log4jLogger::info(string message)
{
	printLine(this_thread::get_id(), "info", message);
}

void Log4jLogger::warn(string message)
{
	printLine(this_thread::get_id(), "warn", message);
}

void Log4jLogger::error(string message)
{
	printLine(this_thread::get_id(), "error", message);
}

void Log4jLogger::fatal(string message)
{
	printLine(this_thread::get_id(), "fatal", message);
}

bool Log4jLogger::isDebugEnabled()
{
	return false;
}

void Log4jLogger::printLine(thread::id threadId, string level, string message)
{
	time_t timep;
	::time(&timep);

	struct tm* p = ::localtime(&timep);

	char strTime[24];
	snprintf(strTime, sizeof(strTime), "%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
		(1900 + p->tm_year),(1 + p->tm_mon),p->tm_mday,
		(p->tm_hour),p->tm_min, p->tm_sec
	);

	printf("thread id = [%d], time = [%s], level = [%s] : %s\n", threadId, strTime, level.c_str(), message.c_str());
}
