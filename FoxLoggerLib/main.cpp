#include "FoxLoggerFactory.h"

int main()
{
	ILogger* logger = FoxLoggerFactory::getLogger();
	logger->debug("test");

	logger->info("%d, %s", 5, "123");


	FoxLoggerFactory::freeLogger();
	return 0;
}