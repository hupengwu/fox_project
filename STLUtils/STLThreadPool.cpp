#include "STLThreadPool.h"

STLThreadPool::STLThreadPool()
{
    // ��ʼ�����
    this->nFinished = 0;
    this->isExit = false;
    this->nRunning = 0;
}

STLThreadPool::~STLThreadPool()
{    
}

STLThreadPool* STLThreadPool::newThreadPool(int nThreads)
{
    // ʵ����һ���յ��̳߳ض���
    STLThreadPool* threadPool = new STLThreadPool();

    // �����������߳�
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
        // ����һ���ź�
        threadPool->semaphore.signal();
    }

    // ��飺ȫ���߳��Ƿ����н���
    while (!threadPool->getFinished())
    {
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    // ɾ��idles
    for (list<STLRunnable*>::iterator it = threadPool->runnables.begin(); it != threadPool->runnables.end(); it++)
    {
        delete *it;
    }
    threadPool->runnables.clear();

    // ����threads
    for (list<thread*>::iterator it = threadPool->threads.begin(); it != threadPool->threads.end(); it++)
    {
        thread* thread = *it;

        // �����߳�
        if (thread->joinable())
        {
            thread->join();
        }
    }

    // ɾ��threads
    for (list<thread*>::iterator it = threadPool->threads.begin(); it != threadPool->threads.end(); it++)
    {
        delete* it;
    }   
    threadPool->threads.clear();

    // �����ʶ
    threadPool->nFinished = 0;
    threadPool->isExit = false;    

    // �����źŵ�
    threadPool->semaphore.reset();

    // ɾ���̳߳�
    delete threadPool;
    threadPool = nullptr;
}

bool STLThreadPool::isBusy()
{
    lock_guard<mutex> guard(this->lock);
    return this->nRunning >= this->threads.size();
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

void STLThreadPool::setRunning(bool increase)
{
    lock_guard<mutex> guard(this->lock);
    if (increase)
    {
        this->nRunning++;
    }
    else
    {
        this->nRunning--;
    }
    
}

int STLThreadPool::getRunning()
{
    lock_guard<mutex> guard(this->lock);
    return this->nRunning;
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

    // ֪ͨһ�����õ��߳�ȥȡ
    this->runnables.push_back(runnable);
    this->semaphore.signal(); 

    // ��������һ�������̻߳ᱻ�������getIdleRunnable()��ȡ�ߴ�ִ�е�Runnable
}

STLRunnable* STLThreadPool::getRunnable()
{
    lock_guard<mutex> guard(this->lock);

    // ��飺��û�����õ�runnable
    if (this->runnables.empty())
    {
        return nullptr;
    }

    // �����õ�runnable��ת�Ƶ����е�runnable�б���
    STLRunnable* runnal = this->runnables.front();
    this->runnables.pop_front();

    return runnal;
}

void STLThreadPool::executThreadFun(STLThreadPool& threadPool)
{
    while (true)
    {
        // �ȴ����ȸ��̵߳��źŵ��ÿ���źŶ�ִ��һ�����°��ŵ�runnable�������˳���
        threadPool.semaphore.wait();

        // �Ƿ�Ϊ�˳����
        if (threadPool.getExit())
        {
            break;
        }
        else
        {
            // ȡ��һ����ִ�е�runnable
            STLRunnable* runnable = threadPool.getRunnable();
            if (runnable != nullptr)
            {
                threadPool.setRunning(true);

                // ִ�и�runnable
                runnable->run();

                threadPool.setRunning(false);
            }
        }
    }

    // ��ʶһ���߳����н���
    threadPool.setFinished();
}
