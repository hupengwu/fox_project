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

    // �����������߳�
    std::thread* threadPtr = new std::thread(executThreadFun, ref(*this));
    this->threadPtr = threadPtr;
}

void STLTimedTask::close()
{
    // �����˳������ʶ
    this->setExit(true);

    // ֪ͨ�̴߳����ź�
    this->signal();

    // ��飺�ڲ��߳��Ƿ����н���
    while (!this->getFinished())
    {
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    // ɾ��runnable
    if (this->runnable != nullptr)
    {
        delete this->runnable;
        this->runnable = nullptr;
    }

    // ����threads
    if (this->threadPtr->joinable())
    {
        this->threadPtr->join();
    }

    // ɾ��ָ�����
    delete this->threadPtr;
    this->threadPtr = nullptr;


    // �����ʶ
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
        // �Ƿ�Ϊ�˳����
        if (timedTask.getExit())
        {
            break;
        }

        if (timedTask.fixedInterval)
        {
            // �̶�ʱ����ģʽ:ÿ��timeInterval֮��ִ��һ������

            timeoPoint += chrono::milliseconds(timedTask.timeInterval);
            timedTask.wait_util(timeoPoint);
            // ����ʱ���
            timeoPoint = std::chrono::system_clock::now();
        }
        else
        {
            // ���ʱ��ģʽģʽ:ÿ������֮��ȴ�timeInterval����

            timedTask.wait();
        }
                

        // ִ��runnable
        if (timedTask.runnable != nullptr)
        {
            // ִ�и�runnable
            timedTask.runnable->run();
        }

    }

    // ��ʶһ���߳����н���
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
