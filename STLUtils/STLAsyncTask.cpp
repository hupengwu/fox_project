#include "STLAsyncTask.h"

STLAsyncTask::STLAsyncTask()
{
	this->threadPool = nullptr;
}

STLAsyncTask::~STLAsyncTask()
{
}

void STLAsyncTask::create(int nThreads)
{
	this->threadPool =  STLThreadPool::newThreadPool(nThreads);
}

void STLAsyncTask::execute(STLRunnable* runnable)
{
	this->threadPool->execute(runnable);
}

void STLAsyncTask::close()
{
	STLThreadPool::shutdownThreadPool(this->threadPool);
}

bool STLAsyncTask::isBusy()
{
	return this->threadPool->isBusy();
}
