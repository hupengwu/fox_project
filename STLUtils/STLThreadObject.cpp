#include "STLThreadObject.h"

STLThreadObject::STLThreadObject()
{
    this->isExit = false;
    this->bFinished = true;
    this->recvThread = nullptr;
}

STLThreadObject::~STLThreadObject()
{
}

void STLThreadObject::createThread()
{
    STLThreadObject* thredObj = this;
    this->setFinished(false);
    this->recvThread = new thread(recvThreadFunc, thredObj);
}

void STLThreadObject::closeThread()
{
    // ֪ͨ�����߳��˳�
    this->setExit(true);

    // ��飺ȫ���߳��Ƿ����н���
    while (!this->getFinished())
    {
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    // �����߳�
    thread* thread = this->recvThread;
    if (thread != nullptr)
    {
        if (thread->joinable())
        {
            thread->join();
        }
        delete this->recvThread;
        this->recvThread = nullptr;
    }


    // ���ñ�ʶ
    this->setFinished(true);
    this->setExit(false);
}

void STLThreadObject::setFinished(bool finished)
{
    lock_guard<mutex> guard(this->lock);
    this->bFinished = finished;
}

bool STLThreadObject::getFinished()
{
    lock_guard<mutex> guard(this->lock);
    return this->bFinished;
}

void STLThreadObject::setExit(bool isExit)
{
    lock_guard<mutex> guard(this->lock);
    this->isExit = isExit;
}

bool STLThreadObject::getExit()
{
    lock_guard<mutex> guard(this->lock);
    return this->isExit;
}

void STLThreadObject::recvThreadFunc(STLThreadObject* threadObj)
{
    while (true)
    {
        // ��飺�˳��̱߳��
        if (threadObj->getExit())
        {
            break;
        }

        threadObj->recvFunc(threadObj);
    }

    threadObj->setFinished(true);
}
