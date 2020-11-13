#pragma once

#include <vector>
#include <map>
#include <thread>
#include <mutex>


using namespace std;

/**
 * �������Ķ��߳�����ִ����:ʹ�ö��̲߳��д��������һ������ĸ�ǿ�ȼ��������<br>
 * ����������ͨ�Ž�����ʱ�򣬿�����������һ�����������ݣ�����Դ�ͳ�Ĵ��з�ʽ�������������ݽ��д����ǳ�������ʱ������<br>
 * ....��ô���ܲ��ܰ���һ������������ͨ�����߳̽��в��д���Ȼ���ٰѽ���ϲ����������������ٶȲ��Ϳ�����<br>
 * ....���ǰ�����ǰ�������ǣ����̵߳Ĳ�������ͨ������Ա��˵��Ҫ���ƺ����ǿ��ǲ�С�����⣬�ܲ����ṩһ�������õĶ��߳�����أ�LiteTask�����˶���<br>
 * @author h00163887
 * @since 2019/09/21
 */
class LiteAsyncTaskWorker
{
public:// ��Ҫ���ص��麯��
    virtual void executThreadFunc() = 0;

public:
    void createAsyncTask(int threadCount);
    void close();

public:
    LiteAsyncTaskWorker();
    virtual ~LiteAsyncTaskWorker();

   
private:// �ڲ�ִ���̵߳ľ�̬����
    static void executThread(LiteAsyncTaskWorker& taskWorker, int threadId);

private:
    void  increaseActivedCount();// ������߳����������߳�ʹ��
    void  decreaseActivedCount();// �ݼ���߳����������߳�ʹ��
    int   queryActivedCount();// ��ѯ��߳����������߳�ʹ��

    bool  queryReqExitFlag();// ��ѯ�˳������̺߳���ʹ��
    void  resetReqExitFlag();// ��λ�˳������̺߳���ʹ��
    void  reqExit();// �����˳������̺߳���ʹ��
    bool  isExit();// �Ƿ��Ѿ�����˳������̺߳���ʹ��

private:// ���̲߳����£���Ҫ�����ı���
    mutex m_mutex;// ����    
    int   m_activedCount = 0;// ��߳���    
    bool  m_reqExitFlag = false;// �����˳�����ı��

private:
    // �߳�
    vector<thread*> m_threads;
};

