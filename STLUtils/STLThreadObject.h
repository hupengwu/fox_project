#pragma once

#include <mutex>
#include <thread>

using namespace std;

/*
* ������
*     ��SOCKET/���ڵ�IO�����У���Ҫһ���򵥵ĺ�̨�첽�߳�ѭ����ȡ���ݵĳ������ṩһ���򵥵��̶߳�����
* ʹ�÷�����
* 1���Ӹ���������
* 2������recvFunc������ʹ����ĺ�̨�߳��ܹ�ִ��������Զ���recvFunc����
* 3���������create/close�׶Σ��ֱ����createThread/closeThread
*/
class STLThreadObject
{
public:
    STLThreadObject();
    virtual ~STLThreadObject();

protected:
    /*
    * �����߳�
    */
    virtual void createThread();

    /*
    * �ر��߳�
    */
    virtual void closeThread();


private:// �߳�״̬
    void		 setFinished(bool finished);
    bool		 getFinished();
    void		 setExit(bool isExit);
    bool		 getExit();

private:// �߳̽���״��
    bool		 isExit;    // �����˳���ʶ
    bool		 bFinished; // �Ƿ��Ѿ��˳��߳�

private:
    /**
     * ����
     */
    mutex		 lock;

    /*
    * ����ѭ���������ݵĺ�̨�̺߳���
    */
    static  void recvThreadFunc(STLThreadObject* threadObj);

    /*
    * ������Ҫʵ�ֵĽ������ݴ�����
    * ���飺�������غ�����ѭ�������ʱ�򣬾�����select()/wait()�Ȼ���ϵͳ���жϵĵȴ�����������������ʹ��usleep()���������ⲻ��Ҫ��CPUռ��
    */
    virtual void recvFunc(STLThreadObject* threadObj) {};

    /*
    * ��������ͻ��˽�����߳�
    */
    thread*      recvThread;
};

