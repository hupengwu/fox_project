#include "FoxLoggerFactory.h"

int main()
{
	ILogger* logger = FoxLoggerFactory::getLogger();
	logger->debug("test");


	FoxLoggerFactory::freeLogger();
	return 0;
}