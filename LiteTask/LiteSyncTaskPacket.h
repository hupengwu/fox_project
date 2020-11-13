#pragma once

#include <list>
#include <mutex>

using namespace std;


enum TaskResult { Finish, Empty};

/**
 * 轻量级的多线程任务包:使用多线程并行处理来解决一批任务的高强度计算的问题<br>
 * 多线程任务包：任务包裹用于放置多线程处理的输入/输出数据<br>
 * @author h00163887
 * @since 2019/09/21
 */
class LiteSyncTaskPacket
{
public:
	/**
	 * 执行任务:注意，涉及被多线程访问的属性，都要加锁<br>
	 * @param content 单个任务对象
	 */
	virtual TaskResult executFunctionOneTask() = 0;

public:
	/**
	 * 设置需要处理的一组任务集合
	 * @param contents 任务列表
	 */
	template<typename TASK>
	void setTaskList(list<TASK>& contents) 
	{
		lock_guard<mutex> guard(lock);

		for (TASK task : contents) {
			// 在堆内存创建一个副本
			TASK* clonePtr = new TASK(task);
			this->tasks.push_back(clonePtr);
		}
	}


	/**
	 * 取出一个任务
	 * @return 任务对象
	 */
	template<typename TASK>
	void popTask(list<TASK>& tasks)
	{
		tasks.clear();

		lock_guard<mutex> guard(lock);

		if (this->tasks.size()==0) {
			return;
		}

		// 对象的类型转换
		TASK* clonePtr = (TASK*)this->tasks.front();

		// 将值传递给外部
		tasks.push_back(*clonePtr);

		// 释放资源：列表中的指针和对应的对象
		this->tasks.pop_front();
		delete clonePtr;
	}	

private:
	mutex lock;// 互斥  
		
	list<void*> tasks;// 需要处理的任务数据
};

