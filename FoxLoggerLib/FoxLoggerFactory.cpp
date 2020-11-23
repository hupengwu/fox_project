#include "FoxLoggerFactory.h"

FoxLoggerFactory::FoxLoggerFactory()
{
}

FoxLoggerFactory::~FoxLoggerFactory()
{
}

ILogger* FoxLoggerFactory::getLogger()
{
   return Log4jLogger::getLogger();
}

void FoxLoggerFactory::freeLogger()
{
	Log4jLogger::freeLogger();
}
