

#include "Demo2AsyncTaskWorker.h"

#include "LiteAsyncTaskWorker.h"

#include "LiteSyncTaskListPacket.h"

#include "Demo1SyncTaskListPacket.h"

#include "LiteSyncTaskWorker.h"


int main()
{
    // 测试：100个任务对象的列表，对象类型为Integer
    list<int> dateList;
    for (int i = 0; i < 50000; i++) {
        dateList.push_back(i);

    }
    // 测试任务：
    Demo1SyncTaskListPacket packet;
    packet.setTaskList<int>(dateList);

    // 定义多线程执行器：10个线程
    LiteSyncTaskWorker executor(packet,10);
      
    // 执行多线程任务
    executor.executeTask(packet);


    // 取出计算结果
    list<double> result = packet.getResultList<double>();
    result.size();
    result.clear();

    printf("thread id = %d\n", 1);
    /*
    for (size_t i = 0; i < 500; i++)
    {
        if (i%1000==1)
        {
            int                j = 0;
        }
        packet.setTaskList(dateList);
        executor.executeTask(packet);
        result = packet.getResultList<double>();
    }
    

    printf("thread id = %d\n", 2);
    
    Demo2AsyncTaskWorker worker;

    worker.createAsyncTask(10);

    this_thread::sleep_for(chrono::milliseconds(1000*1));

    worker.close();

    worker.createAsyncTask(10);

    this_thread::sleep_for(chrono::milliseconds(1000 * 1));

    worker.close();

  //  this_thread::sleep_for(chrono::milliseconds(10000000));
  */
    
}