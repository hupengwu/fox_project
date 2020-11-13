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
        // �ڶ��ڴ��д����̶߳���:������ָ�봫�ݽ�����
        thread* ptrThread = new thread(executThread, ref(*this), i);

        // �����߳�        
        this->m_threads.push_back(ptrThread);

        // ��̵߳ļ�����
        this->increaseActivedCount();
    }
}

void LiteAsyncTaskWorker::close()
{
    // ͨ����ʶ֪ͨ���߳��˳�
    this->reqExit();

    // �ȴ����߳̽��Լ��Ļ�����ݼ�����
    while (this->queryActivedCount() > 0)
    {
        this_thread::sleep_for(chrono::milliseconds(10));
    }


    // �����߳���Դ����ֹй©
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

    // ����б�
    this->m_threads.clear();

    // ��λ�����˳����
    this->resetReqExitFlag();
}

void LiteAsyncTaskWorker::executThread(LiteAsyncTaskWorker& taskWorker, int threadId)
{
    while (true)
    {
        // �����˳��ź�
        if (taskWorker.queryReqExitFlag()) {
            break;
        }

        // ִ��������̺߳���
        taskWorker.executThreadFunc();

        // ����10����
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    // �ݼ���̵߳�����
    taskWorker.decreaseActivedCount();
}





