#include "STLThreadUtils.h"

void fun1(int n)  //初始化构造函数  
{
    n += 10;
    this_thread::sleep_for(chrono::milliseconds(10));
}
void fun2(int& n) //拷贝构造函数  
{
    n += 20;
    this_thread::sleep_for(chrono::milliseconds(10));
}

int STLThreadUtils::demo()
{
    int n = 0;

    // 线程对象1：通过函数1和函数需要的【值】参数，构造一个线程对象
    thread t1(fun1, n + 1);  //按照值传递  ，n参数在各个线程都有一个副本
    t1.join();// 等待线程结束


    // 线程对象2：通过函数2和函数需要的【引用】参数，构造一个线程对象
    n = 10;
    thread t2(fun2, ref(n)); //引用  ，n参数被各个线程共同的跨线程操作，最好要注意通过临界区进行保护
    //没有join，所以线程可能还没结束

    //  线程对象3：将线程对象2执行起来
    thread t3(move(t2));     //t3执行t2，t2不是thread  
    t3.join();//等待线程结束

    return 0;
}
