#include "LiteSyncTaskWorker.h"

LiteSyncTaskWorker::LiteSyncTaskWorker(LiteSyncTaskPacket& taskPacketObj, int nThreads)
{
    this->taskPacket = &taskPacketObj;
    this->nThreads = nThreads;
    this->nFinished = 0;
}

LiteSyncTaskWorker::~LiteSyncTaskWorker()
{
}

void LiteSyncTaskWorker::executThread(LiteSyncTaskPacket& taskPackage, LiteSyncTaskWorker& taskWorker, int threadId)
{
    while (true) 
    {
        // 执行任务
        TaskResult result = taskPackage.executFunctionOneTask();

        // 每个线程碰到Task被取完了，那么线程该结束了
        if (result == TaskResult::Empty)
        {
            taskWorker.increFinished();
            return;
        }        
    }
}

void LiteSyncTaskWorker::increFinished()
{
    lock_guard<mutex> guard(this->lock);
    this->nFinished++;
}

void LiteSyncTaskWorker::resetFinished()
{
    lock_guard<mutex> guard(this->lock);
    this->nFinished = 0;
}

bool LiteSyncTaskWorker::isFinished()
{
    lock_guard<mutex> guard(this->lock);
    return this->nFinished >= this->nThreads;
}

bool LiteSyncTaskWorker::executeTask(LiteSyncTaskPacket& packet)
{
    // 重置线程的结束状态计数器
    this->resetFinished();

    // 创建并启动线程
    vector<thread*> threads;
    for (int i = 0; i < this->nThreads; i++)
    {
        // 在堆内存中创建线程对象:将自身指针传递进函数
        thread* ptrThread = new thread(executThread, ref(packet), ref(*this), i);

        // 保存线程        
        threads.push_back(ptrThread);
    }
    
    // 等待全体线程处理完毕
    while (!this->isFinished()) 
    {
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    // 回收线程资源，防止泄漏
    for (thread* threadPtr : threads)
    {
        if (threadPtr->joinable())
        {
            threadPtr->join();
        }

        delete threadPtr;
        threadPtr = nullptr;
    }

    // 清空列表
    threads.clear();

    return true;
}







