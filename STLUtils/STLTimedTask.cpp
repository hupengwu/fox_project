#include "STLTimedTask.h"

STLTimedTask::STLTimedTask(STLRunnable* runnable, bool fixedInterval)
{
    this->bFinished = false;
    this->isExit = false;
    this->fixedInterval = fixedInterval;

    this->runnable = runnable;    
}

STLTimedTask::~STLTimedTask()
{
}

void STLTimedTask::start(int timeInterval)
{
    this->timeInterval = timeInterval;

    // 创建并启动线程
    std::thread* threadPtr = new std::thread(executThreadFun, ref(*this));
    this->threadPtr = threadPtr;
}

void STLTimedTask::close()
{
    // 设置退出请求标识
    this->setExit(true);

    // 通知线程处理信号
    this->signal();

    // 检查：内部线程是否运行结束
    while (!this->getFinished())
    {
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    // 删除runnable
    if (this->runnable != nullptr)
    {
        delete this->runnable;
        this->runnable = nullptr;
    }

    // 回收threads
    if (this->threadPtr->joinable())
    {
        this->threadPtr->join();
    }

    // 删除指针对象
    delete this->threadPtr;
    this->threadPtr = nullptr;


    // 清除标识
    this->bFinished = false;
    this->isExit = false;
}

void STLTimedTask::setInterval(int timeInterval)
{
    lock_guard<mutex> guard(this->lock);
    this->timeInterval = timeInterval;
}

void STLTimedTask::setFinished()
{
    lock_guard<mutex> guard(this->lock);
    this->bFinished = true;
}

bool STLTimedTask::getFinished()
{
    lock_guard<mutex> guard(this->lock);
    return this->bFinished;
}

void STLTimedTask::setExit(bool isExit)
{
    lock_guard<mutex> guard(this->lock);
    this->isExit = isExit;
}

bool STLTimedTask::getExit()
{
    lock_guard<mutex> guard(this->lock);
    return this->isExit;
}

void STLTimedTask::executThreadFun(STLTimedTask& timedTask)
{   
    std::chrono::system_clock::time_point timeoPoint = std::chrono::system_clock::now();

    while (true)
    {
        // 是否为退出标记
        if (timedTask.getExit())
        {
            break;
        }

        if (timedTask.fixedInterval)
        {
            // 固定时间间隔模式:每个timeInterval之间执行一次任务

            timeoPoint += chrono::milliseconds(timedTask.timeInterval);
            timedTask.wait_util(timeoPoint);
            // 重置时间点
            timeoPoint = std::chrono::system_clock::now();
        }
        else
        {
            // 间隔时间模式模式:每个任务之间等待timeInterval毫秒

            timedTask.wait();
        }
                

        // 执行runnable
        if (timedTask.runnable != nullptr)
        {
            // 执行该runnable
            timedTask.runnable->run();
        }

    }

    // 标识一个线程运行结束
    timedTask.setFinished();
}


void STLTimedTask::signal()
{
    std::unique_lock<std::mutex> lock(this->lock);
    this->cv.notify_one();
}

void STLTimedTask::wait()
{
    std::unique_lock<std::mutex> lock(this->lock);
    this->cv.wait_for(lock, std::chrono::milliseconds(this->timeInterval));
}

void STLTimedTask::wait_util(chrono::system_clock::time_point nextPoint)
{
    std::unique_lock<std::mutex> lock(this->lock);
    this->cv.wait_until(lock, nextPoint);
}
