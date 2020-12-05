#pragma once

#include <mutex>
#include <thread>

using namespace std;

/*
* 背景：
*     在SOCKET/串口等IO操作中，需要一个简单的后台异步线程循环读取数据的场景，提供一个简单的线程对象父类
* 使用方法：
* 1、从该类型派生
* 2、重载recvFunc方法，使父类的后台线程能够执行子类的自定义recvFunc方法
* 3、在子类的create/close阶段，分别调用createThread/closeThread
*/
class STLThreadObject
{
public:
    STLThreadObject();
    virtual ~STLThreadObject();

protected:
    /*
    * 创建线程
    */
    virtual void createThread();

    /*
    * 关闭线程
    */
    virtual void closeThread();


private:// 线程状态
    void		 setFinished(bool finished);
    bool		 getFinished();
    void		 setExit(bool isExit);
    bool		 getExit();

private:// 线程结束状况
    bool		 isExit;    // 请求退出标识
    bool		 bFinished; // 是否已经退出线程

private:
    /**
     * 互斥
     */
    mutex		 lock;

    /*
    * 负责循环接收数据的后台线程函数
    */
    static  void recvThreadFunc(STLThreadObject* threadObj);

    /*
    * 子类需要实现的接受数据处理函数
    * 建议：子类重载函数在循环处理的时候，尽量用select()/wait()等基于系统级中断的等待函数，而不是主动使用usleep()函数，避免不必要的CPU占用
    */
    virtual void recvFunc(STLThreadObject* threadObj) {};

    /*
    * 负责监听客户端接入的线程
    */
    thread*      recvThread;
};

