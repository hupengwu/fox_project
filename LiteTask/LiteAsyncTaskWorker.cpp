#include "LiteAsyncTaskWorker.h"

LiteAsyncTaskWorker::LiteAsyncTaskWorker()
{
}

LiteAsyncTaskWorker::~LiteAsyncTaskWorker()
{
}

void LiteAsyncTaskWorker::reqExit()
{
    lock_guard<mutex> guard(this->m_mutex);
    this->m_reqExitFlag = true;
}

bool LiteAsyncTaskWorker::isExit()
{
    lock_guard<mutex> guard(this->m_mutex);
    return this->m_activedCount == 0;
}

int LiteAsyncTaskWorker::queryActivedCount()
{
    lock_guard<mutex> guard(m_mutex);
    return this->m_activedCount;
}

bool LiteAsyncTaskWorker::queryReqExitFlag()
{
    lock_guard<mutex> guard(m_mutex);
    return this->m_reqExitFlag;
}

void LiteAsyncTaskWorker::resetReqExitFlag()
{
    lock_guard<mutex> guard(m_mutex);
    this->m_reqExitFlag = false;
}

void LiteAsyncTaskWorker::increaseActivedCount()
{
    lock_guard<mutex> guard(m_mutex);
    this->m_activedCount++;
}

void LiteAsyncTaskWorker::decreaseActivedCount()
{
    lock_guard<mutex> guard(m_mutex);
    this->m_activedCount--;
}

void LiteAsyncTaskWorker::createAsyncTask(int threadCount)
{
    for (int i = 0; i < threadCount; i++)
    {
        // 在堆内存中创建线程对象:将自身指针传递进函数
        thread* ptrThread = new thread(executThread, ref(*this), i);

        // 保存线程        
        this->m_threads.push_back(ptrThread);

        // 活动线程的计数器
        this->increaseActivedCount();
    }
}

void LiteAsyncTaskWorker::close()
{
    // 通过标识通知各线程退出
    this->reqExit();

    // 等待各线程将自己的活动数量递减到零
    while (this->queryActivedCount() > 0)
    {
        this_thread::sleep_for(chrono::milliseconds(10));
    }


    // 回收线程资源，防止泄漏
    for (size_t i = 0; i < this->m_threads.size(); i++)
    {
        thread* ptrThread = this->m_threads[i];
        if (ptrThread->joinable())
        {
            ptrThread->join();
        }

        delete ptrThread;
        ptrThread = nullptr;
    }

    // 清空列表
    this->m_threads.clear();

    // 复位请求退出标记
    this->resetReqExitFlag();
}

void LiteAsyncTaskWorker::executThread(LiteAsyncTaskWorker& taskWorker, int threadId)
{
    while (true)
    {
        // 处理退出信号
        if (taskWorker.queryReqExitFlag()) {
            break;
        }

        // 执行子类的线程函数
        taskWorker.executThreadFunc();

        // 休眠10毫秒
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    // 递减活动线程的数量
    taskWorker.decreaseActivedCount();
}





