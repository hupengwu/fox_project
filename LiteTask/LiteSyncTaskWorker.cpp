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
        // ִ������
        TaskResult result = taskPackage.executFunctionOneTask();

        // ÿ���߳�����Task��ȡ���ˣ���ô�̸߳ý�����
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
    // �����̵߳Ľ���״̬������
    this->resetFinished();

    // �����������߳�
    vector<thread*> threads;
    for (int i = 0; i < this->nThreads; i++)
    {
        // �ڶ��ڴ��д����̶߳���:������ָ�봫�ݽ�����
        thread* ptrThread = new thread(executThread, ref(packet), ref(*this), i);

        // �����߳�        
        threads.push_back(ptrThread);
    }
    
    // �ȴ�ȫ���̴߳������
    while (!this->isFinished()) 
    {
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    // �����߳���Դ����ֹй©
    for (thread* threadPtr : threads)
    {
        if (threadPtr->joinable())
        {
            threadPtr->join();
        }

        delete threadPtr;
        threadPtr = nullptr;
    }

    // ����б�
    threads.clear();

    return true;
}







