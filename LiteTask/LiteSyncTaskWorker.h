#pragma once


#include <vector>
#include <map>
#include <thread>
#include <mutex>

#include "LiteSyncTaskPacket.h"


using namespace std;

/**
 * ���������첽����ִ����:ʹ�ö��߳�ѭ�����������һ����̨���������<br>
 * ����:�ܶೡ������Ҫ����һ����̨�̣߳����к�̨�����ѭ��������Ҫ���͸�����<br>
 * ˵��:����ʵ��executThreadFunc����<br>
* @author h00163887
* @since 2019/09/21
*/
class LiteSyncTaskWorker
{
public:
    LiteSyncTaskWorker(LiteSyncTaskPacket& taskPacketObj, int nThreads);
    virtual ~LiteSyncTaskWorker();

public:
    /**
     * ִ��ҵ��
     * @return �Ƿ�ɹ�
     */
    bool executeTask(LiteSyncTaskPacket& packet);

private:// �ڲ�ִ���̵߳ľ�̬����
    static void executThread(LiteSyncTaskPacket& taskPackage,LiteSyncTaskWorker& taskWorker, int threadId);

private:// �߳̽���״��
    void increFinished();
    void resetFinished();
    bool isFinished();
/**
 * �߳���������
 */
private:
    mutex lock;// ����  

    int nThreads = 10;
    int nFinished = 0;

private:
    LiteSyncTaskPacket* taskPacket = nullptr;
};
