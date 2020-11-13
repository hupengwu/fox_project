#pragma once

#include <vector>
#include <map>
#include <thread>
#include <mutex>


using namespace std;

/**
 * 轻量级的多线程任务执行器:使用多线程并行处理来解决一批任务的高强度计算的问题<br>
 * 背景：在做通信解析的时候，控制器返回了一大批分组数据，如果以传统的串行方式逐个逐个分组数据进行处理，非常缓慢耗时几分钟<br>
 * ....那么，能不能把这一大批分组数据通过多线程进行并行处理，然后再把结果合并起来，这样处理速度不就快了吗？<br>
 * ....但是摆在面前的问题是，多线程的并发对普通开发人员来说，要控制好它们可是不小的难题，能不能提供一个简单易用的多线程组件呢，LiteTask就运运而生<br>
 * @author h00163887
 * @since 2019/09/21
 */
class LiteAsyncTaskWorker
{
public:// 需要重载的虚函数
    virtual void executThreadFunc() = 0;

public:
    void createAsyncTask(int threadCount);
    void close();

public:
    LiteAsyncTaskWorker();
    virtual ~LiteAsyncTaskWorker();

   
private:// 内部执行线程的静态函数
    static void executThread(LiteAsyncTaskWorker& taskWorker, int threadId);

private:
    void  increaseActivedCount();// 递增活动线程数量：主线程使用
    void  decreaseActivedCount();// 递减活动线程数量：子线程使用
    int   queryActivedCount();// 查询活动线程数量：主线程使用

    bool  queryReqExitFlag();// 查询退出：子线程函数使用
    void  resetReqExitFlag();// 复位退出：主线程函数使用
    void  reqExit();// 请求退出：主线程函数使用
    bool  isExit();// 是否已经完成退出：主线程函数使用

private:// 多线程并发下，需要保护的变量
    mutex m_mutex;// 互斥    
    int   m_activedCount = 0;// 活动线程数    
    bool  m_reqExitFlag = false;// 请求退出任务的标记

private:
    // 线程
    vector<thread*> m_threads;
};

