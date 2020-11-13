

#include "Demo2AsyncTaskWorker.h"

#include "LiteAsyncTaskWorker.h"

#include "LiteSyncTaskListPacket.h"

#include "Demo1SyncTaskListPacket.h"

#include "LiteSyncTaskWorker.h"


int main()
{
    // ���ԣ�100�����������б���������ΪInteger
    list<int> dateList;
    for (int i = 0; i < 50000; i++) {
        dateList.push_back(i);

    }
    // ��������
    Demo1SyncTaskListPacket packet;
    packet.setTaskList<int>(dateList);

    // ������߳�ִ������10���߳�
    LiteSyncTaskWorker executor(packet,10);
      
    // ִ�ж��߳�����
    executor.executeTask(packet);


    // ȡ��������
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