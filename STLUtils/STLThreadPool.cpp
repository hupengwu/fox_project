#include "STLThreadPool.h"

STLThreadPool::STLThreadPool()
{
}

STLThreadPool::~STLThreadPool()
{    
}

STLThreadPool* STLThreadPool::newThreadPool(int nThreads)
{
    // 实例化一个空的线程池对象
    STLThreadPool* threadPool = new STLThreadPool();

    // 初始化标记
    threadPool->nFinished = 0;
    threadPool->isExit = false;

    // 创建并启动线程
    for (int i = 0; i < nThreads; i++)
    {
        thread* ptrThread = new thread(executThreadFun, ref(*threadPool));
        threadPool->threads.push_back(ptrThread);
    }    

    return threadPool;
}

void STLThreadPool::shutdownThreadPool(STLThreadPool* &threadPool)
{   
    if (threadPool==nullptr)
    {
        return;
    }

    threadPool->setExit(true);

    for (list<thread*>::iterator it = threadPool->threads.begin(); it != threadPool->threads.end(); it++)
    {
        // 发出一个信号
        threadPool->semaphore.signal();
    }

    // 检查：全体线程是否运行结束
    while (!threadPool->getFinished())
    {
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    // 删除idles
    for (list<STLRunnable*>::iterator it = threadPool->runnables.begin(); it != threadPool->runnables.end(); it++)
    {
        delete *it;
    }
    threadPool->runnables.clear();

    // 回收threads
    for (list<thread*>::iterator it = threadPool->threads.begin(); it != threadPool->threads.end(); it++)
    {
        thread* thread = *it;

        // 回收线程
        if (thread->joinable())
        {
            thread->join();
        }
    }

    // 删除threads
    for (list<thread*>::iterator it = threadPool->threads.begin(); it != threadPool->threads.end(); it++)
    {
        delete* it;
    }   
    threadPool->threads.clear();

    // 清除标识
    threadPool->nFinished = 0;
    threadPool->isExit = false;    

    // 重置信号灯
    threadPool->semaphore.reset();

    // 删除线程池
    threadPool = nullptr;
}

void STLThreadPool::setFinished()
{
    lock_guard<mutex> guard(this->lock);
    this->nFinished++;
}

bool STLThreadPool::getFinished()
{
    lock_guard<mutex> guard(this->lock);
    return this->nFinished >= this->threads.size();
}

void STLThreadPool::setExit(bool isExit)
{
    lock_guard<mutex> guard(this->lock);
    this->isExit = isExit;
}

bool STLThreadPool::getExit()
{
    lock_guard<mutex> guard(this->lock);
    return this->isExit;
}

void STLThreadPool::execute(STLRunnable* runnable)
{
    lock_guard<mutex> guard(this->lock);

    // 通知一个闲置的线程去取
    this->runnables.push_back(runnable);
    this->semaphore.signal(); 

    // 后面其中一个闲置线程会被激活并调用getIdleRunnable()，取走待执行的Runnable
}

STLRunnable* STLThreadPool::getRunnable()
{
    lock_guard<mutex> guard(this->lock);

    // 检查：有没有闲置的runnable
    if (this->runnables.empty())
    {
        return nullptr;
    }

    // 将闲置的runnable，转移到运行的runnable列表上
    STLRunnable* runnal = this->runnables.front();
    this->runnables.pop_front();

    return runnal;
}

void STLThreadPool::executThreadFun(STLThreadPool& threadPool)
{
    while (true)
    {
        // 等待调度该线程的信号到达（每个信号都执行一个被新安排的runnable或者是退出）
        threadPool.semaphore.wait();

        // 是否为退出标记
        if (threadPool.isExit)
        {
            break;
        }
        else
        {
            // 取出一个待执行的runnable
            STLRunnable* runnable = threadPool.getRunnable();
            if (runnable != nullptr)
            {
                // 执行该runnable
                runnable->run();
            }
        }
    }

    // 标识一个线程运行结束
    threadPool.setFinished();
}
