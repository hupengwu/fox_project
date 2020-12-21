#include "FoxRestfulAsyncTasks.h"
#include <UUIDUtils.h>
#include <FoxJSonObject.h>

FoxRestfulAsyncTasks::FoxRestfulAsyncTasks()
{
	maxSize = 10;
}

FoxRestfulAsyncTasks::~FoxRestfulAsyncTasks()
{
}

bool FoxRestfulAsyncTasks::pushTask(FoxRestfulAsyncTask& task)
{
	lock_guard<mutex> guard(this->lock);	
	
	if (this->id2task.size() >= this->maxSize)
	{
		return false;
	}

	// 分配ID
	task.taskId = UUIDUtils::generateUUID();

	// 标识：新任务和请求时间
	task.status = fox::status_new;
	task.requestTime = ::time(nullptr);

	this->id2task[task.taskId] = task;
	return true;
}

bool FoxRestfulAsyncTasks::processTask(FoxRestfulAsyncTask& task)
{
	lock_guard<mutex> guard(this->lock);

	map<string, FoxRestfulAsyncTask>::iterator it = this->id2task.begin();
	if (it == this->id2task.end())
	{
		return false;
	}

	// 标识为已经被处理
	it->second.status = fox::status_processing;
	task.respondTime = ::time(nullptr);

	task = it->second;
	return true;
}

bool FoxRestfulAsyncTasks::finishTask(FoxRestfulAsyncTask& task)
{
	lock_guard<mutex> guard(this->lock);

	map<string, FoxRestfulAsyncTask>::iterator it = this->id2task.find(task.taskId);
	if (it == this->id2task.end())
	{
		return false;
	}

	// 标识:任务完成
	FoxRestfulAsyncTask& taskAt = it->second;	
	taskAt.status = fox::status_finish;	
	taskAt.respondTime = ::time(nullptr);
	taskAt.respond = task.respond;

	return true;
}

void FoxRestfulAsyncTasks::removeTask(FoxRestfulAsyncTask& task)
{
	lock_guard<mutex> guard(this->lock);

	map<string, FoxRestfulAsyncTask>::iterator it = this->id2task.find(task.taskId);
	if (it == this->id2task.end())
	{
		task.status = fox::status_not_exist;
		return;
	}

	FoxRestfulAsyncTask& taskAt = it->second;
	if (taskAt.status != fox::status_finish)
	{
		task = taskAt;
		return;
	}

	task = taskAt;
	this->id2task.erase(it->first);
}

void FoxRestfulAsyncTasks::queryTasksByObjectId(const string& objectId, list<FoxRestfulAsyncTask>& tasks)
{
	lock_guard<mutex> guard(this->lock);

	for (map<string, FoxRestfulAsyncTask>::iterator it = this->id2task.begin(); it != this->id2task.end(); it++)
	{
		FoxRestfulAsyncTask& taskAt = it->second;
		if (taskAt.objectId == objectId)
		{
			tasks.push_back(taskAt);
			continue;
		}
	}
}

void FoxRestfulAsyncTasks::cleanOverdue()
{
	lock_guard<mutex> guard(this->lock);

	list<string> keys;
	for (map<string, FoxRestfulAsyncTask>::iterator it = this->id2task.begin(); it != this->id2task.end(); it++) 
	{
		FoxRestfulAsyncTask& taskAt = it->second;
		long span = taskAt.respondTime - taskAt.requestTime;
		if (span < taskAt.lifeCycle)
		{
			continue;
		}
		
		if (taskAt.status == fox::status_finish)
		{
			keys.push_back(it->first);
		}
	}

	for (string key : keys)
	{
		this->id2task.erase(key);
	}
}

void FoxRestfulAsyncTasks::setMaxSize(size_t maxSize)
{
	lock_guard<mutex> guard(this->lock);

	this->maxSize = maxSize;
}

string FoxRestfulAsyncTasks::toJson(const FoxRestfulAsyncTask& task)
{
	FoxJSonObject oJsn;
	oJsn.Add("task_id", task.taskId);
	oJsn.Add("object_id", task.objectId);
	oJsn.Add("request", task.request);
	oJsn.Add("respond", task.respond);
	oJsn.Add("request", task.request);
	oJsn.Add("status", task.status);
	oJsn.Add("request_time", (uint64)task.requestTime);
	oJsn.Add("respond_time", (uint64)task.respondTime);
	oJsn.Add("life_cycle", (uint64)task.lifeCycle);

	return oJsn.ToFormattedString();
}