#include "Demo1SyncTaskListPacket.h"

#include <thread>

using namespace std;

TaskResult Demo1SyncTaskListPacket::executFunctionOneTask()
{
    this_thread::sleep_for(chrono::milliseconds(10));

    // 1、取出一个任务
    list<int> tasks;
    this->popTask(tasks);
    if (tasks.size()==0)
    {
        return TaskResult::Empty;
    }

    // 2、处理任务
    std::thread::id thread_id = std::this_thread::get_id();
    int i = tasks.front();
    double result = i * 2.1;
    printf("thread id = %d,task=%d,result=%f\n", thread_id, i, result);

    // 3、带回结果    
    this->addResult(result);


    return TaskResult::Finish;
}
