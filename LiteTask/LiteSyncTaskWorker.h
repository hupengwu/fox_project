#pragma once


#include <vector>
#include <map>
#include <thread>
#include <mutex>

#include "LiteSyncTaskPacket.h"


using namespace std;

/**
 * 轻量级的异步任务执行器:使用多线程循环处理来解决一批后台任务的问题<br>
 * 背景:很多场景下需要建立一批后台线程，进行后台任务的循环处理，需要降低复杂性<br>
 * 说明:必须实现executThreadFunc函数<br>
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
     * 执行业务
     * @return 是否成功
     */
    bool executeTask(LiteSyncTaskPacket& packet);

private:// 内部执行线程的静态函数
    static void executThread(LiteSyncTaskPacket& taskPackage,LiteSyncTaskWorker& taskWorker, int threadId);

private:// 线程结束状况
    void increFinished();
    void resetFinished();
    bool isFinished();
/**
 * 线程任务数量
 */
private:
    mutex lock;// 互斥  

    int nThreads = 10;
    int nFinished = 0;

private:
    LiteSyncTaskPacket* taskPacket = nullptr;
};
